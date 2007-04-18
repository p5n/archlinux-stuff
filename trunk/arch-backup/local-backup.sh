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

#
# Save package list
#
echo -n "Saving package list..."
case "$PACKAGES" in
    pacman)
	pacman -Q >$BACKUP_DIR/_localhost-pkg-list.txt
	;;
    rpm)
	rpm -qa >$BACKUP_DIR/_localhost-pkg-list.txt
	;;
    dpkg)
	dpkg -l >$BACKUP_DIR/_localhost-pkg-list.txt
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
    homedir=`cat /etc/passwd | perl -ne "if(/^(\S+):\S*:\S*:\S*:\S*:(\S+):\S*/ && \\$1 eq "$user"){print \\$2;}"`
    if [ -f $homedir/.backup-list ]; then
	echo -n "Saving $user's home..."
	list=`cat $homedir/.backup-list | perl -ne "print \"'$homedir/\"; chomp; print; print \"' \";"`
        sh -c "tar c $list" 2>/dev/null | $COMPRESS_CMD 1>$BACKUP_DIR/_localhost-user-$user.tar.$COMPRESSED_EXT 2>/dev/null
	if [ $? -eq 0 ];then
            echo -e "${MSG_OK}"
	else
    	    echo -e "${MSG_ERROR}"
	fi
    else
	echo "Saving $user's home: .backup-list is absent"
    fi
done

#
# Save dirs
#
for dir in ${DIRS[@]}; do
    echo -n "Saving $dir..."
    tar c $dir 2>/dev/null | $COMPRESS_CMD 1>$BACKUP_DIR/_localhost-dir-`echo $dir | tr '/' '_'`.tar.$COMPRESSED_EXT 2>/dev/null
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
    $CMD 2>/dev/null | $COMPRESS_CMD > $BACKUP_DIR/_localhost-cmd-$FILE.$COMPRESSED_EXT 2>/dev/null
    if [ $? -eq 0 ];then
        echo -e "${MSG_OK}"
    else
        echo -e "${MSG_ERROR}"
    fi
done
