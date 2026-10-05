/*
 * eb_adc.h - my high-level ADC driver. Mimics background ADC operation.
 * 01-16-22 E. Brombaugh
 */

#ifndef __eb_adc__
#define __eb_adc__

#define ADC_NUMVALS 4

extern volatile int16_t ADC_val[ADC_NUMVALS];
extern volatile esp_cpu_cycle_count_t adc_sample_period;
/* compute stats */
//#define DO_STATS
 
#ifdef DO_STATS
#define NUM_HIST 1000
extern volatile int16_t adc_hist[ADC_NUMVALS][NUM_HIST], hist_cnt, hist_done;
#endif

esp_err_t eb_adc_init(void);

#endif
