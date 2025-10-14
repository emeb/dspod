/*
 * audio.h - audio processing for RP2350 I2S
 * 02-04-25 E. Brombaugh
 */
#ifndef __audio__
#define __audio__

#include "main.h"

/* processing params */
#define SMPS 32
#define CHLS 2
#define BUFSZ (SMPS*CHLS)

void Audio_Init(uint32_t smpl_rate);
void Audio_Proc(int16_t *dst, int16_t *src, int32_t sz);
void Audio_Fore(void);
int16_t Audio_get_level(uint8_t idx);
void Audio_Mute(int8_t enable);

#endif
