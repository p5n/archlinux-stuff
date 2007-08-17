#!/usr/bin/perl -w

if($#ARGV < 3)
{
    print "Usage: arch-repodiff-diff <basedir> <destdir> <repo> <cssurl>\n";
    exit 1;
}

$cssurl = $ARGV[3];
$repo = $ARGV[2];
$destdir = $ARGV[1];
$basedir = $ARGV[0];

$arch1 = "i686";
$arch2 = "x86_64";

#####################################

$title = "$repo diff ($arch1 vs $arch2) at ".`date +%Y.%m.%d.%H.%m`;

!system("mkdir -p $destdir") || die "can not create destdir";

$template = "[a-z0-9+\\-\\._]+";

%PKGS=();

#
# Search 1st arch
#

print "Searching $arch1/$repo...\n";

open FIND, "find $basedir/$arch1/$repo -name PKGBUILD -exec dirname {} \\; |" || die "can not find files";

while(<FIND>)
{
    chomp;
    if(/\/($template)\/($template)$/i)
    {
        $group = $1;
	$pkg = $2;
	$PKGS{$group."/".$pkg} = 1;
    }
    else
    {
	die "bad find result";
    }
}

close FIND;

#
# Search 2nd arch
#

print "Searching $arch2/$repo...\n";

open FIND, "find $basedir/$arch2/$repo -name PKGBUILD -exec dirname {} \\; |" || die "can not find files";

while(<FIND>)
{
    chomp;
    if(/\/($template)\/($template)$/i)
    {
        $group = $1;
	$pkg = $2;
	$PKGS{$group."/".$pkg} = 1;
    }
    else
    {
	die "bad find result";
    }
}

close FIND;

#
# Diff
#

print "Comparing...\n";

open IDXFH, ">$destdir/index.html" || die "Can not open index.html";

print IDXFH "<HTML><HEAD><TITLE>$title</TITLE></HEAD>\n";
print IDXFH "<link rel=\"stylesheet\" type=\"text/css\" href=\"$cssurl\" media=\"screen\">\n";
print IDXFH "<BODY><H1>$title</H1>\n";
print IDXFH "<TABLE width='100%' class=table>\n";
print IDXFH "<TR class=header><TH>Group<TH>Package<TH>$arch1<TH>$arch2</TR>\n";

$counter = 0;

foreach $i (sort keys %PKGS)
{
    if($i =~ /^(.+)\/(.+)$/)
    {
        $group = $1;
	$pkg = $2;
    }
    else
    {
        die("bad param");
    }

    $p1 = `arch-repodiff-pkginfo $basedir/$arch1/$repo/$group/$pkg/PKGBUILD`;
    $p2 = `arch-repodiff-pkginfo $basedir/$arch2/$repo/$group/$pkg/PKGBUILD`;

    $p1 = "&nbsp;" if($p1 eq "");
    $p2 = "&nbsp;" if($p2 eq "");

    if($p1 ne $p2)
    {
	print "DIFF: $repo-$group-$pkg\n";

	if( ($p1 ne "-") && ($p2 ne "-") )
	{
    	    system("diff2html -N $basedir/$arch1/$repo/$group/$pkg/PKGBUILD $basedir/$arch2/$repo/$group/$pkg/PKGBUILD >$destdir/$repo-$group-$pkg.html");
	    $diffurl = "<A HREF=\"$repo-$group-$pkg.html\">$pkg</A>";
	}
	else
	{
	    $diffurl = "$pkg";
	}
	if($p1 lt $p2)
	{
    	    print IDXFH "<TR class=row$counter><TD>$group<TD>$diffurl<TD class=outdated>$p1<TD>$p2</TR>\n";
	}
	else
	{
    	    print IDXFH "<TR class=row$counter><TD>$group<TD>$diffurl<TD>$p1<TD class=outdated>$p2</TR>\n";
	}
	$counter++;
	$counter %= 2;
    }
}

print IDXFH "</TABLE></BODY></HTML>\n";

close IDXFH;
