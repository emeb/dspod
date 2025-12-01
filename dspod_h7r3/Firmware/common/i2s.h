/*
 * i2s.c - spi-i2s driver for H503 I2S Audio
 * 09-25-21 E. Brombugh
 */

#ifndef __i2s__
#define __i2s__

#include "main.h"
#include "audio.h"

#define I2S_BUFSZ BUFSZ

extern DMA_NodeTypeDef Node_GPDMA1_Channel1;

void i2s_init(void);
void i2s_start(void);
int32_t i2s_get_fsamp(void);

#endif


