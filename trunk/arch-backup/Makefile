DIR=arch-backup-0.7.2

all: src-tar aur-tar

man:
	LANG= LC_ALL= txt2man -t arch-backup arch-backup.man.txt >arch-backup.1

src-tar: man \
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


aur-tar:
	DIRP=arch-backup
	mkdir $(DIRP)
	cp PKGBUILD $(DIRP)/
	tar czf $(DIRP).tar.gz $(DIRP)/
	rm -rf $(DIRP)
