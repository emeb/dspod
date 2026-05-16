/*
 * adc.h - dspod_u3c5 4 channel adc setup
 * 05-05-2026 E. Brombaugh
 */

#ifndef __adc__
#define __adc__

#include "main.h"

#define ADC_BUFSZ 4

extern volatile int16_t ADC_val[ADC_BUFSZ];

void ADC_Init(void);

#endif
