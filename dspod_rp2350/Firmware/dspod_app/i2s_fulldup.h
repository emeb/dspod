/*
 * i2s_fulldup.h - PIO-based full-duplex I2S for RP2040 & RP2350
 */

#ifndef __i2s_fulldup__
#define __i2s_fulldup__

#include "main.h"

void init_i2s_fulldup(void);
int8_t i2s_cpu_load(void);

#endif