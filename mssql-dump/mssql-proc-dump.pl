#!/usr/bin/perl -w

#
# Usage:
#   $0 <server> <db> <user> <password> [<proc-name> [<proc-name [...]]]
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

if(scalar @ARGV > 4)
{
    shift @ARGV;
    shift @ARGV;
    shift @ARGV;
    shift @ARGV;
    $proclist = join ' ', @ARGV;
}
else
{
    $db = DBI->connect("dbi:Sybase:server=$server;database=$database", $user, $password);
    $procs = get_procs($db);
    $proclist = join ' ', @$procs;
    $db->disconnect();
}

system("defncopy -S '$server' -U '$user' -P '$password' -D '$database' ".$proclist);
