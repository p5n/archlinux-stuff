#!/usr/bin/perl -w

use DBI;

# ####################################################################
# GLOBAL CONFIGURATION
# ####################################################################

$MODULES_DIR = "./";
$BASE_DB_DIR = "/tmp";

# ####################################################################
# Check usage, read parameters
# ####################################################################

if($#ARGV lt 2)
{
    print STDERR "Usage: my-own-repo-update [-i|--init] <pkg-format> <repo-name> <path-to-packages>\n";
    exit 1;
}

if( ($ARGV[0] =~ /^-i$/) || ($ARGV[0] =~ /^--init$/) )
{
    $initflag = 1;
    $driver = $ARGV[1];
    $reponame = $ARGV[2];
    $DIR = $ARGV[3];
}
else
{
    $initflag = 0;
    $driver = $ARGV[0];
    $reponame = $ARGV[1];
    $DIR = $ARGV[2];
}

do $MODULES_DIR."/".$driver.".pm";

# ####################################################################
# FUNCTIONS
# ####################################################################

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
# Does pkg exist?
#
sub pkg_exist
{
    my ($db, $pkgname) = @_;
    my ($all, $exist);

    $exist = 0;
    $all = $db->selectall_arrayref("SELECT pkgname FROM packages WHERE pkgname='$pkgname'");

    foreach my $row (@$all)
    {
	$exist = 1;
    }
    return $exist;
}

# ####################################################################
# MAIN
# ####################################################################

if($initflag)
{
    unlink $BASE_DB_DIR."/".$reponame.".db";
}

my $db = DBI->connect("dbi:SQLite:".$BASE_DB_DIR."/".$reponame.".db", "", "", {RaiseError => 1, AutoCommit => 1});

if($initflag)
{
    init_db($db);
}

$ctime = time;

$all = $db->selectall_arrayref("SELECT value FROM state WHERE name='lasttime'");

foreach my $row (@$all)
{
    $lasttime = $$row[0];
    print STDERR "LAST UPDATE = $lasttime\n";
}

opendir DIR, $DIR || die("cannot open packages dir");

while($packagefile = readdir DIR)
{
    unless($packagefile =~ /^\./)
    {
	$packagefile = $DIR."/".$packagefile;
	# ($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,$atime,$mtime,$ctime,$blksize,$blocks)
	@stat = stat($packagefile);
	$mtime = $stat[9];
	if($mtime > $lasttime)
	{
	    ($pkgname, $pkgver, $pkgdesc, $url, $builddate, $packager, $size, $arch, $license, $depend, $filelist) = add_package($packagefile);
	    if(pkg_exist($db, $pkgname))
	    {
		print STDERR "Updating: $packagefile\n";
		$db->do("UPDATE packages SET pkgver='$pkgver', pkgdesc='$pkgdesc', url='$url', builddate='$builddate', packager='$packager', size=$size, arch='$arch', license='$license', depend='$depend', filelist=\"$filelist\" WHERE pkgname='$pkgname'");
	    }
	    else
	    {
		print STDERR "Adding: $packagefile\n";
		$db->do("INSERT INTO packages VALUES (NULL, '$pkgname', '$pkgver', '$pkgdesc', '$url', '$builddate', '$packager', $size, '$arch', '$license', '$depend', \"$filelist\")");
	    }
	}
    }
}

closedir DIR;

# update finished
$db->do("UPDATE state SET value='$ctime' WHERE name='lasttime'");
