#!/bin/sh

repo-add /srv/ftp/archlinux/sergej-repo/i686/sergej-repo.db.tar.gz /srv/ftp/archlinux/sergej-repo/i686/*.pkg.tar.gz
repo-add /srv/ftp/archlinux/sergej-repo/x86_64/sergej-repo.db.tar.gz /srv/ftp/archlinux/sergej-repo/x86_64/*.pkg.tar.gz

./bin/my-own-repo-update.pl pacman dbi:SQLite:/srv/ftp/archlinux/sergej-repo/sergej-repo-i686.db /srv/ftp/archlinux/sergej-repo/i686/
./bin/my-own-repo-update.pl pacman dbi:SQLite:/srv/ftp/archlinux/sergej-repo/sergej-repo-x86_64.db /srv/ftp/archlinux/sergej-repo/x86_64/
