/*
 * int_math.h - integer math operations. 
 * 08-14-26 E. Brombaugh
 */

#ifndef __int_math__
#define __int_math__

#include <stdint.h>

#define half_pi 0x6487ED51
#define INT_2PI (half_pi>>16)

int int_sqrt(int x);
int int_atan2(int y, int x);

#endif
