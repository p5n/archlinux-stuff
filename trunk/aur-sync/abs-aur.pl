#!/usr/bin/perl -w

$dir = pop @ARGV;

open(ERR, ">err.log");

chdir($dir);

mkdir("old");

while(<>)
{
    if(/\[DIR\]\s+\[\d+\](.*)\/\s+(\d{2}-\w{3}-\d{4}\s+\d{2}:\d{2})/)
    {
	print "$1, '$2'\n";
	if(system("mv -f $1.tar.gz ./old/; wget http://aur.archlinux.org/packages/$1/$1.tar.gz"))
	{
	    if(system("mv -f $1.tar.gz ./old/; wget http://aur.archlinux.org/packages/$1/PKGBUILD.tar.gz"))
	    {
		# TODO: try other names or get all tar.gz...
		print ERR "$1 failed\n";
		print STDERR "$1 failed\n";
	    }
	    else
	    {
		if(system("mv PKGBUILD.tar.gz $1.tar.gz"))
		{
		    print ERR "$1 move failed\n";
		    print STDERR "$1 move failed\n";
		}
	    }
	}
    }
}

close(ERR);
