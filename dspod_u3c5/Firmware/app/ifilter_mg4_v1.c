/*
 * ifilter_mg4_v1.c - 4-stage Moog ladder filter, version 1 - integer in S8.23
 * from http://musicdsp.org/archive.php?classid=3#25
 */
 
// Moog 24 dB/oct resonant lowpass VCF
// References: CSound source code, Stilson/Smith CCRMA paper.
// Modified by paul.kellett@maxim.abel.co.uk July 2000

#include <math.h>
#include "ifilter_mg4_v1.h"

#define UNITY (8388608)
#define SAT_LIM (10*UNITY)

/*
 * S8.23 mult
 */
inline int32_t s823mult(int32_t x, int32_t y)
{
	int64_t result;
	
	result = (int64_t)x * (int64_t)y;
	//result += (1<<22);
	result >>= 23;
	//result = result > 0x7fffffff ? 0x7fffffff : result;
	//result = result < -0x8000000 ? -0x8000000 : result;
	return result;
}

/*
 * init_mg4 - initialize the filter state
 */
void init_ifilter_mg4(ifmg4_state *f)
{
	set_ifilter_mg4(f, 0.5F, 0.0F, 1);
	f->b0 = 0;
	f->b1 = 0;
	f->b2 = 0;
	f->b3 = 0;
	f->b4 = 0;
	f->t1 = 0;
	f->t2 = 0;
	f->gain = 0x007fffff;
	f->bypass = 0;
}

/*
 * set_filter - set filter parameters & precalculate some values.
 * 
 * fc = cutoff, nearly linear [0,1] -> [0, fs/2]
 * res = resonance [0, 1] -> [no resonance, self-oscillation]
 * bypass = mode (0 = lowpass, 1 = bypass, 2 = highpass, 3 = bandpass)
 */
void set_ifilter_mg4(ifmg4_state *f, int16_t fc, int16_t res, uint8_t bypass)
{
	int32_t ifc, ires;
	
	/* convert float inputs to int */
	ifc = fc<<8;
	ires = res<<8;
	
	// Set coefficients given frequency & resonance [0.0...1.0]
	f->q = UNITY - ifc;
	f->p = ifc + s823mult(s823mult((0.8F*(float)UNITY), ifc), f->q);
	f->f = f->p + f->p - UNITY;
	f->q = s823mult(ires, (UNITY + (s823mult(f->q, UNITY - f->q + 
		s823mult((5.6F*(float)UNITY), s823mult(f->q, f->q)))>>1)));
	f->gain = UNITY + ires + s823mult(ires<<1, UNITY-ifc);

	// Bypass - explicitly, or if cutoff is > 90%
	f->bypass = bypass;
	if(ifc > (0.9F*(float)UNITY))
	{
		if(bypass <= 1)
			f->bypass = 1;	// bypassed
		else
			f->bypass = 4;	// disabled
	}
}

/*
 * duplicate filter settings
 */
void dupe_ifilter_mg4(ifmg4_state *f1, ifmg4_state *f2)
{
	f2->q = f1->q;
	f2->p = f1->p;
	f2->f = f1->f;
	f2->gain = f1->gain;
	f2->bypass = f1->bypass;
}

/*
 * filter_mg4 - run the filter
 */
int16_t ifilter_mg4(ifmg4_state *f, int16_t input)
{
	int32_t in, out;
	
	/* quick return if bypassed */
	if(f->bypass == 1)
		return input;
	
	/* quick return if disabled */
	if(f->bypass == 4)
		return 0;
	
	/* convert to S8.23 */
	in = (int32_t)input<<8;
	
	/* Filter */
	in -= s823mult(f->q, f->b4);			// feedback
	f->t1 = f->b1;
	f->b1 = s823mult(in + f->b0, f->p) - s823mult(f->b1, f->f);
	f->t2 = f->b2;
	f->b2 = s823mult(f->b1 + f->t1, f->p) - s823mult(f->b2, f->f);
	f->t1 = f->b3;
	f->b3 = s823mult(f->b2 + f->t2, f->p) - s823mult(f->b3, f->f);
	f->b4 = s823mult(f->b3 + f->t1, f->p) - s823mult(f->b4, f->f);
	f->b4 = f->b4 - s823mult(s823mult(s823mult(f->b4, f->b4), f->b4), (0.166667F*(float)UNITY));	// clipping
	f->b0 = in;
	
	/* saturate feedback to prevent overflow & NaN */
	f->b4 = f->b4 >  SAT_LIM ?  SAT_LIM : f->b4;
	f->b4 = f->b4 < -SAT_LIM ? -SAT_LIM : f->b4;
	
	/* select output */
	switch(f->bypass)
	{
		case 0: // Lowpass  output:  b4
			out = s823mult(f->gain, f->b4);
			break;
		
		default:
		case 1: // bypass - shouldn't get here
			out = in;
			break;
		
		case 2: // Highpass output:  in - b4;
			out = in - s823mult(f->gain, f->b4);
			break;
		
		case 3: // Bandpass output:  3.0f * (b3 - b4);
			out = s823mult(f->gain, (3 * (f->b3-f->b4)));
			break;
	}

	/* convert output back to int16 */
	int32_t sat = (out + 128) >> 8;
	return dsp_ssat16(sat);
}


