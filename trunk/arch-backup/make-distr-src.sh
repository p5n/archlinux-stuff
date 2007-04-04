#!/bin/sh

DIR=arch-backup-0.7.2

mkdir $DIR

cp arch-backup.conf $DIR/
cp arch-backup.sh $DIR/
cp common.inc $DIR/
cp local-backup.sh $DIR/
cp local-example.conf $DIR/
cp smb-backup.sh $DIR/
cp smb-example.conf $DIR/
cp ssh-backup.sh $DIR/
cp ssh-example.conf $DIR/

tar czf $DIR.tar.gz $DIR/

rm -rf $DIR

#
#
#

DIRP=arch-backup
mkdir $DIRP
cp PKGBUILD $DIRP/
tar czf $DIRP.tar.gz $DIRP/
rm -rf $DIRP