/*
 * fft_r4_cmsis.h - Radix 4 FFT, kanged from CMSIS
 * 10-02-25 E. Brombaugh
 */

#ifndef __fft_r4_cmsis__
#define __fft_r4_cmsis__

int fft_r4_cmsis_setup(int len);
void fft_r4_cmsis_destroy(void);
void fft_r4_cmsis(float *inout, uint8_t ifftFlag, uint8_t bitReverseFlag);

#endif