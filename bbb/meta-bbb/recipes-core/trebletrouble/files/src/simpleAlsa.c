/*
 *  Copyright (C) 2016-2017  Daniel Rosales
 *  Copyright (C) 2009 Alessandro Ghedini <alessandro@ghedini.me>
 *
 *  This file is covered by the terms of BOTH licenses listed below.
 *
 *  --------------------------------------------------------------
 *  "THE BEER-WARE LICENSE" (Revision 42):
 *  Daniel Rosales wrote this file.
 *  Alessandro Ghedini wrote this file. As long as you retain this
 *  notice you can do whatever you want with this stuff. If we
 *  meet some day, and you think this stuff is worth it, you can
 *  buy me a beer in return.
 *  --------------------------------------------------------------
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
 *
 *
 *
 * 
 */

#include <alsa/asoundlib.h>
#include <stdio.h>
#include <sndfile.h>
#include <math.h>


#define PCM_DEVICE "default"
//WAV code
static snd_pcm_uframes_t frames;
static SNDFILE *infile = NULL;	


snd_pcm_t * init_pcm(char *infilename,short **buf){
        
	SF_INFO sfinfo;
	snd_pcm_hw_params_t *params;
	snd_pcm_t *pcm_handle;
	int dir;
	unsigned int tmp,pcm;
	dir = 0;
	if ( access( infilename, F_OK) ==-1){
		printf("File doesn't exists");	
	}
	else{
		//	printf("File does exist!");
	}
	infile = sf_open(infilename, SFM_READ, &sfinfo);
        /* Open the PCM device in playback mode */
	if ((pcm = snd_pcm_open(&pcm_handle, PCM_DEVICE,
				SND_PCM_STREAM_PLAYBACK, 0)) < 0) 
	  	printf("ERROR: Can't open \"%s\" PCM device. %s\n",
					PCM_DEVICE, snd_strerror(pcm));
       	/* Allocate parameters object and fill it with default values*/
	snd_pcm_hw_params_alloca(&params);
	snd_pcm_hw_params_any(pcm_handle, params);
	/* Set parameters */
	if ((pcm = snd_pcm_hw_params_set_access(pcm_handle, params,
						SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) 
		printf("ERROR: Can't set noninterleaved mode. %s\n", snd_strerror(pcm));
	//set formatting 
	if ((pcm = snd_pcm_hw_params_set_format(pcm_handle, params,
					       SND_PCM_FORMAT_S16_LE)) < 0) 
		printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));
	//Setting channels
	if ((pcm = snd_pcm_hw_params_set_channels(pcm_handle, params,1)) < 0) 
		printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));
	//Set sample rate
	if ((pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, (unsigned int*)&sfinfo.samplerate, &dir)) < 0) 
		printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));
	
		

	/* Resume information */
       	snd_pcm_hw_params_get_channels(params, &tmp);
	if (tmp == 1)
		printf("(mono)\n");
	else if (tmp == 2)
		printf("(stereo)\n");
	snd_pcm_hw_params_get_rate(params, &tmp, 0);
       	/* Write parameters */
	snd_pcm_hw_params(pcm_handle, params);
	/* Allocate buffer to hold single period */
	snd_pcm_hw_params_get_period_size(params, &frames, &dir);
	*buf = malloc(frames * sfinfo.channels * sizeof(short));
	return pcm_handle;
}
void cleanup_pcm(snd_pcm_t *pcm_handle,short *buf){
    //Drain the buffer
    snd_pcm_drain(pcm_handle);
    //Close the device
    snd_pcm_close(pcm_handle);
    free(buf);
    

}



void sound (snd_pcm_t *pcm_handle,short *buf){

	int pcmrc;
	short readcount;
    while ((readcount = sf_readf_short(infile, buf, frames))>0) {
	
        pcmrc = snd_pcm_writei(pcm_handle, buf, frames);
        if (pcmrc == -EPIPE) {
		//    fprintf(stderr, "Underrun!\n");
            snd_pcm_prepare(pcm_handle);
	        }
        else if (pcmrc < 0) {
		//            fprintf(stderr, "Error writing to PCM device: %s\n", snd_strerror(pcmrc));
        }
        else if (pcmrc != readcount) {
		//    fprintf(stderr,"PCM write differs from PCM read.\n");
        }

    }
    sf_seek(infile,0,SEEK_SET);
    }
