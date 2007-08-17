#!/bin/sh

export DISPLAY=127.0.0.1:0.0
user=$1
cmd=$2
shift 2
exec su -c "$cmd $@" - $user
