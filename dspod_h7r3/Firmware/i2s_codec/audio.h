/*
 * audio.h - audio handler H7R3 i2s audio
 * 11-02-25 E. Brombaugh
 */

#ifndef __audio__
#define __audio__

#include "stm32h7rsxx_hal.h"

#define SMPS 128
#define CHLS 2

void Audio_Init(void);
void Audio_Proc(int16_t *dst, int16_t *src, uint32_t sz);

#endif


