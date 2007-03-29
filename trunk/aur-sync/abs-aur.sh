#!/bin/sh

[ -f packages.lst ] || touch packages.lst

mv packages.lst packages.lst.old

lynx -dump http://aur.archlinux.org/packages/ | ./abs-filter.pl >packages.lst

diff packages.lst packages.lst.old |grep "^<"|sed "s#<##" >packages-to-fetch.lst

cat packages-to-fetch.lst | ./abs-aur.pl aur-mirror
