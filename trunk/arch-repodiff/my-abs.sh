#!/bin/bash

if [ "x$1" == "x" ]; then
    echo "Usage: $0 <basedir>"
    exit 1
fi

ABSROOT=$1
[ -f /usr/bin/cvsup ] && CVSUP=/usr/bin/cvsup
[ -f /usr/bin/csup  ] && CVSUP=/usr/bin/csup
supdir=/etc/arch-repodiff

############################################3

SEL=`(echo bold && echo setab 1) | tput -S`
NORM=`(echo op && echo sgr0) | tput -S`

#
# x86_64
#

echo "${SEL}ARCH: x86_64${NORM}"

# core
echo "${SEL}[core/x86_64]${NORM}"
mkdir -p $ABSROOT/x86_64/core
$CVSUP -L 1 -r 0 -g -b $ABSROOT/x86_64 -P m -c .sup $supdir/supfile.core-64

# extra
echo "${SEL}[extra/x86_64]${NORM}"
mkdir -p $ABSROOT/x86_64/extra
$CVSUP -L 1 -r 0 -g -b $ABSROOT/x86_64 -P m -c extra/.sup $supdir/supfile.extra-64

# community
echo "${SEL}[community/x86_64]${NORM}"
mkdir -p $ABSROOT/x86_64/community
$CVSUP -L 1 -r 0 -g -b $ABSROOT/x86_64 -P m -c community/.sup $supdir/supfile.community-64

#
# i686
#

echo "${SEL}ARCH: i686${NORM}"

# core
echo "${SEL}[core/i686]${NORM}"
mkdir -p $ABSROOT/i686/core
$CVSUP -L 1 -r 0 -g -b $ABSROOT/i686 -P m -c .sup $supdir/supfile.core

# extra
echo "${SEL}[extra/i686]${NORM}"
mkdir -p $ABSROOT/i686/extra
$CVSUP -L 1 -r 0 -g -b $ABSROOT/i686 -P m -c extra/.sup $supdir/supfile.extra

# community
echo "${SEL}[community/i686]${NORM}"
mkdir -p $ABSROOT/i686/community
$CVSUP -L 1 -r 0 -g -b $ABSROOT/i686 -P m -c community/.sup $supdir/supfile.community
