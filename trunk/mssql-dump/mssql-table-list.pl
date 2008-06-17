#!/usr/bin/perl -w

#
# Usage:
#   $0 <server> <db> <user> <password> [--table-names-only]
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

$tables = get_tables($db);

foreach $i (@$tables)
{
    $t = get_table_columns($db, $i);
    print "$i\n";
    if(scalar @ARGV <= 4)
    {
        foreach $j (@$t)
        {
            if($NEEDSIZE{$j->{type}}==2)
            {
                print "    ".$j->{name}."(".$TYPES{$j->{type}}.":".$j->{len}.",".$j->{prec}.")\n";
            }
            elsif($NEEDSIZE{$j->{type}}==1)
            {
                print "    ".$j->{name}."(".$TYPES{$j->{type}}.":".$j->{len}.")\n";
            }
            else
            {
                print "    ".$j->{name}."(".$TYPES{$j->{type}}.")\n";
            }
        }
    }
}

$db->disconnect();
