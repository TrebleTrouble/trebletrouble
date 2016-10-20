bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

bin/app: src/app.c bin/display.o
	$(CC) $(CFLAGS) $< bin/display.o -o $@

app:	bin/app

image:
	. ./poky-krogoth/oe-init-build-env bbb/build/ && bitbake trebletrouble-image

all:	image
