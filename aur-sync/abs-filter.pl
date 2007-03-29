#!/usr/bin/perl -w

while(<>)
{
    if(/\[DIR\]\s+\[\d+\](.*)\/\s+(\d{2}-\w{3}-\d{4}\s+\d{2}:\d{2})/)
    {
	print "[DIR] [1]$1/ $2\n";
    }
}
