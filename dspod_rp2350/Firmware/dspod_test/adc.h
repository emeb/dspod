/*
 * ADC.h - background ADC management
 */

#ifndef __ADC__
#define __ADC__

#define ADC_NUMVALS 4

extern volatile int16_t ADC_val[ADC_NUMVALS];

void ADC_init(void);

#endif