/*
 * cyclesleep.h - zyp's cycle counter sleep routines
 * 09-05-15 E. Brombaugh - updated for F7 and HAL
 * 03-20-17 E. Brombaugh - fixed wrap bug, update comments
 * 11-08-24 E. Brombaugh - updated for H5
 * 01-28-25 E. Brombaugh - updated for RP2350
 */

#ifndef __cyclesleep__
#define __cyclesleep__

#include "pico/stdlib.h"

void cyccnt_enable(void);
void cyclesleep(uint32_t cycles);
uint32_t cyclegoal(uint32_t cycles);
uint32_t cyclegoal_ms(uint32_t ms);
uint32_t cyclecheck(uint32_t goal);
void cyclesleep_delay(uint32_t ms);
void start_meas(void);
void end_meas(void);
void get_meas(uint32_t *act, uint32_t *tot);

#endif
