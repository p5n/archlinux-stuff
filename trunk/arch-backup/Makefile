AVER=0.8.10
DIR=arch-backup-$(AVER)

all: $(DIR).tar.gz

arch-backup.8:
	LANG= LC_ALL= txt2man -r $(AVER) -t arch-backup -s 8 arch-backup.man.txt >arch-backup.8

$(DIR).tar.gz: arch-backup.8 \
 arch-backup.conf arch-backup.sh \
 common.inc \
 local-backup.sh local-example.conf \
 smb-backup.sh smb-example.conf \
 ssh-backup.sh ssh-example.conf
	mkdir $(DIR)
	cp arch-backup.conf $(DIR)/
	cp arch-backup.sh $(DIR)/
	cp common.inc $(DIR)/
	cp local-backup.sh $(DIR)/
	cp local-example.conf $(DIR)/
	cp smb-backup.sh $(DIR)/
	cp smb-example.conf $(DIR)/
	cp ssh-backup.sh $(DIR)/
	cp ssh-example.conf $(DIR)/
	cp arch-backup.8 $(DIR)/
	tar czf $(DIR).tar.gz $(DIR)/
	rm -rf $(DIR)
	sed -i "s|md5sums=(.*$$|md5sums=('`md5sum $(DIR).tar.gz | cut -f 1 -d \ `')|g" PKGBUILD
	sed -i "s|pkgver=.*$$|pkgver=$(AVER)|g" PKGBUILD

clean:
	rm -f *.tar.gz *.8
