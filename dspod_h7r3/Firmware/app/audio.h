/*
 * audio.h - audio handler H7R3 i2s audio
 * 11-02-25 E. Brombaugh
 */

#ifndef __audio__
#define __audio__

#include "main.h"

#define SMPS 128
#define CHLS 2
#define BUFSZ (SMPS*CHLS)

void Audio_Init(uint32_t smpl_rate);
void Audio_Proc(int16_t *dst, int16_t *src, uint32_t sz);
int16_t Audio_get_level(uint8_t idx);
void Audio_Mute(int8_t enable);
int8_t Audio_cpu_load(void);

#endif


