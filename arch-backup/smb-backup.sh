#!/bin/sh

#
# Usage:
#   $0 /path/to/config
#

source /etc/arch-backup.conf
source $LIBEXEC_DIR/common.inc

[ "x$1" == "x" ] && exit 1
[ -f "$1" ] || exit 2

source $1

#
# Save dirs
#
for dir in ${DIRS[@]}; do
    echo -n "Saving $dir..."

    share=`echo $dir | cut -d: -f1`
    dir=`echo $dir | cut -d: -f2`

    if [ "x$SMB_PASSWD" == "x" ]; then
        SMB_COMMAND="smbtar -s $HOST -u $SMB_USER -x $share -d $dir -t /proc/self/fd/1"
    else
	SMB_COMMAND="smbtar -s $HOST -u $SMB_USER -p \"$SMB_PASSWD\" -x $share -d $dir -t /proc/self/fd/1"
    fi

    $SMB_COMMAND | bzip2 >$BACKUP_DIR/$HOST-dir-$share-`echo $dir | tr '/' '_'`.tar.bz2 1>/dev/null 2>/dev/null
    if [ $? -eq 0 ];then
        echo -e "${MSG_OK}"
    else
        echo -e "${MSG_ERROR}"
    fi
done
