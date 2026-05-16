/*
 * cyclesleep.h - zyp's cycle counter sleep routines
 * 09-05-15 E. Brombaugh - updated for F7 and HAL
 * 03-20-17 E. Brombaugh - fixed wrap bug, update comments
 * 11-08-24 E. Brombaugh - updated for H5
 * 04-02-26 E. Brombaugh - updated for U3
 */

#ifndef __cyclesleep__
#define __cyclesleep__

#include "main.h"

void cyccnt_enable(void);
void cyclesleep(uint32_t cycles);
uint32_t cyclegoal(uint32_t cycles);
uint32_t cyclegoal_ms(uint32_t ms);
uint32_t cyclecheck(uint32_t goal);
uint32_t cycle2us(uint32_t cycles);
void delay(uint32_t ms);
void delay_us(uint32_t us);
void start_meas(void);
void end_meas(void);
void get_meas(uint32_t *act, uint32_t *tot);

#endif
