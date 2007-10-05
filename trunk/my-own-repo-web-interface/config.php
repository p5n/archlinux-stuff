<?php
    $repos[0]['db'] = "sqlite:/srv/my-own-repo/myrepo.db";
    $repos[0]['name'] = "myrepo(i686)";
    $repos[0]['rcspath'] = "/var/abs";
    $repos[0]['rcsurl'] = "http://cvs.archlinux.org/cgi-bin/viewcvs.cgi/%s";
    $repos[1]['db'] = "sqlite:/srv/my-own-repo/myrepo64.db";
    $repos[1]['name'] = "myrepo(x86_64)";
    $repos[1]['rcspath'] = "/var/abs";
    $repos[1]['rcsurl'] = "http://cvs.archlinux.org/cgi-bin/viewcvs.cgi/%s";

    $enableMarkOutOfDate = true;
    $enableMailNotification = true;
    $repoOwnerEmail = "root@localhost";
?>
