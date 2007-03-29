#!/bin/sh

#
# Usage:
#   $0 [-p|-P] [config-name1 [config-name2 [...]]]
# -p - backup package list (only for localhost now)
# -P - backup package list and exit (only for localhost now)
# config-name - filename from /etc/arch-backup.d
# if no config-name given, backups all configurations from
# $BACKUP in /etc/arch-backup.conf
#

source /etc/arch-backup.conf
source $LIBEXEC_DIR/common.inc

DATE=`date +%Y.%m.%d.%H.%M.%S`
export BACKUP_DIR=$BACKUP_BASE_DIR/arch-backup-$DATE-$BACKUP_SUFFIX

mkdir -p $BACKUP_DIR

#
# Backup
#
if [ "x$1" == "x" ]; then
    for config in ${BACKUP[@]}; do
	echo "BACKUP $config"
	source $CONFIG_DIR/$config

	[ "x$BEFORE_BACKUP" == "x" ] && BEFORE_BACKUP=true
	[ "x$AFTER_BACKUP" == "x" ] && AFTER_BACKUP=true

	echo -n "Executing \"$BEFORE_BACKUP\" before backup:"
	$BEFORE_BACKUP
	if [ $? -eq 0 ]; then
	    echo -e "${MSG_OK}"
	    $LIBEXEC_DIR/$TYPE $CONFIG_DIR/$config
	else
	    echo -e "${MSG_ERROR}"
	fi
	echo -n "Executing \"$AFTER_BACKUP\" after backup:"
        $AFTER_BACKUP && echo -e "${MSG_OK}" || echo -e "${MSG_ERROR}"
    done
else
    while [ "x$1" != "x" ]; do
	echo "BACKUP $1"
        source $CONFIG_DIR/$1

	[ "x$BEFORE_BACKUP" == "x" ] && BEFORE_BACKUP=true
	[ "x$AFTER_BACKUP" == "x" ] && AFTER_BACKUP=true

	echo -n "Executing \"$BEFORE_BACKUP\" before backup:"
	$BEFORE_BACKUP
	if [ $? -eq 0 ]; then
	    echo -e "${MSG_OK}"
	    $LIBEXEC_DIR/$TYPE $CONFIG_DIR/$1
	else
	    echo -e "${MSG_ERROR}"
	fi
	echo -n "Executing \"$AFTER_BACKUP\" after backup:"
        $AFTER_BACKUP && echo -e "${MSG_OK}" || echo -e "${MSG_ERROR}"

	shift 1
    done
fi

#
# Remove old backup dirs
#

NUM_BACKUPS=`expr $NUM_BACKUPS + 1`

cd $BACKUP_BASE_DIR && (ls -1t | tail -n+$NUM_BACKUPS | while read A; do
    echo "Removing $A..."
    rm -rf $A
done)
