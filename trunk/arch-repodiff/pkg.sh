#!/bin/bash

[ -f $1 ] && source $1

echo -n $pkgver-$pkgrel
