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
	for (i = 0; i < screensize; i += 2) {
		/*
		 * So, we have 16-bits per pixel. It is split up into 
		 * 5 bits for Red, 6 bits for Green and 5 bits for Blue
		 * Why 6 bits for Green? Human biology, that's why.
		 * The least significant _byte_ is first and the most 
		 * significant byte is second. Byte order is always the same.
		 * 0x25E2 is actually the colour E225 which is orange.
		 * Red - 1110 0; Green - 010 001; Blue - 0 0101)
		 */
		memset(fbp + i, 0x25E2, 2);
	}

	// fuck it, loop forever
	while(1);

	munmap(fbp, screensize);
	close(fbfd);

	return 0;
}
