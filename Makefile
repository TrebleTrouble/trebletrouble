package/bin/app: package/src/app.c package/bin/display.o
	$(CC) $(CFLAGS) $< package/bin/display.o -o $@

package/bin/%.o: package/src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

app:	package/bin/app

image:
	. ./poky-krogoth/oe-init-build-env bbb/build/ && bitbake -f -c fetch trebletrouble && bitbake -f -c compile trebletrouble && bitbake trebletrouble-image

all:	app image
