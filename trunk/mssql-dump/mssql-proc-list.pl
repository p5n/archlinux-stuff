#!/usr/bin/perl -w

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
$procs = get_procs($db);
$db->disconnect();

system("defncopy -S '$server' -U '$user' -P '$password' -D '$database' ".join ' ', @$procs);
