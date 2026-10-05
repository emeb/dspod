/*
 * audio.h - audio via I2S.
 * 03-9-22 E. Brombaugh
 */

#ifndef __audio__
#define __audio__

extern uint64_t audio_load[3];
esp_err_t audio_init(void);

#endif
