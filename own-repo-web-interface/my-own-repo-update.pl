#!/usr/bin/perl -w

use DBI;

$packagefile = pop @ARGV;

my $db = DBI->connect("dbi:SQLite:test.db", "", "", {RaiseError => 1, AutoCommit => 1});

$db->do("CREATE TABLE packages (id INTEGER PRIMARY KEY, pkgname varchar(512),
pkgver varchar(512), pkgdesc varchar(512), url varchar(512), builddate varchar(64),
packager varchar(512), size integer, arch varchar(32), license varchar(512), depend varchar(512),
filelist TEXT)");

$filelist=`tar xf $packagefile .FILELIST -O`;

open FH, "tar xf $packagefile .PKGINFO -O|" || die("tar failed");

while(<FH>)
{
    if(/^pkgname = (.+)$/)
    {
	$pkgname = $1;
    }
    if(/^pkgver = (.+)$/)
    {
	$pkgver = $1;
    }
    if(/^pkgdesc = (.+)$/)
    {
	$pkgdesc = $1;
    }
    if(/^url = (.+)$/)
    {
	$url = $1;
    }
    if(/^builddate = (.+)$/)
    {
	$builddate = $1;
    }
    if(/^packager = (.+)$/)
    {
	$packager = $1;
    }
    if(/^size = (\d+)$/)
    {
	$size = $1;
    }
    if(/^arch = (.+)$/)
    {
	$arch = $1;
    }
    if(/^license = (.+)$/)
    {
	$license = $1;
    }
    if(/^depend = (.+)$/)
    {
	$depend = $1;
    }
}

close FH;

$db->do("INSERT INTO packages VALUES (NULL, '$pkgname', '$pkgver', '$pkgdesc', '$url', '$builddate', '$packager', $size, '$arch', '$license', '$depend', \"$filelist\")");
