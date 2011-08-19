#!/bin/sh

dirs="/dev /proc /sys /home"

for i in $dirs; do
  if [ -d /emul32/$i ]; then
    echo $i exists
  else
    mkdir -p /emul32/$i
  fi
done

for i in $dirs; do
  echo Unmounting $i...
  umount /emul32/$i
done

for i in $dirs; do
  echo Mounting $i...
  mount --bind $i /emul32/$i
done
