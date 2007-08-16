#!/usr/bin/perl -w

$arch2 = "x86_64";

while(<>)
{
    chomp;
    $path = $_;

    if($path =~ /\/(.+)\/(.+)\/(.+)\/(.+)$/)
    {
        $arch = $1;
	$repo = $2;
        $group = $3;
	$pkg = $4;
    }
    else
    {
        die("bad param");
    }

    $p1 = `./pkg.sh $path/PKGBUILD`;
    $p2 = `./pkg.sh ./$arch2/$repo/$group/$pkg/PKGBUILD`;

    $p1 = "&nbsp;" if($p1 eq "");
    $p2 = "&nbsp;" if($p2 eq "");

    if($p1 ne $p2)
    {
        $diff = `diff2html -N $path/PKGBUILD ./$arch2/$repo/$group/$pkg/PKGBUILD`;

        if($diff)
        {
    	    print "<TR><TD>$group<TD><A HREF=\"$repo-$group-$pkg.html\">$pkg</A><TD>$p1<TD>$p2</TR>\n";
	    open FH, ">$repo-$group-$pkg.html";
    	    print FH $diff;
    	    close FH;
	}
    }
}
