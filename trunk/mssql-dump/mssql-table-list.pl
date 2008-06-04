#!/usr/bin/perl -w

use DBI;
use Encode;

# select * from syscolumns where id in (select id from sysobjects where name = 'Logins')

$db = DBI->connect('dbi:Sybase:server=localdb;database=iStatServerDB',
		   'iStatDBUser',
		   'mjnGfhjkm{eqGjl,thtim@))^');

$sql = "SELECT name, id, xtype FROM sysobjects";

$q = $db->prepare($sql);
$q->execute();

while(@r = $q->fetchrow_array())
{
    if($r[2] eq "U ")
    {
	print "$r[0] $r[1]\n";
    }
}

$db->disconnect();
