/*
 * button.h - dspod_up5k button setup
 * 06-16-26 E. Brombaugh
 */

#ifndef __button__
#define __button__

#include "main.h"

#define NUM_BTNS 5

typedef enum
{
	BTN_ENC_A,
	BTN_ENC_B,
	BTN_ENC_E,
	BTN_TAP,
	BTN_BOOT
} btn_type_t;

uint8_t Button_Init(void);
uint8_t Button_state(btn_type_t btn);
uint8_t Button_fe(btn_type_t btn);
uint8_t Button_re(btn_type_t btn);
uint8_t Button_raw(btn_type_t btn);
int16_t Button_get_enc(void);
uint8_t encoder_poll(int16_t *rtn_enc_val, uint8_t *rtn_enc_btn);
void Button_Handler(void);

#endif
