/*
 * systick.h - H7 Audio prototype systick setup, encoder and button polling 
 * E. Brombaugh 07-07-2019
 */

#ifndef __systick__
#define __systick__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32h7rsxx_hal.h"

#define NUM_DBS 3

/* array indexes for debouncers */
enum buttons
{
	ENC_A,
	ENC_B,
	ENC_E,
};

void systick_init(void);
uint8_t systick_get_button(uint8_t btn);
uint8_t systick_button_fe(uint8_t btn);
uint8_t systick_button_re(uint8_t btn);
int16_t systick_get_enc(void);
void systick_pulse_led(uint8_t led, uint32_t duration);

#ifdef __cplusplus
}
#endif

#endif
