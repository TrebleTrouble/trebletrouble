/*
 *  Copyright (C) 2016-2017  William Pearson
 *  Copyright (C) 2016-2017  Emilie Cobbold
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Additional permission under GNU GPL version 3 section 7
 *
 *  If you modify this Program, or any covered work, by linking or
 *  combining it with the uGFX Library, containing parts covered by the
 *  terms of the GFX License, Version 1.2, the licensors of this Program
 *  grant you additional permission to convey the resulting work.
 *  Corresponding Source for a non-source form of such a combination
 *  shall include the source code for the parts of the uGFX Library used
 *  as well as that of the covered work.
 *
 */

#include <stdio.h>
#include <string.h>

#include "colours.h"
#include "display.h"
#include "fileFormat.h"
#include "fileRead.h"

extern int DISP_WIDTH;
extern long SCREENSIZE;
/*x_start for new song format*/
static int x_start;

#define KEYS 88

double FREQS[KEYS] = {27.5, 29.1352, 30.8677, 32.7032, 34.6478, 36.7081, 38.8909, 41.2034, 43.6535, 46.2493, 48.9994, 51.9131, 55.0, 58.2705, 61.7354, 65.4064, 69.2957, 73.4162, 77.7817, 82.4069, 87.3071, 92.4986, 97.9989, 103.826, 110.0, 116.541, 123.471, 130.813, 138.591, 146.832, 155.563, 164.814, 174.614, 184.997, 195.998, 207.652, 220.0, 233.082, 246.942, 261.626, 277.183, 293.665, 311.127, 329.628, 349.228, 369.994, 391.995, 415.305, 440.0, 466.164, 493.883, 523.251, 554.365, 587.330, 622.254, 659.255, 698.456, 739.989, 783.991, 830.609, 880.0, 932.328, 987.767, 1046.50, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 1760.0, 1864.66, 1975.53, 2093.0, 2217.46, 2349.32, 2489.02, 2637.02, 2793.83, 2959.96, 3135.96, 3322.44, 3520.0, 3729.31, 3951.07, 4186.01};

char *OCTAVE[9] = {"/srv/trebletrouble/0.pnm", "/srv/trebletrouble/1.pnm", "/srv/trebletrouble/2.pnm", "/srv/trebletrouble/3.pnm", "/srv/trebletrouble/4.pnm", "/srv/trebletrouble/5.pnm", "/srv/trebletrouble/6.pnm", "/srv/trebletrouble/7.pnm", "/srv/trebletrouble/8.pnm"};

char *TIME_STAMP[10] = {"","","/srv/trebletrouble/2_ts.pnm","/srv/trebletrouble/3_ts.pnm","/srv/trebletrouble/4_ts.pnm","/srv/trebletrouble/5_ts.pnm","","/srv/trebletrouble/7_ts.pnm","/srv/trebletrouble/8_ts.pnm", ""};

/* Internal functions */
int find_freq_recur(double freq, int i, int len) {
	while (1) {
		if (FREQS[i] <= freq) {
			if (freq <= FREQS[i+1]) {
				if ((freq - FREQS[i]) < (FREQS[i+1] - freq))
					return i;
				else
					return i+1;
			} else
				i += (len%2 == 0 ? len/4 : len/4 + 1);
		} else
			i -= (len%2 == 0 ? len/4 : len/4 + 1);
		len /= 2;
	}
}

int get_xnote(int i) {
	/* First note is at 145,
	   each subsequent note is 34 px further */
	return x_start + i*XS;
}

void draw_note(int i, int ynote, char* fbp, short colour, int time) {
		
	/* Fall code - to be deleted when new format is working
	draw lines if notes are above/below staff
	if(ynote < 61){
		bitblit(L_PIC, fbp, get_xnote(i)-3, 45);
	}
	if(ynote < 31){
		bitblit(L_PIC, fbp, get_xnote(i)-3, 15);
	}
	if(ynote > 239){
		bitblit(L_PIC, fbp, get_xnote(i)-3, 225);
	}

	*if ynote on treble clef is above B - flip the stem
	*150 = 90+30+30
	if(ynote < 150){
		if (time == 1)
			bitblit_colour(Q_N_F, fbp, get_xnote(i), ynote-15, colour);
		else 
			bitblit_colour(W_N, fbp, get_xnote(i), ynote-15, colour);
	}else{
		if (time == 1)
			bitblit_colour(Q_N, fbp, get_xnote(i), ynote-105, colour);
		else 
			bitblit_colour(W_N, fbp, get_xnote(i), ynote-105, colour);

	}*/
}

void set_time_signature(int t1,int t2, char *fbp){
	int y = 90;
	bitblit(TIME_STAMP[t1], fbp, x_start, y);
	bitblit(TIME_STAMP[t2], fbp, x_start, y+60);
	x_start = x_start+20;
}

void draw_bar(char *fbp, int x_pos){
	int y = 90;
	bitblit(BAR, fbp, x_pos-5,y);
	/*TODO: Delete when for loop has been written in load_notes*/
	/*
	for(;numBars>0;numBars--){
		bitblit(BAR, fbp, x_start+(XS*4*pass)-5, y);
		pass++;	
	}*/
	/* Fall code - to be deleted when new format is working
	hard coded bar spaces for now
	with xstart being defined in display.h, and there's 4 notes per bar
	each bar will be 160 apart
	int y = 90;
	bitblit("/srv/trebletrouble/bar.pnm", fbp, XSTART+(XS*4)-5, y);
	bitblit("/srv/trebletrouble/bar.pnm", fbp, XSTART+(XS*4*2)-5, y);
	bitblit("/srv/trebletrouble/bar.pnm", fbp, XSTART+(XS*4*3)-5, y);*/
}

int draw_key(char *fbp, char key){
	/* p is x variable for the pitch signatures*/
	int p = 64;
	/* m is the x interval for each pitch */
	int m = 15;
	int y = 90;
	/*Drawing key Signature*/
	if (0x80 & key){
		if (0x40 & key){
			bitblit(SHARP, fbp, p, y-15);
			p = p+m;
		}if(0x20 & key){
			bitblit(SHARP, fbp, p, y+30);
			p = p+m;
		}if(0x10 & key){
			bitblit(SHARP, fbp, p, y-30);
			p = p+m;
		}if(0x08 & key){
			bitblit(SHARP, fbp, p, y+15);
			p = p+m;
		}if(0x04 & key){
			bitblit(SHARP, fbp, p, y+60);
			p = p+m;
		}if(0x02 & key){
			bitblit(SHARP, fbp, p, y);
			p = p+m;
		}if(0x01 & key){
			bitblit(SHARP, fbp, p, y+45);
			p = p+m;
		}
	}else{
		if (0x40 & key){
			bitblit(FLAT, fbp, p, y+45);
			p = p+m;
		}if(0x20 & key){
			bitblit(FLAT, fbp, p, y);
			p = p+m;
		}if(0x10 & key){
			bitblit(FLAT, fbp, p, y+60);
			p = p+m;
		}if(0x08 & key){
			bitblit(FLAT, fbp, p, y+15);
			p = p+m;
		}if(0x04 & key){
			bitblit(FLAT, fbp, p, y+75);
			p = p+m;
		}if(0x02 & key){
			bitblit(FLAT, fbp, p, y+30);
			p = p+m;
		}if(0x01 & key){
			bitblit(FLAT, fbp, p, y+90);
			p = p+m;
		}
	}
	return p;
}

/* External API */
int find_freq(double freq) {
	if (freq <= FREQS[0])
		return 0;
	if (FREQS[KEYS-1] <= freq)
		return KEYS-1;
	return find_freq_recur(freq, KEYS / 2, KEYS);
}

int find_ind(char note, int oct){
	int ind, v;

	ind = (oct*12);

	v = ((note - 'C'+7)%7)*2;

	if (v > 4)
		v--;

	return ind+v-9;
}

int bitblit_colour(char* filename, char* fbp, int x, int y, short colour) {
	FILE* fd;
	int i, j, w, h, vw;
	unsigned char buf[3], px[2];

	if (!fbp)
		return -3;

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
	j = 0;
	if (i == 3) {
		/* throw away whitespace */
		fread(buf, 1, 1, fd);
	} else if (i < 2) {
		for (; j+i+1 < 3; j++) {
			buf[j] = buf[j+i+1];
		}
	}
	fread(buf+j, 1, 3-j, fd);
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

	vw = (x+w) > 800 ? x+w-799 : 0;

	for (i = 0; i < SCREENSIZE/2; i++) {
		if ((i / DISP_WIDTH) < y)
			continue;
		if (vw && (i % DISP_WIDTH) == DISP_WIDTH-1)
			fseek(fd, 3*vw, SEEK_CUR);
		if ((i % DISP_WIDTH) < x || (x+w) < (i % DISP_WIDTH))
			continue;
		if ((y+h) < (i / DISP_WIDTH))
			break;
		if (fread(buf, 1, 3, fd) != 3) {
			break;
		}
		if (buf[0] == 0xFF && buf[1] == 0xFF && buf[2] == 0xFF)
			continue;
		if (buf[0] == 0x00 && buf[1] == 0x00 && buf[2] == 0x00) {
			memset(fbp + 2*i, colour, 2);
			continue;
		}
		px[0] = (buf[0] & 0xF8) | ((buf[1] & 0xE0) >> 5);
		px[1] = ((buf[1] & 0x1C) << 3) | ((buf[2] & 0xF8) >> 3);
		memset(fbp + 2*i, *(short*)px, 2);
	}

	fclose(fd);
	return 0;
}

void colour_screen(char* fbp, short colour) {
	int i;

	if (!fbp)
		return;
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
		memset(fbp + 2*i, colour, 2);
	}
}

int bitblit(char* filename, char* fbp, int x, int y) {
	return bitblit_colour(filename, fbp, x, y, BLACK);
}

void draw_staff(char* fbp){
	/* place staff (five lines) on screen */
	int i;
	int x = 0;
	int y = 90;
	/* m is the space between the lines */
	int m = 30;
	int start =0;
	int end = 800;
	/* draw clef and end lines */
	bitblit("/srv/trebletrouble/clef.pnm", fbp, start, y-35);
	bitblit("/srv/trebletrouble/bar.pnm", fbp, end-16, y);
	bitblit("/srv/trebletrouble/end.pnm", fbp, end-7, y);

	/* draw the lines of the staff */
	for(i=0; i < 5; i++, y+=m)
		bitblit("/srv/trebletrouble/line.pnm", fbp, x, y);
}

int get_ynote(int i) {
	int j;

	/* How does this work?
	 * A0 is at index 0, so the value of actual will be 0 in that case.
	 * We want to re-index it so that C0 is 0 which means adding 9 for the
	 * calculations hence the subtraction after we have the value for ynote.
	 *	0, 1  -> C ( 0 )
	 *	2, 3  -> D ( 1 )
	 *	4     -> E ( 2 )
	 *	5, 6  -> F ( 3 )
	 *	7, 8  -> G ( 4 )
	 *	9, 10 -> A ( 5 )
	 *	11    -> B ( 6 )
	 * If we divide by 2, the odd numbers will go have their result rounded
	 * down. This is good for 1 and 3 but for values over 5, we want them
	 * rounded up. This is easily accomplished by incrementing those values
	 * by 1 before performing the division.
	 *
	 * The spacing between successive notes is 15 so we multiple our j value
	 * by 15.
	 *
	 * The spacing between successive octaves is 7 notes - 15 * 7 = 105
	 * We have set C4 to 240. C is mapped to 0 so no need to account for any
	 * adjustment there. But, the octave number will be 4, so we need to
	 * subtract 4 before multiplying by 105. Octaves which are lower will
	 * have a negative portion here and octaves which are higher will have a
	 * positive portion.
	 *
	 * B3 will be mapped to 6 and 3 -> 6*15 = 90, 105*(3-4) = -105
	 * 90-105 = -15 - exactly where we want to place B3 relative to C4
	 */
	i += 9;
	j = i%12;
	if (j > 4)
		j++;
	j /= 2;
	return (240 - (15*j + 105*(i/12 - 4)));
}

void compare_notes(int expected, int actual, int i, char* fbp) {
	short colour = actual == expected ? GREEN : RED;
	draw_note(i, get_ynote(actual), fbp, colour, 1);
}

void clear_notes(int i, int expected[NUM_NOTES], int actual[NUM_NOTES], char* fbp) {
	int actual_ynote;
	for (; i < NUM_NOTES; i++) {
		actual_ynote = get_ynote(actual[i]);
		draw_note(i, actual_ynote, fbp, WHITE, 1);
		if (!(actual_ynote%30))
			bitblit(L_PIC, fbp, get_xnote(i)-3, get_ynote(actual[i])-14);
		draw_note(i, get_ynote(expected[i]), fbp, BLACK, 1);
	}
}

void load_song(char *fbp, int bpm, int numBars, int numNotes, char key, unsigned char timeSignature){
//	unsigned char buf[2];
//	int i, ynote;

	/*TESTING ONLY - Twinkle Twinkle
	Time signature:
	Timesig = Ts1-1 * 16 + Ts2-1 
	so for 4/4= 51
	so for 3/4= 35*/
	
	/*Draw the key, return the current x value*/	
	x_start = draw_key(fbp, key);

	/*Take ints from char away*/
	int ts1 = (timeSignature/16)+1;
	int ts2 = (timeSignature%16)+1;
	set_time_signature(ts1, ts2, fbp);

}

/*Old load song file - to be deleted when new one is working 
void load_song(FILE *song, char *fbp, int expected[NUM_NOTES]) {
	unsigned char buf[2];
	int i, ynote;
	
	//for now, assume song is is in time signature 4 4 - common time
	set_time_signature(4,4, fbp);
	
	//draw bars on staff
	draw_bar(fbp);

	for (i = 0; i < NUM_NOTES && fread(buf, 1, 2, song) == 2; i++) {
		expected[i] = find_ind(buf[0], buf[1] - '0');
		ynote =(240 - (((buf[1]-'4') * 105) + (((buf[0]-'C'+7)%7)*15)));
		draw_note(i, ynote, fbp, BLACK, 1);
	}
}*/
