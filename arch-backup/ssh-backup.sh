#!/bin/bash

#
# Usage:
#   $0 /path/to/config
#

source /etc/arch-backup/arch-backup.conf
source $LIBEXEC_DIR/common.inc

[ "x$1" == "x" ] && exit 1
[ -f "$1" ] || exit 2

source $1

SSH_COMMAND="ssh -x -l $SSH_USER -i $SSH_KEY $HOST -C"

[ "x$SSH_PORT" != "x" ] && SSH_COMMAND="$SSH_COMMAND -p $SSH_PORT"

#
# Save package list
#
echo -n ">>> Saving package list..."
case "$PACKAGES" in
    pacman)
	$SSH_COMMAND "pacman -Q" >$BACKUP_DIR/$HOST-pkg-list.txt
	;;
    rpm)
	$SSH_COMMAND "rpm -qa" >$BACKUP_DIR/$HOST-pkg-list.txt
	;;
    dpkg)
	$SSH_COMMAND "dpkg -l" >$BACKUP_DIR/$HOST-pkg-list.txt
	;;
    none)
	echo "have no package manager"
	;;
    *)
	echo "unknown package manager type"
	;;
esac
if [ $? -eq 0 ];then
    echo -e "${MSG_OK}"
else
    echo -e "${MSG_ERROR}"
fi

#
# Save users homes according to ~/.backup-list
#
for user in ${USERS[@]}; do
    homedir=`$SSH_COMMAND "cat /etc/passwd" | perl -ne "if(/^(.+):.*:.*:.*:.*:(.+):.*/ && \\$1 eq "$user"){print \\$2;}"`
    echo -n ">>> Saving $user's home..."
    list=`$SSH_COMMAND "cat $homedir/.backup-list" | perl -ne "print \"'${homedir:1}/\"; chomp; print; print \"' \";"`
    $SSH_COMMAND "cd / && tar c $list | $COMPRESS_CMD" >$BACKUP_DIR/$HOST-user-$user.tar.$COMPRESSED_EXT
    if [ $? -eq 0 ];then
        echo -e "${MSG_OK}"
    else
        echo -e "${MSG_ERROR}"
    fi
done

#
# Save dirs
#
for dir in ${DIRS[@]}; do
    echo -n ">>> Saving $dir..."
    $SSH_COMMAND "cd / && tar c ${dir:1} | $COMPRESS_CMD" >$BACKUP_DIR/$HOST-dir-`echo $dir | tr '/' '_'`.tar.$COMPRESSED_EXT
    if [ $? -eq 0 ];then
        echo -e "${MSG_OK}"
    else
        echo -e "${MSG_ERROR}"
    fi
done

#
# Save commands out
#
i=0
while [ "x${COMMANDS[$i]}" != "x" ]; do
    FILE=${COMMANDS[$i]}
    i=`expr $i + 1`
    CMD=${COMMANDS[$i]}
    i=`expr $i + 1`
    echo -n ">>> Saving command to file $FILE..."
    $SSH_COMMAND "cd / && $CMD | $COMPRESS_CMD" >$BACKUP_DIR/$HOST-cmd-$FILE.$COMPRESSED_EXT
    if [ $? -eq 0 ];then
        echo -e "${MSG_OK}"
    else
        echo -e "${MSG_ERROR}"
    fi
done
