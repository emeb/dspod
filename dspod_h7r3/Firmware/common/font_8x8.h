/*
 * font8x8.h - font definitions for 8x8 character set
 * E. Brombaugh 03-24-2020
 */

#ifndef __font8x8__
#define __font8x8__

#include "stm32h7rsxx_hal.h"

enum font_special
{
	FONT_FLAG_A = 240,
	FONT_FLAG_A_ACT,
	FONT_FLAG_B,
	FONT_FLAG_B_ACT,
	FONT_FLAG_X,
	FONT_FLAG_X_ACT,
	FONT_FLAG_T,
	FONT_FLAG_T_ACT,
	FONT_SWP_YT,
	FONT_SWP_XY,
	FONT_TRG_TA,
	FONT_TRG_TN,
	FONT_PTR_UP,
	FONT_PTR_DN,
};

extern const unsigned char fontdata[];

#endif
