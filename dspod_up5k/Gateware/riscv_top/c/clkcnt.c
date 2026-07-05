/*
 * clkcnt.h - clock cycle counter driver
 * 07-03-19 E. Brombaugh
 */

#include "clkcnt.h"

/*
 * compute goal for clock counter based on desired delay in clks
 */
uint32_t clkcnt_goal(uint32_t clks)
{
	return clks + clkcnt_reg;
}

/*
 * compute goal for cycle counter based on desired delay in milliseconds
 */
uint32_t clkcnt_goal_ms(uint32_t ms)
{
	int64_t inc = ms*(CLKCNT_FREQ/1000);
	int64_t cnt = clkcnt_reg;
	
	return cnt + inc;
}

/*
 * return TRUE if goal is reached
 */
uint32_t clkcnt_check(uint32_t goal)
{
    /**************************************************/
    /* DANGER WILL ROBINSON!                          */
    /* the following syntax is CRUCIAL to ensuring    */
    /* that this test doesn't have a wrap bug         */
    /**************************************************/
	return ((int32_t)(clkcnt_reg - goal) >= 0);
}

/*
 * delay for clocks
 */
void clkcnt_wait(uint32_t clks)
{
	uint32_t goal = clkcnt_goal(clks);
	while(!clkcnt_check(goal));
}

/*
 * delay for number of milliseconds
 */
void clkcnt_delayms(uint32_t ms)
{
	uint32_t goal = clkcnt_goal_ms(ms);
	while(!clkcnt_check(goal));
}
