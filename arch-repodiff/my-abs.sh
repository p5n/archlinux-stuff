#!/bin/sh

ABSROOT=~/arch-abs/
CVSUP=cvsup

############################################3

SEL=`(echo bold && echo setab 1) | tput -S`
NORM=`(echo op && echo sgr0) | tput -S`

#
# x86_64
#

echo "${SEL}ARCH: x86_64${NORM}"

# current
echo "${SEL}[current/x86_64]${NORM}"
mkdir -p $ABSROOT/x86_64/current
$CVSUP -L 1 -r 0 -g -b $ABSROOT/x86_64/current -P m -c .sup ./abs/supfile.arch-64

# extra
echo "${SEL}[extra/x86_64]${NORM}"
mkdir -p $ABSROOT/x86_64/extra
$CVSUP -L 1 -r 0 -g -b $ABSROOT/x86_64 -P m -c extra/.sup ./abs/supfile.extra-64

# community
echo "${SEL}[community/x86_64]${NORM}"
mkdir -p $ABSROOT/x86_64/community
$CVSUP -L 1 -r 0 -g -b $ABSROOT/x86_64 -P m -c community/.sup ./abs/supfile.community-64

#
# i686
#

echo "${SEL}ARCH: i686${NORM}"

# current
echo "${SEL}[current/i686]${NORM}"
mkdir -p $ABSROOT/i686/current
$CVSUP -L 1 -r 0 -g -b $ABSROOT/i686/current -P m -c .sup ./abs/supfile.arch

# extra
echo "${SEL}[extra/i686]${NORM}"
mkdir -p $ABSROOT/i686/extra
$CVSUP -L 1 -r 0 -g -b $ABSROOT/i686 -P m -c extra/.sup ./abs/supfile.extra

# community
echo "${SEL}[community/i686]${NORM}"
mkdir -p $ABSROOT/i686/community
$CVSUP -L 1 -r 0 -g -b $ABSROOT/i686 -P m -c community/.sup ./abs/supfile.community
