AVER=0.7.4
DIR=arch-backup-$(AVER)
DIRP=arch-backup

all: arch-backup.8 $(DIR).tar.gz $(DIRP).tar.gz

arch-backup.8:
	LANG= LC_ALL= txt2man -r 0.7.3 -t arch-backup -s 8 arch-backup.man.txt >arch-backup.8

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


$(DIRP).tar.gz: $(DIR).tar.gz
	mkdir $(DIRP)
	sed -i "s|md5sums=(.*$$|md5sums=('`md5sum $(DIR).tar.gz | cut -f 1 -d \ `')|g" PKGBUILD
	cp PKGBUILD $(DIRP)/
	tar czf $(DIRP).tar.gz $(DIRP)/
	rm -rf $(DIRP)

clean:
	rm -f *.tar.gz *.8
