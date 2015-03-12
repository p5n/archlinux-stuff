# Installation #

Place php-files into web server dir, pm-files into /usr/lib/my-own-repo-web-interface/, my-own-repo-update.pl file into /usr/bin/my-own-repo-update.

# Usage #

Init db with:

my-own-repo-update --init pacman DBI:SQLite:/path/to/repo-sqlite.db /path/to/packages/dir

Place following command into cron:

my-own-repo-update pacman DBI:SQLite:/path/to/repo-sqlite.db /path/to/packages/dir

# Web interface configuration #
All you need is edit config.php to change list of repositories.

Examples:

  * Single repo and single arch:
```
    $repos[0]['db'] = "sqlite:/srv/my-own-repo/myrepo.db";
    $repos[0]['name'] = "myrepo(i686)";
```

  * Three repo
```
    $repos[0]['db'] = "sqlite:/srv/my-own-repo/myrepo1.db";
    $repos[0]['name'] = "myrepo1";
    $repos[1]['db'] = "sqlite:/srv/my-own-repo/myrepo2.db";
    $repos[1]['name'] = "myrepo2";
    $repos[2]['db'] = "sqlite:/srv/my-own-repo/myrepo3.db";
    $repos[2]['name'] = "myrepo3";
```

# Browsing #

Do
```
ln -s default-theme.css theme.css 
```

to set default theme as current one.

Now you can browse http://your-server.tld/path/to/search.php

# Screenshots #

  * Search page
![http://archlinux-stuff.googlecode.com/svn/wiki/my-own-repo-screen1.png](http://archlinux-stuff.googlecode.com/svn/wiki/my-own-repo-screen1.png)