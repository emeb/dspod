/*
 * si5351.h - SI5351 Clock Generator driver for dspod_u3c5
 * 01-08-2017 E. Brombaugh
 * 05-06-2026 E. Brombaugh - updated for stm32u3c5
 */

#ifndef __si5351__
#define __si5351__

#include "main.h"

HAL_StatusTypeDef si5351_init(void);
void si5351_set_output_chl(uint8_t chl, uint32_t freq, uint8_t odiv);
void si5351_set_output_3(uint8_t idiv, uint8_t odiv);
void si5351_oena(uint8_t chl, uint8_t state);
uint32_t si5351_get_vco_freq(uint8_t chl);

#endif
