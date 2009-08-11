# Contributor: Andrea Scarpino <bash.lnx@gmail.com>

pkgname=css-docs
pkgver=2.1
pkgrel=1
pkgdesc="Set of HTML documentation for CSS."
arch=('any')
url="http://www.w3.org/TR/CSS21/"
license=('GPL')
options=('docs')
source=(http://www.w3.org/TR/2007/CR-CSS21-20070719/css2.tgz)
md5sums=('4dc0c96b49591199f75373afd2463226')

build(){
  cd "$srcdir"
  rm css2.tgz || return 1
  mkdir -p $startdir/pkg/usr/share/doc/css
  cp -rf * $startdir/pkg/usr/share/doc/css || return 1
}
