#!/bin/sh

#
# Usage:
#   $0 /path/to/config
#

source /etc/arch-backup/arch-backup.conf
source $LIBEXEC_DIR/common.inc

[ "x$1" == "x" ] && exit 1
[ -f "$1" ] || exit 2

source $1

SSH_COMMAND="ssh -l $SSH_USER -i $SSH_KEY $HOST -C"

#
# Save package list
#
echo -n "Saving package list..."
case "$PACKAGES" in
    pacman)
	$SSH_COMMAND "pacman -Q" 2>/dev/null >$BACKUP_DIR/$HOST-pkg-list.txt
	;;
    rpm)
	$SSH_COMMAND "rpm -qa" 2>/dev/null >$BACKUP_DIR/$HOST-pkg-list.txt
	;;
    dpkg)
	$SSH_COMMAND "dpkg -l" 2>/dev/null >$BACKUP_DIR/$HOST-pkg-list.txt
	;;
    *)
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
    homedir=`$SSH_COMMAND "cat /etc/passwd" 2>/dev/null | perl -ne "if(/^(.+):.*:.*:.*:.*:(.+):.*/ && \\$1 eq "$user"){print \\$2;}"`
    echo -n "Saving $user's home..."
    list=`$SSH_COMMAND "cat $homedir/.backup-list" 2>/dev/null | perl -ne "print \"'$homedir/\"; chomp; print; print \"' \";"`
    $SSH_COMMAND "tar c $list" 2>/dev/null | $COMPRESS_CMD >$BACKUP_DIR/$HOST-user-$user.tar.$COMPRESSED_EXT 2>/dev/null
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
    echo -n "Saving $dir..."
    $SSH_COMMAND "tar c $dir" 2>/dev/null | $COMPRESS_CMD >$BACKUP_DIR/$HOST-dir-`echo $dir | tr '/' '_'`.tar.$COMPRESSED_EXT 2>/dev/null
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
    echo -n "Saving command to file $FILE..."
    $SSH_COMMAND "$CMD" 2>/dev/null | $COMPRESS_CMD > $BACKUP_DIR/$HOST-cmd-$FILE.$COMPRESSED_EXT 2>/dev/null
    if [ $? -eq 0 ];then
        echo -e "${MSG_OK}"
    else
        echo -e "${MSG_ERROR}"
    fi
done
