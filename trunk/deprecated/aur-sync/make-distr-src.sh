#!/bin/sh

DIRP=aur-sync
mkdir $DIRP
cp PKGBUILD $DIRP/
tar czf $DIRP.tar.gz $DIRP/
rm -rf $DIRP
