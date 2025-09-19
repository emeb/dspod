/*
 * cyclesleep.c - zyp's cycle counter sleep routines
 * 09-05-15 E. Brombaugh - updated for F7 and HAL
 * 03-20-17 E. Brombaugh - fixed wrap bug, update comments
 * 11-08-24 E. Brombaugh - updated for H5
 * 01-28-25 E. Brombaugh - updated for RP2350
 */
 
#include "cyclesleep.h"
#include "hardware/clocks.h"
#include "hardware/regs/m33.h"
#include "hardware/structs/clocks.h"
#include "hardware/structs/m33.h"

uint32_t DelayCyc1s; 
uint32_t s_tot, act_cyc, tot_cyc;

/*
 * turn on cycle counter
 */
void cyccnt_enable()
{
    /* Enable TRC */
	m33_hw->demcr |=  M33_DEMCR_TRCENA_BITS;
	
    /* Enable cycle counter */
	m33_hw->dwt_ctrl |=  M33_DWT_CTRL_CYCCNTENA_BITS;
	
	/* get sysclk freq for computing delay in khz*/
	DelayCyc1s = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
}

/*
 * compute goal for cycle counter based on desired delay in cycles
 */
uint32_t cyclegoal(uint32_t cycles)
{
	return cycles + m33_hw->dwt_cyccnt;
}

/*
 * compute goal for cycle counter based on desired delay in milliseconds
 */
uint32_t cyclegoal_ms(uint32_t ms)
{
	return ms*DelayCyc1s + m33_hw->dwt_cyccnt;
}

/*
 * return TRUE if goal is reached
 */
uint32_t cyclecheck(uint32_t goal)
{
    /**************************************************/
    /* DANGER WILL ROBINSON!                          */
    /* the following syntax is CRUCIAL to ensuring    */
    /* that this test doesn't have a wrap bug         */
    /**************************************************/
	return (((int32_t)m33_hw->dwt_cyccnt - (int32_t)goal) < 0);
}

/*
 * sleep for a certain number of cycles
 */
void cyclesleep(uint32_t cycles)
{
    uint32_t goal = cyclegoal(cycles);
    
    while(cyclecheck(goal));
}

/*
 * sleep for a certain number of milliseconds
 */
void cyclesleep_delay(uint32_t ms)
{
	cyclesleep(ms*DelayCyc1s);
}

/*
 * called at start of routine to be measured
 */
void start_meas(void)
{
	/* grab current cycle count & measure total cycles */
	uint32_t curr_cyc = m33_hw->dwt_cyccnt;
	tot_cyc = curr_cyc - s_tot;
	s_tot = curr_cyc;

}

/*
 * called at end of routine to be measured
 */
void end_meas(void)
{
	/* grab current cycle count and measure active cycles */
	uint32_t curr_cyc = m33_hw->dwt_cyccnt;
	act_cyc = curr_cyc - s_tot;
}

/*
 * return the measurement results
 */
void get_meas(uint32_t *act, uint32_t *tot)
{
	*act = act_cyc;
	*tot = tot_cyc;
}
