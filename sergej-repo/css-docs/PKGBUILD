# Contributor: Andrea Scarpino <bash.lnx@gmail.com>

pkgname=css-docs
pkgver=20090908
pkgrel=1
pkgdesc="Set of HTML documentation for CSS."
arch=('any')
url="http://www.w3.org/TR/CSS21/"
license=('GPL')
options=('docs')
source=(http://www.w3.org/TR/2009/CR-CSS2-$pkgver/css2.tgz)
md5sums=('48d9d41f5664b4f64a37900899ae7572')

build(){
  cd "$srcdir"
  rm css2.tgz
  mkdir -p $startdir/pkg/usr/share/doc/css
  cp -rf * $startdir/pkg/usr/share/doc/css
}
