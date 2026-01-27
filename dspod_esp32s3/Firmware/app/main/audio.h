/*
 * audio.h - audio handler H7R3 i2s audio
 * 11-02-25 E. Brombaugh
 */

#ifndef __audio__
#define __audio__

#include "main.h"

#ifdef SPECT
#define SMPS 1024
#else
#define SMPS 128
#endif

#define CHLS 2
#define BUFSZ (SMPS*CHLS)

extern uint64_t audio_load[3];

void Audio_Init(uint32_t smpl_rate);
int16_t Audio_get_level(uint8_t idx);
void Audio_Mute(int8_t enable);
int8_t Audio_cpu_load(void);

#endif


