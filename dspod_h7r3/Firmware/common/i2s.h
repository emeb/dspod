/*
 * i2s.c - spi-i2s driver for H503 I2S Audio
 * 09-25-21 E. Brombugh
 */

#ifndef __i2s__
#define __i2s__

#include "stm32h7rsxx_hal.h"
#include "audio.h"

#define I2S_BUFSZ (SMPS*CHLS)

void i2s_init(void);
void i2s_start(void);
int32_t i2s_get_fsamp(void);

#endif


