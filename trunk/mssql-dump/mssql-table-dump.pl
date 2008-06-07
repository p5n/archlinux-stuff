#!/usr/bin/perl -w

use DBI;
use Encode;

%TYPES = (
    56 => "int",
    167 => "varchar",
    104 => "bit",
    36 => "uniqueidentifier",
    231 => "nvarchar",
    61 => "datetime",
    60 => "money",
    34 => "image",
    99 => "ntext",
    127 => "bigint"
);

%NEEDSIZE = (
    56 => 0,
    167 => 1,
    104 => 1,
    36 => 0,
    231 => 1,
    61 => 0,
    60 => 0,
    34 => 1,
    99 => 1,
    127 => 0
);

%NEEDQUOTES = (
    56 => 0,
    167 => 1,
    104 => 0,
    36 => 0,
    231 => 1,
    61 => 1,
    60 => 0,
    34 => 1,
    99 => 1,
    127 => 0
);

sub get_table_columns
{
    my $db = shift;
    my $table = shift;
    my $sql = "select name, xtype, length from syscolumns where id in (select id from sysobjects where name = '$table')";
    my $q = $db->prepare($sql);
    $q->execute();

    my $r;
    my $ret = [];
    my $c = 0;

    while(@$r = $q->fetchrow_array())
    {
	$ret->[$c] = {};
        $ret->[$c]{name} = $r->[0];
        $ret->[$c]{type} = $r->[1];
        $ret->[$c]{len} = $r->[2];
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
		$r[$i] = "'".$r[$i]."'";
	    }
	}
	print join(',', @r).")\n";
    }
}

#
# MAIN
#

$server="local";

#$database="iWorldServerDB";
#$user="iWorldDBPublicUser";
#$password="VsRhextDct[@))%1";

#$database="iStatServerDB";
#$user="iStatDBUser";
#$password="mjnGfhjkm{eqGjl,thtim@))^";

$database="iWorldServerDB";
$user="sa";
$password="nt_admin";

$db = DBI->connect("dbi:Sybase:server=$server;database=$database", $user, $password);

table_dump($db, "Persons");

$db->disconnect();
