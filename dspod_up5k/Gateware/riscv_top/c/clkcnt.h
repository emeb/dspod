/*
 * clkcnt.h - clock cycle counter driver
 * 07-03-19 E. Brombaugh
 */

#ifndef __clkcnt__
#define __clkcnt__

#include "main.h"

#define CLKCNT_FREQ 24000000

uint32_t clkcnt_goal(uint32_t clks);
uint32_t clkcnt_goal_ms(uint32_t ms);
uint32_t clkcnt_check(uint32_t goal);
void clkcnt_wait(uint32_t clks);
void clkcnt_delayms(uint32_t ms);

#endif

