#!/usr/bin/perl -w

use DBI;

sub add_package
{
    my ($packagefile) = @_;
    my ($pkgname, $pkgver, $pkgdesc, $url, $builddate, $packager, $size, $arch, $license, $depend, $filelist);
    $pkgname = "n/a";
    $pkgver = "n/a";
    $pkgdesc = "n/a";
    $url = "unknown";
    $builddate = "unknown";
    $packager = "unknown";
    $size = "unknown";
    $arch = "unknown";
    $license = "unknown";
    $depend = "";
    $backup = "";
    $filelist = "n/a";

    $filelist = `tar xf $packagefile .FILELIST -O`;

    open(FH, "tar xf $packagefile .PKGINFO -O|") || die("tar failed");

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
            $depend = $depend.$1.",";
        }
        if(/^backup = (.+)$/)
        {
            $backup = $backup.$1.",";
        }
    }

    close FH;

    chop $depend;
    chop $backup;

    return ($pkgname, $pkgver, $pkgdesc, $url, $builddate, $packager, $size, $arch, $license, $depend, $backup, $filelist);
}

return TRUE;
