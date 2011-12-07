#!/bin/bash

#
# Usage:
#   $0 [ [-c] | [config-name1 [config-name2 [...]]] ]
# -c - check ~/.backup-list
# config-name - filename from /etc/arch-backup
# if no config-name given, backups all configurations from
# $BACKUP in /etc/arch-backup/arch-backup.conf
#

#
# check ~/.backup-list
#
if [ "x$1" == "x-c" ];then
    echo "List of missing files/dirs:"
    cat ~/.backup-list| while read A; do [ -f ~/$A ] || [ -d ~/$A ] || echo $A; done
    echo "end of list"
    exit 0
fi

#

source /etc/arch-backup/arch-backup.conf
source $LIBEXEC_DIR/common.inc

DATE=`date +%Y.%m.%d.%H.%M.%S`

[ -z "$BACKUP_OWNER" ] && BACKUP_OWNER=root:root
[ -z "$BACKUP_PERMS_DIR" ] && BACKUP_PERMS_DIR=0700
[ -z "$BACKUP_PERMS_FILE" ] && BACKUP_PERMS_FILE=0600

#
# Backup
#
if [ "x$1" == "x" ]; then
    for config in ${BACKUP[@]}; do
	if [ "$config" != "arch-backup.conf" ]; then
	    echo "${C_SEL}BACKUP $config${C_NORM}"
	    # clean previous vars
	    unset TYPE USERS PACKAGES DIRS AFTER_BACKUP BEFORE_BACKUP COMMANDS HOST SMB_USER SMB_PASSWD SSH_PORT SSH_USER SSH_KEY
	    export BACKUP_DIR=$BACKUP_BASE_DIR/arch-backup-$DATE-$BACKUP_SUFFIX

	    source $CONFIG_DIR/$config

	    mkdir -p $BACKUP_DIR
	    chown $BACKUP_OWNER $BACKUP_DIR
	    chmod $BACKUP_PERMS_DIR $BACKUP_DIR

	    [ "x$BEFORE_BACKUP" == "x" ] && BEFORE_BACKUP=true
	    [ "x$AFTER_BACKUP" == "x" ] && AFTER_BACKUP=true

	    echo -n "Executing \"$BEFORE_BACKUP\" before backup..."
	    eval $BEFORE_BACKUP
	    if [ $? -eq 0 ]; then
		echo -e "${MSG_OK}"
		$LIBEXEC_DIR/$TYPE $CONFIG_DIR/$config
	    else
		echo -e "${MSG_ERROR}"
	    fi
	    echo -n "Executing \"$AFTER_BACKUP\" after backup..."
	    eval $AFTER_BACKUP && echo -e "${MSG_OK}" || echo -e "${MSG_ERROR}"

	    chown $BACKUP_OWNER $BACKUP_DIR/*
	    chmod $BACKUP_PERMS_FILE $BACKUP_DIR/*

	fi
    done
else
    while [ "x$1" != "x" ]; do
	if [ "$config" != "arch-backup.conf" ]; then
	    echo "${C_SEL}BACKUP $1${C_NORM}"
	    unset TYPE USERS PACKAGES DIRS AFTER_BACKUP BEFORE_BACKUP COMMANDS HOST SMB_USER SMB_PASSWD SSH_PORT SSH_USER SSH_KEY
	    export BACKUP_DIR=$BACKUP_BASE_DIR/arch-backup-$DATE-$BACKUP_SUFFIX

	    source $CONFIG_DIR/$1

	    mkdir -p $BACKUP_DIR
	    chown $BACKUP_OWNER $BACKUP_DIR
	    chmod $BACKUP_PERMS_DIR $BACKUP_DIR

	    [ "x$BEFORE_BACKUP" == "x" ] && BEFORE_BACKUP=true
	    [ "x$AFTER_BACKUP" == "x" ] && AFTER_BACKUP=true

	    echo -n "Executing \"$BEFORE_BACKUP\" before backup..."
	    eval $BEFORE_BACKUP
	    if [ $? -eq 0 ]; then
		echo -e "${MSG_OK}"
		$LIBEXEC_DIR/$TYPE $CONFIG_DIR/$1
	    else
		echo -e "${MSG_ERROR}"
	    fi
	    echo -n "Executing \"$AFTER_BACKUP\" after backup..."
	    eval $AFTER_BACKUP && echo -e "${MSG_OK}" || echo -e "${MSG_ERROR}"

	    chown $BACKUP_OWNER $BACKUP_DIR/*
	    chmod $BACKUP_PERMS_FILE $BACKUP_DIR/*

	    shift 1
	fi
    done
fi

#
# Remove old backup dirs
#

echo "${C_SEL}Cleanup${C_NORM}"

cd $BACKUP_BASE_DIR && cleardir $NUM_BACKUPS
