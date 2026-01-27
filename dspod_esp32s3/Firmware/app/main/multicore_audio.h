/*
 * multicore_audio.h - start audio processing on 2nd core
 * 12-28-24 E. Brombaugh
 */

#ifndef __multicore_audio__
#define __multicore_audio__

/* CPU loading stats */
extern uint64_t mc_duration, mc_period;

void multicore_audio_init(void);
void multicore_audio_select_algo(uint8_t algo);

#endif