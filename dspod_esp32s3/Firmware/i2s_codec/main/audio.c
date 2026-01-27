/*
 * audio.c - audio via I2S.
 * 03-9-22 E. Brombaugh
 */
 
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "audio.h"
#include "eb_i2s.h"

#define FRAMESZ			(64)

static const char* TAG = "audio";
uint64_t audio_load[3];
int16_t audio_mute_state, audio_mute_cnt;
int16_t *rxbuf = NULL;
int16_t prc[2*FRAMESZ];
uint32_t frq[2] = { 0x02aaaaaa, 0}, phs[2] = { 0, 0};

/*
 * NOTE: ESP32 HW interleaves stereo R/L/R/L with R @ index 0
 * (opposite of most other systems)
 */
 
/*
 * Audio processing callbacks
 */
void audio_proc_cb(int16_t *dst, int16_t *src, uint32_t len)
{
	uint8_t i;
	
	/* update start time for load calcs */
	audio_load[2] = audio_load[0];
	audio_load[0] = esp_timer_get_time();
	
	len >>= 2;	// len input is in bytes - we need stereo 16-bit samples
		
	/* handle output */
	for(i=0;i<len;i++)
	{
		/* pass-thru */
		dst[2*i] = src[2*i];
		dst[2*i+1] = src[2*i+1];
	}
	
	/* update end timer */
	audio_load[1] = esp_timer_get_time();
}

/*
 * initializer
 */
esp_err_t audio_init(void)
{
	/* init state */
	audio_load[0] = audio_load[1] = audio_load[2] = 0;
	
	/* init I2S w/ audio processing callback */
    ESP_LOGI(TAG, "Initialize I2S");
	i2s_init(audio_proc_cb);
	
	return ESP_OK;
}
