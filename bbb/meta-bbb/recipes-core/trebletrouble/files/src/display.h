#ifndef DISPLAY_H_
#define DISPLAY_H_

int find_freq(double freq);
int find_ind(char note, int oct);
int bitblit(char* filename, char* fbp, int x, int y);

void cleanup_display(char* fbp, int *fbfd);
void display_frequency(double frequency, char* fbp);
char *init_display(int *fbfd);
void colour_screen(char* fbp, short colour);

#endif
