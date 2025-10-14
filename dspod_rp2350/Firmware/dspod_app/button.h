/*
 * button.h - user input button for RP2040
 * 12-19-21 E. Brombaugh
 */

#ifndef __button__
#define __button__

#include "main.h"

#define NUM_BTNS 6

enum buttons
{
	BTN_UNUSED,
	BTN_TAP,
	BTN_ENC,
	BTN_PHSB,
	BTN_PHSA,
	BTN_GATEIN,
};

void button_init(void);
uint8_t button_get(uint8_t idx);
uint8_t button_fe(uint8_t idx);
uint8_t button_re(uint8_t idx);
int16_t button_encoder(void);
uint8_t encoder_poll(int16_t *rtn_enc_val, uint8_t *rtn_enc_btn);

#endif
