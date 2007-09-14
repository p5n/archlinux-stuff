#!/bin/sh

pkgver=0.0.1

sed -i "s|\$VERSION = \".*\";|\$VERSION = \"$pkgver\";|" my-own-repo-update.pl

tar czf my-own-repo-web-interface-$pkgver.tar.gz *.pl *.pm *.php
