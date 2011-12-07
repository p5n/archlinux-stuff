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

#
# Save package list
#
echo -n ">>> Saving package list..."
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
    homedir=`cat /etc/passwd | perl -ne "if(/^(\S+):.*:.*:.*:.*:(.+):.*/ && \\$1 eq "$user"){print \\$2;}"`
    if [ -f $homedir/.backup-list ]; then
	# check files
	echo -n ">>> Checking $user's home:"
	cat $homedir/.backup-list | while read A; do
	    if [ -d $homedir/$A -o -f $homedir/$A ]; then
		echo -n -e "${C_DONE}$A${C_CLEAR} "
	    else
		echo -n -e "${C_FAIL}$A${C_CLEAR} "
	    fi
	done
	echo
	# arc
	echo -n ">>> Saving $user's home..."
	list=`cat $homedir/.backup-list | perl -ne "print \"'${homedir:1}/\"; chomp; print; print \"' \";"`
	sh -c "cd / && tar c $list" | $COMPRESS_CMD 1>$BACKUP_DIR/_localhost-user-$user.tar.$COMPRESSED_EXT
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
    echo -n ">>> Saving $dir..."
    [ -d $dir -o -f $dir ] && cd / && tar c ${dir:1} | $COMPRESS_CMD 1>$BACKUP_DIR/_localhost-dir-`echo $dir | tr '/' '_'`.tar.$COMPRESSED_EXT
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
    sh -c "cd / && $CMD" | $COMPRESS_CMD > $BACKUP_DIR/_localhost-cmd-$FILE.$COMPRESSED_EXT
    if [ $? -eq 0 ];then
        echo -e "${MSG_OK}"
    else
        echo -e "${MSG_ERROR}"
    fi
done
