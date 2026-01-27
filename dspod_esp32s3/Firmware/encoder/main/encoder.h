/*
 * encoder.h - handle the encoder
 * 01-19-26 E. Brombaugh
 */

#ifndef __encoder__
#define __encoder__

#define NUM_DBS 4

/* array indexes for debouncers */
enum buttons
{
	ENC_A,
	ENC_B,
	ENC_E,
	TAP,
};

esp_err_t encoder_init(void);
uint8_t encoder_get_button(uint8_t btn);
uint8_t encoder_button_fe(uint8_t btn);
uint8_t encoder_button_re(uint8_t btn);
uint8_t encoder_btn_raw(uint8_t btn);
int16_t encoder_get_enc(void);
uint8_t encoder_poll(int16_t *rtn_enc_val, uint8_t *rtn_enc_btn);

#endif
