(based on http://search.cpan.org/dist/Net-XMPP2/ and its samples)

# Usage #

jabber-tool-roster-dump 

&lt;JID&gt;

 

&lt;password&gt;

 [

&lt;host&gt;

 

&lt;port&gt;

]

jabber-tool-roster-import 

&lt;JID&gt;

 

&lt;password&gt;

 [

&lt;host&gt;

 

&lt;port&gt;

]

# Examples #

## Move your roster to another server ##

```
jabber-tool-roster-dump user@domain1.org 123456 | \
        jabber-tool-roster-import user@domain2.org 654321
```

## Move ICQ contacts from one jabber transport to another ##

```
jabber-tool-roster-dump sergej@qwe.ru 123456 | \
        grep '@icq.qwe.ru' | \
        sed 's#icq.qwe.ru#icq.vrn.asd.ru#' | \
        jabber-tool-roster-import sergej@vrn.asd.ru 654321
```

# Source #

http://code.google.com/p/archlinux-stuff/source/browse/trunk/jabber-tools/