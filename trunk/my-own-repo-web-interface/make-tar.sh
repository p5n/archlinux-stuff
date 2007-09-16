#!/bin/sh

pkgver=0.0.3

sed -i "s|\$VERSION = \".*\";|\$VERSION = \"$pkgver\";|" my-own-repo-update.pl
sed -i "s|pkgver=.*\$|pkgver=$pkgver|" PKGBUILD

tar czf my-own-repo-web-interface-$pkgver.tar.gz *.pl *.pm *.php *.html

MD5SUM=`makepkg -g`

sed -i "s|md5sums=('.*')|$MD5SUM|" PKGBUILD
