/*
 * audio.c - audio processing for RP2350 I2S
 * 02-04-25 E. Brombaugh
 */

#include <math.h>
#include <string.h>
#include "main.h"
#include "audio.h"

static uint32_t Fsamp;
int32_t __scratch_x("core1") phs, __scratch_x("core1") frq;
#define SINE_PHSBITS 12
#define SINE_SZ (1<<(SINE_PHSBITS))
int16_t sinetab[SINE_SZ];

/*
 * initialize
 */
void Audio_Init(uint32_t smpl_rate)
{
	/* set up basic osc params */
	Fsamp = smpl_rate;
	frq = (1000.0F/(float)Fsamp) * 4294967296.0F;
	phs = 0;
		
	/* init sine table */
	for(int i=0;i<SINE_SZ;i++)
	{
		float th = (float)i/(float)SINE_SZ;
		sinetab[i] = floorf(32767.0*sinf(6.2832f*th) + 0.5);
	}
	
}

/*
 * Audio processing - normally in audio.c but we're cheating here
 */
void Audio_Proc(int16_t *dst, int16_t *src, int32_t sz)
{
#if 1
	/* just copy src to dest for loopback */
	memcpy(dst, src, sz*sizeof(int16_t));
#else
#if 1
	/* sawtooth generator */
	for(int i=0;i<sz;i+=2)
	{
		dst[i] = sinetab[(uint32_t)phs>>(32-SINE_PHSBITS)];
		dst[i+1] = phs>>16;
		phs += frq;
	}
#else
	/* set msbit of output to test alignment */
	for(int i=0;i<sz;i+=2)
	{
		dst[i] = 0x8000;
		dst[i+1] = 0x8000;
	}	
#endif
#endif
}


/*
 * Audio foreground task - run by core 1 if multicore
 */
void Audio_Fore(void)
{
}

