#!/usr/bin/perl -w

#
# Usage:
#   $0 <server> <db> <user> <password> [<table-name> [<table-name [...]]]
#

use DBI;
use Encode;

do("mssql-lib.pl");

#
# MAIN
#

$server=$ARGV[0];
$database=$ARGV[1];
$user=$ARGV[2];
$password=$ARGV[3];

$db = DBI->connect("dbi:Sybase:server=$server;database=$database", $user, $password);

@tables = ();

if(scalar @ARGV > 4)
{
    shift @ARGV;
    shift @ARGV;
    shift @ARGV;
    shift @ARGV;
    @tables = @ARGV;
}
else
{
    @tables = @{get_tables($db)};
}

foreach $t (@tables)
{
    print STDERR "Dumping $t...\n";
    table_structure_dump($db, $t);
    table_dump($db, $t);
}

$db->disconnect();
