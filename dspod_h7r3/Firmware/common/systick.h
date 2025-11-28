/*
 * systick.h - dspod h7r3 systick setup, encoder and button polling 
 * E. Brombaugh 11-14-2025
 */

#ifndef __systick__
#define __systick__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32h7rsxx_hal.h"

#define NUM_DBS 4

/* array indexes for debouncers */
enum buttons
{
	ENC_A,
	ENC_B,
	ENC_E,
	TAP,
};

void systick_init(void);
uint8_t systick_get_button(uint8_t btn);
uint8_t systick_button_fe(uint8_t btn);
uint8_t systick_button_re(uint8_t btn);
int16_t systick_get_enc(void);
uint8_t encoder_poll(int16_t *rtn_enc_val, uint8_t *rtn_enc_btn);

#ifdef __cplusplus
}
#endif

#endif
