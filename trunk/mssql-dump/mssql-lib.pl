#!/usr/bin/perl -w

use DBI;

%TYPES = (
    34 => "image",
    35 => "text",
    36 => "uniqueidentifier",
    56 => "int",
    60 => "money",
    61 => "datetime",
    62 => "float",
    99 => "ntext",
    104 => "bit",
    106 => "decimal",
    127 => "bigint",
    167 => "varchar",
    175 => "char",
    231 => "nvarchar"
);

%NEEDSIZE = (
    34 => 1,
    35 => 1,
    36 => 0,
    56 => 0,
    60 => 0,
    61 => 0,
    62 => 0,
    99 => 1,
    104 => 1,
    106 => 2,
    127 => 0,
    167 => 1,
    175 => 1,
    231 => 1
);

%NEEDQUOTES = (
    34 => 1,
    35 => 1,
    36 => 0,
    56 => 0,
    60 => 0,
    61 => 1,
    62 => 0,
    99 => 1,
    104 => 0,
    106 => 0,
    127 => 0,
    167 => 1,
    175 => 1,
    231 => 1
);

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

    map {push @colnames, $_->{name}; $coltypes{$_->{name}} = $NEEDQUOTES{$_->{type}}} @$cols;

    my $sql = "select ".join(',', @colnames)." from $table";
    my $q = $db->prepare($sql);
    $q->execute();
    my (@r, $i);

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
