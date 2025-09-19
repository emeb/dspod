/*
 * led.h - led driver for dspod
 * 08-28-25 E. Brombaugh
 */
#ifndef __led__
#define __led__

#include "main.h"

#define PICO_DEFAULT_LED_PIN 25

void led_init(void);
void set_led(bool led_on);

#endif
