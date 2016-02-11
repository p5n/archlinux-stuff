#!/bin/sh

version=tzdata2016a

rm -rf data ${version}

[ -f ${version}.tar.gz ] || wget http://www.iana.org/time-zones/repository/releases/${version}.tar.gz
mkdir -p ${version}
(cd ${version} && tar xzf ../${version}.tar.gz)
mkdir data

timezones=('africa' 'antarctica' 'asia' 'australasia'
           'europe' 'northamerica' 'southamerica'
           'pacificnew' 'etcetera' 'backward'
           'systemv' 'factory')

for i in ${timezones[@]}; do
    zic -d data ${version}/$i
done

javac ZoneCompactor.java ZoneInfo.java

(
    cat $version/* | grep '^Link' | awk '{print $1, $2, $3}'
    (
        cat $version/* | grep '^Zone' | awk '{print $2}'
        cat $version/* | grep '^Link' | awk '{print $3}'
    ) | LC_ALL="C" sort
) | LC_ALL="C" sort | grep -v Hanoi > setup

java -classpath `pwd` ZoneCompactor setup data

echo $version | sed 's/tzdata//' > zoneinfo.version
