#!/usr/bin/perl -w

$f = 'qwe
asd';

print $f;

$f =~ s/\n/\\n/g;

print $f;
