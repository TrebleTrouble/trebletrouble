#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stropts.h>
#include <unistd.h>

#include <linux/fb.h>
#include <sys/mman.h>

int display(void) {
	int fbfd, i, xpos;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long screensize;
	char* fbp;

	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd == -1) {
		printf("Error: cannot open framebuffer.\n");
		return 1;
	}

	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Error reading variable screen info\n");
	}

	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Error reading fixed screen info\n");
	}

	screensize = finfo.smem_len;

	fbp = (char*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, 
			  fbfd, 0);
	if ((long)fbp == -1) {
		printf("Mmap of screen failed.\n");
		return 1;
	}

	memset(fbp, 0x00, screensize);
	for (i = 0; i < screensize; i++) {
		if (i%4 == 0)
			memset(fbp + i, 0x26, 1);
		if (i%4 == 1)
			memset(fbp + i, 0x45, 1);
		if (i%4 == 2)
			memset(fbp + i, 0xdf, 1);
	}

	// fuck it, loop forever
	while(1);

	munmap(fbp, screensize);
	close(fbfd);

	return 0;
}
