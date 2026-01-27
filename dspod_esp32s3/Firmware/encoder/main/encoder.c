/*
 * encoder.c - handle the encoder
 * 01-19-26 E. Brombaugh
 */

#include <stdio.h>
#include "main.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "debounce.h"
#include "encoder.h"

/*
 * Button Connections for dspod esp32s3 & module
 *
 * btn1  GPIO36	NC
 * btn2  GPIO35	TAP
 * btn3  GPIO34	ENC E
 * btn4  GPIO33	ENC B
 * btn5  GPIO47	ENC A
 *
 */
 
/* hookups for buttons */
#define ENC_A_PIN 47
#define ENC_B_PIN 33
#define ENC_E_PIN 34
#define TAP_PIN 35

volatile int16_t enc_val;
debounce_state btn_dbs[NUM_DBS];
volatile uint8_t btn_fe[NUM_DBS], btn_re[NUM_DBS];

const uint32_t btn_pins[NUM_DBS] = 
{
	ENC_A_PIN,
	ENC_B_PIN,
	ENC_E_PIN,
	TAP_PIN,
};

const uint8_t btn_db_lens[NUM_DBS] =
{
	2,
	2,
	15,
	15,
};

const uint8_t btn_state_act[NUM_DBS] =
{
	0,	// all buttons active low
	0,
	0,
	0,
};

uint8_t btn_state_raw[NUM_DBS];

/*
 * encoder scanning callback
 */
void encoder_timer_callback(void *arg)
{
	uint8_t i;

	/* gather inputs and run all debouncers */
	for(i=0;i<NUM_DBS;i++)
	{
		btn_state_raw[i] = gpio_get_level(btn_pins[i]);
		debounce(&btn_dbs[i], (btn_state_raw[i] == btn_state_act[i]));
		btn_fe[i] |= btn_dbs[i].fe;
		btn_re[i] |= btn_dbs[i].re;
	}
	
	/* decode encoder phases */
	int16_t enc_del = 0;
	/* equivalent to linux "half-step" method (2 transitions/step) */
    if(btn_dbs[ENC_B].re)
	{
        if(btn_dbs[ENC_A].state == 1)
            enc_del++;
        else
            enc_del--;
    }
    else if(btn_dbs[ENC_B].fe)
    {
        if(btn_dbs[ENC_A].state == 0)
            enc_del++;
        else
            enc_del--;
    }
	enc_val += enc_del;
}

/*
 * init encoder scanning
 */
esp_err_t encoder_init(void)
{
	int i;
	gpio_config_t io_conf = {
		.intr_type = GPIO_INTR_DISABLE,
		.mode = GPIO_MODE_INPUT,
		.pin_bit_mask = 0,
		.pull_down_en = 0,
		.pull_up_en = 1
	};

	for(i=0;i<NUM_DBS;i++)
	{
		/* configure GPIO inputs for encoder & buttons */
		io_conf.pin_bit_mask = (1LLU<<btn_pins[i]);
		gpio_config(&io_conf);
		
		/* init debouncers */
		init_debounce(&btn_dbs[i], btn_db_lens[i]);
		btn_fe[i] = 0;
		btn_re[i] = 0;
	}
	
	enc_val = 0;
	
	/* start 1ms timer */
	esp_timer_handle_t timer_handle;
	esp_timer_create_args_t timer_args = {
		.callback = encoder_timer_callback,
		.arg = NULL,
		.name = "encoder_timer",
		.skip_unhandled_events = true
	};
	esp_timer_create(&timer_args, &timer_handle);
	esp_timer_start_periodic(timer_handle, 1000);
	
	return ESP_OK;
}

/*
 * check status of buttons
 */
uint8_t encoder_get_button(uint8_t btn)
{
	return btn_dbs[btn].state;
}

/*
 * check for falling edge of buttons
 */
uint8_t encoder_btn_fe(uint8_t btn)
{
	uint8_t result = btn_fe[btn];
	btn_fe[btn] = 0;
	return result;
}

/*
 * check for rising edge of buttons
 */
uint8_t encoder_btn_re(uint8_t btn)
{
	uint8_t result = btn_re[btn];
	btn_re[btn] = 0;
	return result;
}

/*
 * check for rising edge of buttons
 */
uint8_t encoder_btn_raw(uint8_t btn)
{
	return btn_state_raw[btn];
}

/*
 * get state of encoder shaft
 */
int16_t encoder_get_enc(void)
{
	int16_t result = enc_val;
	enc_val = 0;
	return result;
}

/*
 * wrapper for encoder + button menuing
 */
uint8_t encoder_poll(int16_t *rtn_enc_val, uint8_t *rtn_enc_btn)
{
	uint8_t result = 0;
	
	*rtn_enc_val = 0;
	*rtn_enc_btn = 0;
	
	if(enc_val != 0)
	{
		result++;
		*rtn_enc_val = enc_val;
		enc_val = 0;
	}
	
	if(btn_fe[ENC_E])
	{
		result++;
		*rtn_enc_btn = 1;
		btn_fe[ENC_E] = 0;
	}
	
	return result;
}

