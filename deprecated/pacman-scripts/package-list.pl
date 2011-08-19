#!/usr/bin/perl -w

#
# Outputs installed packages list in dependency order and in following format:
#   packagename version-revision
#
# For example:
#   kernel-headers 2.6.12.0-1
#   glibc 2.4-2
#   attr 2.4.32-2
#   acl 2.2.39-1
#   ncurses 5.5-1
#   readline 5.1.4-1
#   bash 3.1.17-1
#

%DEPS = ();
%PROVS = ();
@PKGS = ();
%VERS = ();
%FLAGS=();
%LINKS=();
%PRESENTS=();
%PRESENT_LINK=();
@stack = ();

#
#
#

sub get_pkg_deps
{
    my ($deps, $info, @ret, $i);
    $info = pop @_;
    $info =~ tr/\n//d;
    @ret = ();
    if($info =~ /Depends On\s+:\s+(.+)Required By/)
    {
	$deps = $1;
    }
    if($deps ne "None")
    {
	@ret = split /\s+/, $deps;
    }
    for($i=0; $i<=$#ret; $i++)
    {
	if($ret[$i] =~ /^\s*(\S+)>=.*$/ || $ret[$i] =~ /^\s*(\S+)=.*$/)
	{
	    $ret[$i] = $1;
	}
    }
    return @ret;
}

sub get_pkg_provs
{
    my ($provs, $info, @ret, $i);
    $info = pop @_;
    $info =~ tr/\n//d;
    @ret = ();
    if($info =~ /Provides\s+:\s+(.+)Depends On/)
    {
	$prov = $1;
    }
    if($prov ne "None")
    {
	@ret = split /\s+/, $prov;
    }
    for($i=0; $i<=$#ret; $i++)
    {
	if($ret[$i] =~ /^\s*(\S+)\s*$/)
	{
	    $ret[$i] = $1;
	}
    }
    return @ret;
}

sub get_pkg_ver
{
    my ($ver, $info, $ret);
    $info = pop @_;
    $info =~ tr/\n//d;
    $ret = "";
    if($info =~ /Version\s+:\s+(.+)Groups/)
    {
	$ver = $1;
    }
    $ret = $ver;
    return $ret;
}

sub get_pkgs
{
    my @ret;
    @ret = ();
    open FH, "pacman -Q|";
    while(<FH>)
    {
	if(/^(\S+)\s+(\S+)$/)
	{
	    push @ret, $1;
	}
    }
    close FH;
    return @ret;
}

sub install_pkg
{
    my $pkgname;
    $pkgname = pop @_;

    if($pkgname =~ /^(\S+)[<>=].*$/)
    {
	$pkgname = $1;
    }

    if(!defined $PRESENTS{$pkgname} && !defined $PRESENT_LINK{$pkgname})
    {
	print STDERR "ERROR: $pkgname not found\n";
	return;
    }

    foreach $i (@stack)
    {
	if($pkgname eq $i)
	{
	    print STDERR "ERROR: dependency loop detected ($pkgname), deps dump: ";
	    foreach $j (@stack)
	    {
		print STDERR "$j ";
	    }
	    print STDERR "\n";
	    return;
	}
    }
    push @stack, $pkgname;

    if(!defined $PRESENTS{$pkgname} || $PRESENTS{$pkgname} != 1)
    {
        if($FLAGS{$LINKS{$pkgname}} == 1)
	{
	    pop @stack;
	    return;
        }
        foreach $dep (@{$DEPS{$LINKS{$pkgname}}})
        {
	    install_pkg($dep);
        }
        $FLAGS{$LINKS{$pkgname}} = 1;
	print "$pkgname $VERS{$LINKS{$pkgname}}\n";
    }
    else
    {
        if($FLAGS{$pkgname} == 1)
	{
	    pop @stack;
	    return;
        }
        foreach $dep (@{$DEPS{$pkgname}})
        {
	    install_pkg($dep);
        }
        $FLAGS{$pkgname} = 1;
	print "$pkgname $VERS{$pkgname}\n";
    }
    pop @stack;
}
#
# #############################################################
#

@PKGS = get_pkgs;

print STDERR "Processing packages:";

foreach $pkgname (@PKGS)
{
    $q = `pacman -Qi $pkgname`;
    @{$DEPS{$pkgname}} = get_pkg_deps($q);
    @{$PROVS{$pkgname}} = get_pkg_provs($q);
    $VERS{$pkgname} = get_pkg_ver($q);
    $FLAGS{$pkgname} = 0;
    foreach $link (@{$PROVS{$pkgname}})
    {
	$LINKS{$link} = $pkgname;
	$PRESENT_LINK{$link} = 1;
    }
    $PRESENTS{$pkgname} = 1;
    print STDERR substr($pkgname, 0, 1);
}

print STDERR "\n";

foreach $pkgname (@PKGS)
{
    install_pkg($pkgname);
}
