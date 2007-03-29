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
        sh -c "tar cjf $BACKUP_DIR/_localhost-user-$user.tar.bz2 $list" 1>/dev/null 2>/dev/null
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
    tar cjf $BACKUP_DIR/_localhost-dir-`echo $dir | tr '/' '_'`.tar.bz2 $dir 1>/dev/null 2>/dev/null
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
    $SSH_COMMAND "$CMD" 2>/dev/null | bzip2 > $BACKUP_DIR/_localhost-cmd-$FILE.bz2 2>/dev/null
    if [ $? -eq 0 ];then
        echo -e "${MSG_OK}"
    else
        echo -e "${MSG_ERROR}"
    fi
done
