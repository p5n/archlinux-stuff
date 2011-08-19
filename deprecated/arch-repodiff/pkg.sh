#!/bin/bash

export CARCH=i686

[ -f $1 ] && source $1

echo -n $pkgver-$pkgrel
