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

sub get_proc_text
{
    my $db = shift;
    my $name = shift;
    return $ret;
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
$procs = get_procs($db);
$db->disconnect();

system("defncopy -S '$server' -U '$user' -P '$password' -D '$database' ".join ' ', @$procs);
