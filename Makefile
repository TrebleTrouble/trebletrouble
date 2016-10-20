bin/%: src/%.c
	$(CC) $(CFLAGS) $< -o $@

image:
	. ./poky-krogoth/oe-init-build-env bbb/build/ && bitbake trebletrouble-image

all:	image
