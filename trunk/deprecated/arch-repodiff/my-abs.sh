#!/bin/bash

if [ "x$1" == "x" ]; then
    echo "Usage: $0 <basedir>"
    exit 1
fi

ABSROOT=$1

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
rsync -mrtv --no-motd --delete-after --delete-excluded rsync.archlinux.org::abs/x86_64/core $ABSROOT/x86_64

# extra
echo "${SEL}[extra/x86_64]${NORM}"
mkdir -p $ABSROOT/x86_64/extra
rsync -mrtv --no-motd --delete-after --delete-excluded rsync.archlinux.org::abs/x86_64/extra $ABSROOT/x86_64

# community
echo "${SEL}[community/x86_64]${NORM}"
mkdir -p $ABSROOT/x86_64/community
rsync -mrtv --no-motd --delete-after --delete-excluded rsync.archlinux.org::abs/x86_64/community $ABSROOT/x86_64

#
# i686
#

echo "${SEL}ARCH: i686${NORM}"

# core
echo "${SEL}[core/i686]${NORM}"
mkdir -p $ABSROOT/i686/core
rsync -mrtv --no-motd --delete-after --delete-excluded rsync.archlinux.org::abs/i686/core $ABSROOT/i686

# extra
echo "${SEL}[extra/i686]${NORM}"
mkdir -p $ABSROOT/i686/extra
rsync -mrtv --no-motd --delete-after --delete-excluded rsync.archlinux.org::abs/i686/extra $ABSROOT/i686

# community
echo "${SEL}[community/i686]${NORM}"
mkdir -p $ABSROOT/i686/community
rsync -mrtv --no-motd --delete-after --delete-excluded rsync.archlinux.org::abs/i686/community $ABSROOT/i686
