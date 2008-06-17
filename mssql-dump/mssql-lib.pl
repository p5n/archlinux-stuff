#!/usr/bin/perl -w

use DBI;

#
# Initialization
#

#$ echo -e "use master\nGO\nselect xtype,name,length,xprec,xscale,variable from systypes order by xtype\nGO\n" | \
#    fisql -S local -U sa -P password | \
#    perl -ne "if(/(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s*/) {print \"\$1 => {name=>'\$2', len=>\$3, prec=>\$4, scale=>\$5, variable=>\$6},\\n\";}" | \
#    tail -n +3

%MSSQLTYPES = (
    34 => {name=>'image', len=>16, prec=>0, scale=>0, variable=>0},
    35 => {name=>'text', len=>16, prec=>0, scale=>0, variable=>0},
    36 => {name=>'uniqueidentifier', len=>16, prec=>0, scale=>0, variable=>0},
    48 => {name=>'tinyint', len=>1, prec=>3, scale=>0, variable=>0},
    52 => {name=>'smallint', len=>2, prec=>5, scale=>0, variable=>0},
    56 => {name=>'int', len=>4, prec=>10, scale=>0, variable=>0},
    58 => {name=>'smalldatetime', len=>4, prec=>16, scale=>0, variable=>0},
    59 => {name=>'real', len=>4, prec=>24, scale=>0, variable=>0},
    60 => {name=>'money', len=>8, prec=>19, scale=>4, variable=>0},
    61 => {name=>'datetime', len=>8, prec=>23, scale=>3, variable=>0},
    62 => {name=>'float', len=>8, prec=>53, scale=>0, variable=>0},
    98 => {name=>'sql_variant', len=>8016, prec=>0, scale=>0, variable=>0},
    99 => {name=>'ntext', len=>16, prec=>0, scale=>0, variable=>0},
    104 => {name=>'bit', len=>1, prec=>1, scale=>0, variable=>0},
    106 => {name=>'decimal', len=>17, prec=>38, scale=>38, variable=>0},
    108 => {name=>'numeric', len=>17, prec=>38, scale=>38, variable=>0},
    122 => {name=>'smallmoney', len=>4, prec=>10, scale=>4, variable=>0},
    127 => {name=>'bigint', len=>8, prec=>19, scale=>0, variable=>0},
    165 => {name=>'varbinary', len=>8000, prec=>0, scale=>0, variable=>1},
    167 => {name=>'varchar', len=>8000, prec=>0, scale=>0, variable=>1},
    173 => {name=>'binary', len=>8000, prec=>0, scale=>0, variable=>0},
    175 => {name=>'char', len=>8000, prec=>0, scale=>0, variable=>0},
    189 => {name=>'timestamp', len=>8, prec=>0, scale=>0, variable=>0},
    231 => {name=>'sysname', len=>256, prec=>0, scale=>0, variable=>1},
    231 => {name=>'nvarchar', len=>8000, prec=>0, scale=>0, variable=>1},
    239 => {name=>'nchar', len=>8000, prec=>0, scale=>0, variable=>0}
);

%TYPES = ();
%NEEDSIZE = ();
%ITEMSIZE = ();
%NEEDQUOTES = ();
foreach $i (keys %MSSQLTYPES)
{
    $TYPES{$i} = $MSSQLTYPES{$i}{name};
    $NEEDSIZE{$i} = $MSSQLTYPES{$i}{variable};
    $ITEMSIZE{$i} = 1;
    $NEEDQUOTES{$i} = 0;
}

$ITEMSIZE{99} = 2;
$ITEMSIZE{231} = 2;
$ITEMSIZE{239} = 2;

$NEEDQUOTES{34} = 1;
$NEEDQUOTES{35} = 1;
$NEEDQUOTES{36} = 1;
$NEEDQUOTES{61} = 1;
$NEEDQUOTES{99} = 1;
$NEEDQUOTES{167} = 1;
$NEEDQUOTES{175} = 1;
$NEEDQUOTES{231} = 1;

#
# #############################################################
#

sub get_procs
{
    my $db = shift;
    my $sql = "SELECT name, id, xtype FROM sysobjects";
    my $q = $db->prepare($sql);
    $q->execute();

    my $r;
    my $ret = [];

    while(@$r = $q->fetchrow_array())
    {
        if($r->[2] eq "P ")
        {
            push @$ret, $r->[0];
        }
    }
    return $ret;
}

sub get_table_columns
{
    my $db = shift;
    my $table = shift;
    my $sql = "select name, xtype, length, xprec from syscolumns where id in (select id from sysobjects where name = '$table')";
    my $q = $db->prepare($sql);
    $q->execute();

    my $r;
    my $ret = [];
    my $c = 0;

    while(@$r = $q->fetchrow_array())
    {
        $ret->[$c] = {};
        $ret->[$c]{name} = "[".$r->[0]."]";
        $ret->[$c]{type} = $r->[1];
        $ret->[$c]{len} = $r->[2];
        $ret->[$c]{prec} = $r->[3];
        $c++;
    }
    return $ret;
}

sub table_dump
{
    my $db = shift;
    my $table = shift;

    my $cols = get_table_columns($db, $table);
    my @colnames = ();
    my %colquotes = ();

    map
    {
        push @colnames, $_->{name};
        $coltypes{$_->{name}} = $NEEDQUOTES{$_->{type}};
    } @$cols;

    my $sql = "select ".join(',', @colnames)." from $table";
    my $q = $db->prepare($sql);
    $q->execute();
    my (@r, $i);

    print "--\n";
    print "-- DATA: $table\n";
    print "--\n";

    while(@r = $q->fetchrow_array())
    {
        print "insert into $table values (";
        for($i=0; $i<scalar @r;$i++)
        {
            if(!defined $r[$i])
            {
                $r[$i] = "NULL";
            }
            elsif($coltypes{$colnames[$i]})
            {
                $r[$i] = $db->quote($r[$i]);
            }
        }
        print join(',', @r).")\n";
    }

    print "GO\n\n";
}

sub table_structure_dump
{
    my $db = shift;
    my $table = shift;

    my $cols = get_table_columns($db, $table);

    print "--\n";
    print "-- CREATE: $table\n";
    print "--\n";

    $sql = "CREATE TABLE [$table](\n";
    foreach $i (@$cols)
    {
        $sql .= "    ";
        $sql .= $i->{name};
        $sql .= " ";
        $sql .= $TYPES{$i->{type}};
        if($NEEDSIZE{$i->{type}})
        {
            $sql .= "(";
            $sql .= $i->{len}/$ITEMSIZE{$i->{type}};
            $sql .= ")";
        }
        unless($i == $cols->[scalar @$cols - 1])
        {
            $sql .= ",";
        }
        $sql .= "\n";
    }
    $sql .= "  )\nGO\n\n";

    print $sql;
}


sub get_tables
{
    my $db = shift;
    my $sql = "SELECT name, id, xtype FROM sysobjects";
    my $q = $db->prepare($sql);
    $q->execute();

    my $r;
    my $ret = [];

    while(@$r = $q->fetchrow_array())
    {
        if($r->[2] eq "U ")
        {
            push @$ret, $r->[0];
        }
    }
    return $ret;
}
