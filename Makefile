CFLAGS="-Wall"

bin/app: package/src/app.c bin/display.o
	$(CC) $(CFLAGS) $< bin/display.o -o $@

bin/%.o: package/src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

app:	bin/app

image:
	. ./poky-krogoth/oe-init-build-env bbb/build/ && bitbake -f -c fetch trebletrouble && bitbake -f -c compile trebletrouble && bitbake trebletrouble-image

install-srv:
	./install-srv.sh

install-bin:
	./intall-bin.sh

sudo-format-sdcard:
	sudo ./bbb/meta-bbb/scripts/mk2parts.sh sdb

install-sdcard:
	OETMP=bbb/build/tmp ./bbb/meta-bbb/scripts/copy_rootfs.sh sdb trebletrouble

install-sdcard-boot:
	OETMP=bbb/build/tmp ./bbb/meta-bbb/scripts/copy_boot.sh sdb

all:	app image
