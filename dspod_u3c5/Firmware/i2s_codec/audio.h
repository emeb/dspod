/*
 * audio.h - audio handler for dspod_u3c5 i2s audio
 * 05-05-2026 E. Brombaugh
 */

#ifndef __audio__
#define __audio__

#include "main.h"

#define SMPS 128
#define CHLS 2
#define BUFSZ (SMPS*CHLS)

void Audio_Init(void);
void Audio_Proc(int16_t *dst, int16_t *src, uint32_t sz);

#endif


