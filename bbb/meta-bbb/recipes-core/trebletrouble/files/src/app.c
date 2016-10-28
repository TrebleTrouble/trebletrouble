#include <stdio.h>

#include "display.h"

int main(int argc, char** argv) {
	char* fbp;
	int fbfd, err;

	fbp = init_display(&fbfd);

	err = bitblit("/srv/trebletrouble/timbit.pnm", fbp, 400, 240);

	if (err) {
		if (err == -1) {
			printf("File not found :(");
		} else if (err == -2) {
			printf("Not P6 file");
		}
	}

	cleanup_display(fbp, &fbfd);
	return 0;
}
