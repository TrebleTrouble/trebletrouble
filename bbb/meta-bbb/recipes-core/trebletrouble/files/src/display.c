#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stropts.h>
#include <unistd.h>

#include <linux/fb.h>
#include <sys/mman.h>

int DISP_WIDTH = 1;
long SCREENSIZE;

int bitblit(char* filename, char* fbp, int x, int y) {
	FILE* fd;
	int i, j, w, h;
	unsigned char buf[3], px[2];

	fd = fopen(filename, "rb");
	if (fd == NULL)
		return -1;
	fread(buf, 1, 3, fd);
	if (buf[0] != 'P' || buf[1] != '6') {
		fclose(fd);
		return -2;
	}
	fread(buf, 1, 3, fd);
	w = 0;
	for (i = 0; i < 3; i++) {
		if (buf[i] == ' ' || (9 < buf[i] && buf[i] < 13))
			break;
		w *= 10;
		w += buf[i] - '0';
	}
	w--;
	if (i == 3) {
		/* throw away whitespace */
		fread(buf, 1, 1, fd);
	} else if (i < 2) {
		for (j = 0; j+i+1 < 3; i++) {
			buf[j] = buf[j+i+1];
		}
	}
	fread(buf, 1, 3, fd);
	h = 0;
	for (i = 0; i < 3; i++) {
		if (buf[i] == ' ' || (9 < buf[i] && buf[i] < 13))
			break;
		h *= 10;
		h += buf[i] - '0';
	}
	h--;
	if (i == 3) {
		buf[0] = fgetc(fd);
	}
	do {
		buf[0] = fgetc(fd);
	} while (buf[0] != ' ' && (buf[0] < 9 || 13 < buf[0]));

	for (i = 0; i < SCREENSIZE/2; i++) {
		if ((i / DISP_WIDTH) < y)
			continue;
		if ((i % DISP_WIDTH) < x || (x+w) < (i % DISP_WIDTH))
			continue;
		if ((y+h) < (i / DISP_WIDTH))
			break;
		if (fread(buf, 1, 3, fd) != 3) {
			break;
		}
		if (buf[0] == 0xFF && buf[1] == 0xFF && buf[2] == 0xFF)
			continue;
		px[0] = (buf[0] & 0xF8) | ((buf[1] & 0xE0) >> 5);
		px[1] = ((buf[1] & 0x1C) << 3) | ((buf[2] & 0xF8) >> 3);
		memset(fbp + 2*i, *(short*)px, 2);
	}

	fclose(fd);
	return 0;
}

char *init_display(int *fbfd) {
	int i, xpos, err;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	char* fbp;

	*fbfd = open("/dev/fb0", O_RDWR);
	if (*fbfd == -1) {
		printf("Error: cannot open framebuffer.\n");
		return NULL;
	}

	if (ioctl(*fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Error reading variable screen info\n");
	}

	if (ioctl(*fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Error reading fixed screen info\n");
	}

	DISP_WIDTH = vinfo.xres;
	SCREENSIZE = finfo.smem_len;

	fbp = (char*)mmap(0, SCREENSIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
			  *fbfd, 0);
	if ((long)fbp == -1) {
		printf("Mmap of screen failed.\n");
		return NULL;
	}

	memset(fbp, 0x00, SCREENSIZE);
	for (i = 0; i < SCREENSIZE/2; i++) {
		/*
		 * So, we have 16-bits per pixel. It is split up into 
		 * 5 bits for Red, 6 bits for Green and 5 bits for Blue
		 * Why 6 bits for Green? Human biology, that's why.
		 * The least significant _byte_ is first and the most 
		 * significant byte is second. Byte order is always the same.
		 * 0x25E2 is actually the colour E225 which is orange.
		 * Red - 1110 0; Green - 010 001; Blue - 0 0101)
		 */
		memset(fbp + 2*i, 0x25E2, 2);
	}
	return fbp;
}

void cleanup_display(char* fbp, int *fbfd) {
	munmap(fbp, SCREENSIZE);
	close(*fbfd);
}
