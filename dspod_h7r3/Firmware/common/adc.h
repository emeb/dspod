/*
 * adc.h - h7r3 adc setup
 * E. Brombaugh 11-03-2025
 */

#ifndef __adc__
#define __adc__

#include "main.h"

#define ADC_BUFSZ 4

extern volatile int16_t ADC_val[ADC_BUFSZ];

void ADC_Init(void);

#endif
