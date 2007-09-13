#!/usr/bin/perl -w

use DBI;

use pacman;
use rpm;
use dpkg;

$DIR = "./pkg";

#
# DB initialization
#
sub init_db
{
    my ($db) = @_;
    $db->do("CREATE TABLE packages (id INTEGER PRIMARY KEY, pkgname varchar(512), pkgver varchar(512), pkgdesc varchar(512), url varchar(512), builddate varchar(64), packager varchar(512), size integer, arch varchar(32), license varchar(512), depend varchar(512), filelist TEXT)");
    $db->do("CREATE TABLE state (id INTEGER PRIMARY KEY, name varchar(512), value varchar(512))");
    $db->do("INSERT INTO state VALUES (NULL, 'lasttime', '0')");
}

#
# MAIN
#

my $db = DBI->connect("dbi:SQLite:/tmp/test.db", "", "", {RaiseError => 1, AutoCommit => 1});

#init_db($db);

$ctime = time;

$all = $db->selectall_arrayref("SELECT value FROM state WHERE name='lasttime'");

foreach my $row (@$all)
{
    $lasttime = $$row[0];
    print "LAST UPDATE = $lasttime\n";
}

opendir DIR, $DIR || die("cannot open packages dir");

while($packagefile = readdir DIR)
{
    unless($packagefile =~ /^\./)
    {
	$packagefile = $DIR."/".$packagefile;
	# ($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,$atime,$mtime,$ctime,$blksize,$blocks)
	@stat = stat($packagefile);
#	$size = $stat[7];
#	$atime = $stat[8];
	$mtime = $stat[9];
#	$ctime = $stat[10];
print "$packagefile : $mtime > $lasttime\n";
	if($mtime > $lasttime)
	{
	    add_package_pacman($db, $packagefile);
	}
    }
}

closedir DIR;

# update finished
$db->do("UPDATE state SET value='$ctime' WHERE name='lasttime'");
