#!/bin/sh

version=tzdata2016a

rm -rf data $version
rm -f setup $version.tar.gz *.class zoneinfo.{dat,idx,version}
