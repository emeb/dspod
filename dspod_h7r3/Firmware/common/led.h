/*
 * led.h - dspod_h7r3 LED setup
 * 10-14-25 E. Brombaugh
 */

#ifndef __led__
#define __led__

#include "stm32h7rsxx_hal.h"

void LEDInit(void);
void LEDOn(void);
void LEDOff(void);
void LEDToggle(void);

#endif
