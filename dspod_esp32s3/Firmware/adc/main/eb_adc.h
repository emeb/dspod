/*
 * eb_adc.h - my high-level ADC driver. Mimics background ADC operation.
 * 01-16-22 E. Brombaugh
 */

#ifndef __eb_adc__
#define __eb_adc__

#define ADC_NUMVALS 4

extern volatile int16_t adc_val[ADC_NUMVALS];

esp_err_t eb_adc_init(void);

#endif
