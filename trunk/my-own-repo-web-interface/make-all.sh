#!/bin/sh

pkgver=0.4

sed -i "s|\$VERSION = \".*\";|\$VERSION = \"$pkgver\";|" bin/my-own-repo-update.pl
sed -i "s|pkgver=.*\$|pkgver=$pkgver|" PKGBUILD

tar czf my-own-repo-web-interface-$pkgver.tar.gz bin www

MD5SUM=`makepkg -g`

sed -i "s|md5sums=('.*')|$MD5SUM|" PKGBUILD

makepkg -c
