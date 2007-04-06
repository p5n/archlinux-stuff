AVER=0.7.3
DIR=arch-backup-$(AVER)
DIRP=arch-backup

all: arch-backup.1 $(DIR).tar.gz $(DIRP).tar.gz

arch-backup.1:
	LANG= LC_ALL= txt2man -t arch-backup arch-backup.man.txt >arch-backup.1

$(DIR).tar.gz: arch-backup.1 \
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
	cp arch-backup.1 $(DIR)/
	tar czf $(DIR).tar.gz $(DIR)/
	rm -rf $(DIR)


$(DIRP).tar.gz: $(DIR).tar.gz
	mkdir $(DIRP)
	sed -i "s|md5sums=(.*$$|md5sums=('`md5sum $(DIR).tar.gz | cut -f 1 -d \ `')|g" PKGBUILD
	cp PKGBUILD $(DIRP)/
	tar czf $(DIRP).tar.gz $(DIRP)/
	rm -rf $(DIRP)

clean:
	rm -f *.tar.gz *.1
