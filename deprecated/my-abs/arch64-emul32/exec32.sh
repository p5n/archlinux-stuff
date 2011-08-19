#!/bin/sh

user=`id -un`

sudo chroot /emul32 /start32.sh $user $@
