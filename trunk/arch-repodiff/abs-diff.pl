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

$title = "$repo diff ($arch1 vs $arch2) at ".`date +"%Y.%m.%d.%H.%M.%S(%Z)"`;

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
    if($repo ne "community" and /\/($template)$/i)
    {
        $group = ".";
	$pkg = $1;
	$PKGS{$group."/".$pkg} = 1;
    }
    elsif($repo eq "community" and /\/($template)\/($template)$/i)
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
    if($repo ne "community" and /\/($template)$/i)
    {
        $group = ".";
	$pkg = $1;
	$PKGS{$group."/".$pkg} = 1;
    }
    elsif($repo eq "community" and /\/($template)\/($template)$/i)
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
print IDXFH "<TR class=header><TH>#<TH>Package<TH>$arch1<TH>$arch2</TR>\n";

$counter = 0;
$count_hidden = 0;
$count_reldiff = 0;
$count_absent = 0;

mloop:
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

    if($pkg =~ /^lib32\-.+/)
    {
	next mloop;
    }

    $org_p1 = `arch-repodiff-pkginfo $basedir/$arch1/$repo/$group/$pkg/PKGBUILD`;
    $org_p2 = `arch-repodiff-pkginfo $basedir/$arch2/$repo/$group/$pkg/PKGBUILD`;

    $org_p1 = "&nbsp;" if($org_p1 eq "");
    $org_p2 = "&nbsp;" if($org_p2 eq "");

    if($org_p1 =~ /^(.*)-(.+)\..+$/)
    {
        $p1 = "$1-$2";
    }
    else
    {
	$p1 = $org_p1;
    }

    if($org_p2 =~ /^(.*)-(.+)\..+$/)
    {
        $p2 = "$1-$2";
    }
    else
    {
	$p2 = $org_p2;
    }

    if($p1 =~ /^(.+)-(.+)$/)
    {
	$v1 = $1;
    }

    if($p2 =~ /^(.+)-(.+)$/)
    {
	$v2 = $1;
    }

    if( ($org_p1 ne $org_p2) and ($p1 eq $p2) )
    {
	$count_hidden++;
    }

    if($p1 ne $p2)
    {
	print "DIFF: $repo-$pkg\n";

	if($v1 eq $v2)
	{
	    $count_reldiff++;
	}

	if( ($p1 ne "-") && ($p2 ne "-") )
	{
    	    system("diff2html -N $basedir/$arch1/$repo/$group/$pkg/PKGBUILD $basedir/$arch2/$repo/$group/$pkg/PKGBUILD >$destdir/$repo-$pkg.html");
	    $diffurl = "<A HREF=\"$repo-$pkg.html\">$pkg</A>";
	}
	else
	{
	    $diffurl = "$pkg";
	    $count_absent++;
	}
	$class = $counter % 2;
	if($p1 lt $p2)
	{
    	    print IDXFH "<TR class=row$class><TD>$counter<TD>$diffurl<TD class=outdated>$org_p1<TD>$org_p2</TR>\n";
	}
	else
	{
    	    print IDXFH "<TR class=row$class><TD>$counter<TD>$diffurl<TD>$org_p1<TD class=outdated>$org_p2</TR>\n";
	}
	$counter++;
    }
}

print IDXFH "</TABLE>\n";
print IDXFH "<P>Total: $counter\n";
print IDXFH "<P>Hidden (pkgrel differs in after dot part): $count_hidden\n";
print IDXFH "<P>pkgrel diffs: $count_reldiff\n";
print IDXFH "<P>Absent packages (present in one arch, but absent in other): $count_absent\n";

print IDXFH "</BODY></HTML>\n";

close IDXFH;
