/*
 * int_math.c - integer math operations. 
 * 08-14-26 E. Brombaugh
 */
 
#include "int_math.h"

/*
 * integer sqrt
 */
int int_sqrt(int x)
{
	uint32_t op = x, res = 0, one = 1;
	
	/* one is largest power of 4 less than input */
	while(one <= op)
		one <<= 2;
	one >>= 2;
	
	/* iterate */
	while(one != 0)
	{
		uint32_t dltasqr = res + one;
		if(op >= dltasqr)
		{
			op -= dltasqr;
			res += one << 1;
		}
		res >>= 1;
		one >>= 2;
	}
	
	return res;
}

/*
 * stuff for atan2()
 */
#define CORDIC_NTAB 12
const int cordic_ctab [] = {0x3243F6A8, 0x1DAC6705, 0x0FADBAFC, 0x07F56EA6,
						0x03FEAB76, 0x01FFD55B, 0x00FFFAAA, 0x007FFF55,
						0x003FFFEA, 0x001FFFFD, 0x000FFFFF, 0x0007FFFF};
						
/*
 * integer atan2
 * output scaled to fit into int16_t with INT_2PI = 0x6488 = 25736
 */
int int_atan2(int y, int x)
{
	/* detect which quadrant - 0,3 need no correction */
	int quad = 0;
	if((x < 0))
	{
		x = -x;
		if(y<0)
			quad = 2;
		else
			quad = 1;
	}

	/* CORDIC iteration driving y towards zero, accumulating angle */
	int k, d, tx, ty, tz;
	int z=0, n=CORDIC_NTAB;
	for (k=0; k<n; ++k)
	{
		d = y<=0 ? 0 : -1;
		tx = x - (((y>>k) ^ d) - d);
		ty = y + (((x>>k) ^ d) - d);
		tz = z - ((cordic_ctab[k] ^ d) - d);
		x = tx; y = ty; z = tz;
	}
	
	/* scale angle down so half-pi = pi */
	z>>=1;
	
	/* apply correction for quadrants 1,2 */
	if(quad == 1)
		z = half_pi - z;
	else if(quad == 2)
		z = -half_pi - z;
	
	/* scale angle so full circle fits into 16-bits */
	return z>>17;
}

