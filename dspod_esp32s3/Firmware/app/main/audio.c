/*
 * audio.c - audio handler for dspod H7R3 fx app
 * 11-15-25 E. Brombaugh
 */

#include <math.h>
#include <string.h>
#include "main.h"
#include "esp_timer.h"
#include "audio.h"
#include "dsp_lib.h"
#include "eb_adc.h"
#include "eb_i2s.h"
#include "multicore_audio.c"
#include "fx.h"

enum fadestate 
{
    FADE_OFF,
    FADE_IN,
    FADE_OUT,
};

static uint32_t Fsamp;
int16_t proc_buf[BUFSZ];
uint32_t fadecnt;
uint8_t init, fadest;
int16_t audio_sl[4];
uint64_t audio_load[3];
volatile int16_t audio_mute_state, audio_mute_cnt;
int16_t prev_wet;

/*
 * signal level calc
 */
void IRAM_ATTR level_calc(int16_t sig, int16_t *level)
{
	/* rectify */
	sig = (sig < 0) ? -sig : sig;

	/* peak hold - externally reset */
	if(*level < sig)
		*level = sig;
}

/*
 * Audio processing
 * dst - ptr to dest buffer
 * src - ptr to src buffer
 * len - num uint8_t in buffer (/2 for int16_t R/L samples)
 */
void IRAM_ATTR Audio_Proc(int16_t *dst, int16_t *src, uint32_t len)
{
	int16_t *prc = proc_buf;
	int32_t sz = len/2;			// num int16_t in buffer
	int32_t inframes = sz/2;	// num stereo samples eg "frames" in buffer
	uint16_t index;
	int32_t wet, dry, mix;
	float live_wet, slope_wet;
	
	/* update start time for load calcs */
	audio_load[2] = audio_load[0];
	audio_load[0] = esp_timer_get_time();
	
	/* check input levels */
	for(index=0;index<inframes;index++)
	{
		level_calc(*(src+0), &audio_sl[0]);
		level_calc(*(src+1), &audio_sl[1]);
	}
	
	/* apply the effect */
	fx_proc(prc, src, inframes);
	
	/* set W/D mix gain and prep linear interp */
	wet = ADC_val[3];
	live_wet = prev_wet;
	slope_wet = (float)(wet - prev_wet) / (float)inframes;
	prev_wet = wet;

	/* W/D mixing and output level detect */
	for(index=0;index<inframes;index++)
	{
		/* linear interp W/D mix gain */
		wet = live_wet;
		dry = 0xfff - wet;
		live_wet += slope_wet;
		
		/* W/D with saturation */
		mix = *prc++ * wet + *src++ * dry;
		*dst++ = dsp_ssat16(mix>>12);
		mix = *prc++ * wet + *src++ * dry;
		*dst++ = dsp_ssat16(mix>>12);

		/* handle muting */
		switch(audio_mute_state)
		{
			case 0:
				/* pass thru and wait for foreground to force a transition */
				break;
			
			case 1:
				/* transition to mute state */
				mix = (*(dst-2) * audio_mute_cnt);
				*(dst-2) = dsp_ssat16(mix>>9);
				mix = (*(dst-1) * audio_mute_cnt);
				*(dst-1) = dsp_ssat16(mix>>9);
				audio_mute_cnt--;
				if(audio_mute_cnt == 0)
					audio_mute_state = 2;
				break;
				
			case 2:
				/* mute and wait for foreground to force a transition */
				*(dst-2) = 0;
				*(dst-1) = 0;
				break;
			
			case 3:
				/* transition to unmute state */
				mix = (*(dst-2) * audio_mute_cnt);
				*(dst-2) = dsp_ssat16(mix>>9);
				mix = (*(dst-1) * audio_mute_cnt);
				*(dst-1) = dsp_ssat16(mix>>9);
				audio_mute_cnt++;
				if(audio_mute_cnt == 512)
				{
					audio_mute_state = 0;
					audio_mute_cnt = 0;
				}
				break;
				
			default:
				/* go to legal state */
				audio_mute_state = 0;
				break;
		}

		/* check output levels */
		level_calc(*(dst-2), &audio_sl[2]);
		level_calc(*(dst-1), &audio_sl[3]);
	}

	/* update end timer */
	audio_load[1] = esp_timer_get_time();
}

/*
 * initialize the audio processing
 */
void Audio_Init(uint32_t smpl_rate)
{
	/* set up basic osc params */
	Fsamp = smpl_rate;
	
	/* signal levels */
	audio_sl[0] = audio_sl[1] = audio_sl[2] = audio_sl[3] = 0;
#ifndef MULTICORE
	audio_load[0] = audio_load[1] = audio_load[2] = 0;
#endif

	/* Muting */
	audio_mute_state = 2;	// start up  muted
	audio_mute_cnt = 0;
	
	/* init fx */
	fx_init();
	
	/* start I2S */
	i2s_init(Audio_Proc);
}

/*
 * get audio level for in/out right/left
 */
int16_t Audio_get_level(uint8_t idx)
{
	idx &= 3;
	int16_t result = audio_sl[idx];
	audio_sl[idx] = 0;
	return result;
}

/*
 * Audio Mute enable - run by core 0 foreground
 * blocks until operation is complete
 */
void Audio_Mute(int8_t enable)
{
	
	if((audio_mute_state == 0) && (enable == 1))
	{
		printf("Audio_Mute: requesting mute\n\r");
		audio_mute_cnt = 512;
		audio_mute_state = 1;
		while(audio_mute_state != 2);
	}
	else if((audio_mute_state == 2) && (enable == 0))
	{
		printf("Audio_Mute: requesting unmute\n\r");
		audio_mute_cnt = 0;
		audio_mute_state = 3;
		while(audio_mute_state != 0);
	}
}

/*
 * get audio CPU load in %
 */
int8_t Audio_cpu_load(void)
{
#ifdef MULTICORE
	uint64_t period = mc_period;
	uint64_t duration = mc_duration;
#else
	uint64_t period = audio_load[0] - audio_load[2];
	uint64_t duration = audio_load[1] - audio_load[0];
#endif
	if(period != 0)
		return 100*duration/period;
	else
		return 0;
}