AVER=0.5

all: arch-xdg-menu-$(AVER).tar.gz

arch-xdg-menu-$(AVER).tar.gz:
	tar czf arch-xdg-menu-$(AVER).tar.gz arch-desktop-directories arch-xdg-menu xdg_menu xdg_menu_su update-menus update-menus.conf
	sed -i "s|pkgver=.*$$|pkgver=$(AVER)|g" PKGBUILD
	sed -i "s|md5sums=(.*$$|md5sums=('`md5sum arch-xdg-menu-$(AVER).tar.gz | cut -f 1 -d \ `')|g" PKGBUILD

clean:
	rm -f *.tar.gz
