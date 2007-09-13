#!/usr/bin/perl -w

use DBI;

use pacman;
use rpm;
use dpkg;

my $db = DBI->connect("dbi:SQLite:/tmp/test.db", "", "", {RaiseError => 1, AutoCommit => 1});
$db->do("CREATE TABLE packages (id INTEGER PRIMARY KEY, pkgname varchar(512), pkgver varchar(512), pkgdesc varchar(512), url varchar(512), builddate varchar(64), packager varchar(512), size integer, arch varchar(32), license varchar(512), depend varchar(512), filelist TEXT)");

while(<>)
{
    chomp;
    $packagefile = $_;
    add_package_pacman($db, $packagefile);
}
