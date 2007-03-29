#!/usr/bin/perl -w

$arg=pop @ARGV;

print "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\" \"http://www.w3.org/TR/REC-html40/loose.dtd\">\n";
print "<HTML>\n";
print "<HEAD>\n";
print "  <TITLE>echat - $arg</TITLE>\n";
print "  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=koi8-r\">\n";
print "  <link rel=\"stylesheet\" type=\"text/css\" href=\"echat.css\" media=\"screen\">\n";
print "</HEAD>\n";
print "<BODY>\n";
print "<H4>echat - $arg</H4>\n";

while(<>)
{
	s/\</&lt;/g;
	s/\>/&gt;/g;
	s/&/&amp;/g;

    if(/\[(\d{2}-\d{2}-\d{4})\s+(\d{2}:\d{2}:\d{2})\]\s+(.+)\s+enter\s+(.+)\s+mode\./)
    {
        print "<SPAN class=mode>[<SPAN class=date>$1 $2</SPAN>] <SPAN class=name>$3</SPAN> enter mode $4</SPAN><BR>\n";
    }
    elsif(/\[(\d{2}-\d{2}-\d{4})\s+(\d{2}:\d{2}:\d{2})\]\s+(.+)\s+enter\s+(.+)\s+mode:/)
    {
        print "<SPAN class=mode>[<SPAN class=date>$1 $2</SPAN>] <SPAN class=name>$3</SPAN> enter mode $4: ";
        $_ = <>;
		s/\</&lt;/g;
		s/\>/&gt;/g;
		s/&/&amp;/g;
        chomp;
        print "$_</SPAN><BR>\n";
    }
    elsif(/\[(\d{2}-\d{2}-\d{4})\s+(\d{2}:\d{2}:\d{2})\]\s+(.+)\s+join channel\s+(.+)./)
    {
        print "<SPAN class=join>[<SPAN class=date>$1 $2</SPAN>] <SPAN class=name>$3</SPAN> join channel <SPAN class=chnl>$4</SPAN></SPAN><BR>\n";
    }
    elsif(/\[(\d{2}-\d{2}-\d{4})\s+(\d{2}:\d{2}:\d{2})\]\s+(.+)\s+leave channel\s+(.+)./)
    {
        print "<SPAN class=leave>[<SPAN class=date>$1 $2</SPAN>] <SPAN class=name>$3</SPAN> leave channel <SPAN class=chnl>$4</SPAN></SPAN><BR>\n";
    }
    elsif(/\[(\d{2}-\d{2}-\d{4})\s+(\d{2}:\d{2}:\d{2})\]\s+mass message from\s+(.+):/)
    {
        print "<SPAN class=mass>[<SPAN class=date>$1 $2</SPAN>] mass message from <SPAN class=name>$3</SPAN>: ";
        $_ = <>;
		s/\</&lt;/g;
		s/\>/&gt;/g;
		s/&/&amp;/g;
        chomp;
        print "$_</SPAN><BR>\n";
    }
    elsif(/\[(\d{2}-\d{2}-\d{4})\s+(\d{2}:\d{2}:\d{2})\]\s+(.+)\s+change nick to\s+(.+)\./)
    {
        print "<SPAN class=nick>[<SPAN class=date>$1 $2</SPAN>] <SPAN class=name>$3</SPAN> change nick to <SPAN class=name>$4</SPAN></SPAN><BR>\n";
    }
    elsif(/\[(\d{2}-\d{2}-\d{4})\s+(\d{2}:\d{2}:\d{2})\]\s+new topic on\s+(.+)/)
    {
        print "<SPAN class=topic>[<SPAN class=date>$1 $2</SPAN>] new topic on <SPAN class=chnl>$3</SPAN>: ";
        $_ = <>;
		s/\</&lt;/g;
		s/\>/&gt;/g;
		s/&/&amp;/g;
        chomp;
        print "$_</SPAN><BR>\n";
    }
    elsif(/^\[(\d{2}-\d{2}-\d{4})\s+(\d{2}:\d{2}:\d{2})\]\s+&lt;(.+)&gt;\s+(.+)$/)
    {
        print "<SPAN class=chat>[<SPAN class=date>$1 $2</SPAN>] &lt;<SPAN class=name>$3</SPAN>&gt; $4</SPAN><BR>\n";
    }
    elsif(/^&lt;(.+)&gt;s+(.+)$/)
    {
        print "<SPAN class=chat>&lt;<SPAN class=name>$1</SPAN>&gt; $2</SPAN><BR>\n";
    }
    else
    {
#        print "UNKNOWN: $_";
    }
}

print "</BODY>\n";
print "</HTML>\n";
