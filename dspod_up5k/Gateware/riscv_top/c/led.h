/*
 * led.h - dspod_up5k LED setup
 * 06-18-26 E. Brombaugh
 */

#ifndef __led__
#define __led__

#include "main.h"

enum {
	LED_RED = 0x4,
	LED_GREEN = 0x2,
	LED_BLUE = 0x1
} led_enum;

void LED_Init(void);
void LED_On(uint8_t led);
void LED_Off(uint8_t led);
void LED_Set(uint8_t led);
void LED_Toggle(uint8_t led);

#endif
