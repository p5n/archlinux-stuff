#!/bin/bash

[ -f $1 ] && source $1

export CARCH=i686

echo -n $pkgver-$pkgrel
