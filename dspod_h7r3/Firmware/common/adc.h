/*
 * adc.h - h7r3 adc setup
 * E. Brombaugh 11-03-2025
 */

#ifndef __adc__
#define __adc__

#include "stm32h7rsxx_hal.h"

#define ADC_BUFSZ 4

void ADC_Init(void);
int16_t ADC_GetChl(uint8_t chl);

#endif
