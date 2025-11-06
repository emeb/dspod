/*
 * audio.c - audio handler H530 i2s audio
 * 09-02-24 E. Brombaugh
 */

#include "audio.h"
#include "cyclesleep.h"

int32_t frq, phs;

/*
 * initialize the audio processing
 */
void Audio_Init(void)
{
	phs = 0;
	
	frq = (int32_t)floorf(100.0F * powf(2.0F, 32.0F) / 48076.0F);
}

/*
 * Process buffers of I2S data
 */
void Audio_Proc(int16_t *dst, int16_t *src, uint32_t sz)
{
	uint16_t i;
	
	start_meas();
	
#if 0
	/* loopback */
	for(i=0;i<sz;i++)
		*dst++ = *src++;
#else
	/* sawtooth output */
	for(i=0;i<sz/2;i++)
	{
		*dst++ = phs >> 17;
		*dst++ = -(phs >> 17);
		
		phs += frq;
	}
#endif
	
	end_meas();
}

