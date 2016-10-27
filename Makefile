bin/app: package/src/app.c bin/display.o
	$(CC) $(CFLAGS) $< bin/display.o -o $@

bin/%.o: package/src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

app:	bin/app

image:
	. ./poky-krogoth/oe-init-build-env bbb/build/ && bitbake -f -c fetch trebletrouble && bitbake -f -c compile trebletrouble && bitbake trebletrouble-image

all:	app image
