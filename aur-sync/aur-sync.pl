#!/usr/bin/perl -w

$destdir = "/var/abs/unsupported";
$aur_zone_shift = -6*3600;

use POSIX;
use Time::Zone;

%PACKAGES=();

%months = ('Jan'=>1, 'Feb'=>2, 'Mar'=>3, 'Apr'=>4, 'May'=>5, 'Jun'=>6,
	   'Jul'=>7, 'Aug'=>8, 'Sep'=>9, 'Oct'=>10, 'Nov'=>11, 'Dec'=>12);

mkdir($destdir);
chdir($destdir) || die "ERROR: can not access $destdir";

$zone_shift = tz_local_offset();

printf("Local time zone shift: %.1f \n", $zone_shift/3600);
print "Getting index...\n";

$ret = system("rm -f index.html && wget -nv http://aur.archlinux.org/packages/?");

if($ret)
{
    die "ERROR: Getting index failed";
}

open(FH, "./index.html") || die "ERROR: can not open index";

print "Fetching pkgbuilds...\n";

while(<FH>)
{
    if(/.a href=.(\S+)\/">/)
    {
	$name = $1;
	if(/(\d{2})-(\S{3})-(\d{4}) (\d{2}):(\d{2})\s+/)
	{
	    $PACKAGES{$name} = POSIX::mktime(0, $5, $4, $1, $months{$2}-1, $3-1900);
	}
    }
}

close(FH);

foreach $i (sort keys %PACKAGES)
{
    $ctime = (stat("$i.tar.gz"))[10];
    if(!defined $ctime)
    {
	$ctime = (stat("_$i.mark"))[10];
    }

    $ret = 0;

    if( (defined $ctime) && ($ctime-$zone_shift < $PACKAGES{$i}-$aur_zone_shift) )
    {
	print "===> $i $ctime < ".$PACKAGES{$i}."\n";
	$ret = system("rm -f $i.tar.gz && wget -nv http://aur.archlinux.org/packages/$i/$i.tar.gz?");
    }
    elsif(!defined $ctime)
    {
	print "===> $i 0 < ".$PACKAGES{$i}."\n";
	$ret = system("rm -f $i.tar.gz && wget -nv http://aur.archlinux.org/packages/$i/$i.tar.gz?");
    }

    system("touch ./_$i.mark") unless(stat("$i.tar.gz"));
    print "ERROR: $i downloading failed\n" if($ret);
}

print "DONE\n";
