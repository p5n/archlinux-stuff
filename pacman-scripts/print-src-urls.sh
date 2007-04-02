#!/bin/sh

DIRS_CURRENT="/var/abs/base \
    /var/abs/daemons \
    /var/abs/devel \
    /var/abs/editors \
    /var/abs/kernels \
    /var/abs/lib \
    /var/abs/multimedia \
    /var/abs/network \
    /var/abs/office \
    /var/abs/system \
    /var/abs/x11 \
    /var/abs/x11-drivers \
    /var/abs/x11-libs"

DIRS_EXTRA="/var/abs/extra"

DIRS_COMMUNITY="/var/abs/community"

case "x$1" in
    "xcurrent")
	DIRS=$DIRS_CURRENT
	;;
    "xextra")
	DIRS=$DIRS_EXTRA
	;;
    "xcommunity")
	DIRS=$DIRS_COMMUNITY
	;;
    "xall")
	DIRS="$DIRS_CURRENT $DIRS_EXTRA $DIRS_COMMUNITY"
	;;
    "x")
	echo "Usage:"
	echo "    $0 all|current|extra|community"
	exit 1
	;;
esac

find $DIRS -name PKGBUILD | while read A; do
    source $A
    for i in ${source[@]}; do
	echo $i | egrep "^http://|^ftp://|^https://"
    done
done
