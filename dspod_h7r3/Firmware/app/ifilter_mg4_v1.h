/*
 * filter_mg4_v1.c - 4-stage Moog ladder filter, version 1
 * from http://musicdsp.org/archive.php?classid=3#25 
 */

#ifndef __ifilter_mg4__
#define __ifilter_mg4__

#include "fx.h"

typedef struct {
	int32_t f, p, q;				// filter coefficients
	int32_t b0, b1, b2, b3, b4;		// filter buffers (beware denormals!)
	int32_t t1, t2;					// temporary buffers
	int32_t gain;					// DC gain correction
	uint8_t bypass;					// filter bypass
} ifmg4_state;

void init_ifilter_mg4(ifmg4_state *f);
void set_ifilter_mg4(ifmg4_state *f, int16_t fc, int16_t res, uint8_t bypass);
void dupe_ifilter_mg4(ifmg4_state *f1, ifmg4_state *f2);
int16_t ifilter_mg4(ifmg4_state *f, int16_t input);

#endif
