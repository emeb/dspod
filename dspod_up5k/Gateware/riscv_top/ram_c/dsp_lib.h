/*
 * dsp_lib.h - miscellaneous DSP stuff for dspod_u3c5
 * 05-09-26 E. Brombaugh
 */

#ifndef __dsp_lib__
#define __dsp_lib__

#include "main.h"

uint8_t dsp_gethyst(int16_t *oldval, int16_t newval);
uint8_t dsp_ratio_hyst_arb(uint16_t *old, uint16_t in, uint8_t range);

#endif

