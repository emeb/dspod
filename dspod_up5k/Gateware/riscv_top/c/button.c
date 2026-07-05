/*
 * button.c - dspod_up5k button setup
 * 06-16-26 E. Brombaugh
 */

#include "button.h"
#include "debounce.h"

#define BTN_ENC_A_PIN 4
#define BTN_ENC_B_PIN 5
#define BTN_ENC_E_PIN 6
#define BTN_TAP_PIN 7
#define BTN_BOOT_PIN 8

/* choose hardware of firmware debounce */
#define HW_DEBOUNCE

static const uint32_t btn_pin[NUM_BTNS] =
{
	BTN_ENC_A_PIN,
	BTN_ENC_B_PIN,
	BTN_ENC_E_PIN,
	BTN_TAP_PIN,
	BTN_BOOT_PIN
};

#ifndef HW_DEBOUNCE
debounce_state btn_db[NUM_BTNS];
#else
uint8_t btn_state[NUM_BTNS];
#endif
uint8_t btn_raw[NUM_BTNS];
uint8_t btn_fe[NUM_BTNS], btn_re[NUM_BTNS];
int16_t enc_val;

/*
 * Initialize the button handler
 */
uint8_t Button_Init(void)
{
	/* init the debouncer and button state */
	for(uint8_t i=0;i<NUM_BTNS;i++)
	{
#ifndef HW_DEBOUNCE
		init_debounce(&btn_db[i], 15);
#endif
		btn_fe[i] = 0;
		btn_re[i] = 0;
	}
	
    enc_val = 0;
	
	/* reset HW decoder */
	gp_out |= GP_OUT_ENCVCLR;
	gp_out &= ~GP_OUT_ENCVCLR;
	
	return 0;
}

/*
 * get current button status
 */
uint8_t Button_state(btn_type_t btn)
{
#ifndef HW_DEBOUNCE
	return btn_db[btn].state;
#else
	return btn_state[btn];
#endif
}

/*
 * check for falling edge of button
 */
uint8_t Button_fe(btn_type_t btn)
{
	uint8_t result = btn_fe[btn];
	btn_fe[btn] = 0;
	return result;
}

/*
 * check for rising edge of button
 */
uint8_t Button_re(btn_type_t btn)
{
	uint8_t result = btn_re[btn];
	btn_re[btn] = 0;
	return result;
}

/*
 * check for raw button
 */
uint8_t Button_raw(btn_type_t btn)
{
	return btn_raw[btn];
}

/*
 * get state of encoder shaft
 */
int16_t Button_get_enc(void)
{
	int16_t result = enc_val;
	enc_val = 0;
	return result;
}

/*
 * wrapper for encoder + button
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
	
	if(btn_fe[BTN_ENC_E])
	{
		result++;
		*rtn_enc_btn = 1;
		btn_fe[BTN_ENC_E] = 0;
	}
	
	return result;
}

/*
 * Button input processing at Systick rate
 */
void Button_Handler(void)
{
#ifndef HW_DEBOUNCE
	/* debounce everything */
	for(uint8_t i=0;i<NUM_BTNS;i++)
	{
		btn_raw[i] = (gp_in >> btn_pin[i]) & 1;
		debounce(&btn_db[i], (btn_raw[i] == 0));
		btn_fe[i] |= btn_db[i].fe;
		btn_re[i] |= btn_db[i].re;
	}
	
	/* decode encoder phases */
	int16_t enc_del = 0;
	/* equivalent to linux "half-step" method (2 transitions/step) */
    if(btn_db[BTN_ENC_B].re)
	{
        if(btn_db[BTN_ENC_A].state == 1)
            enc_del++;
        else
            enc_del--;
    }
    else if(btn_db[BTN_ENC_B].fe)
    {
        if(btn_db[BTN_ENC_A].state == 0)
            enc_del++;
        else
            enc_del--;
    }
	enc_val += enc_del;
#else
	/* get current button inputs */
	uint32_t button_in = gp_in;
	
	/* reset HW decoder */
	gp_out |= GP_OUT_ENCVCLR;
	gp_out &= ~GP_OUT_ENCVCLR;
	
	/* parse */
	for(uint8_t i=0;i<NUM_BTNS;i++)
	{
		/* parse raw values */
		btn_raw[i] = (button_in >> btn_pin[i]) & 1;
		
		/* parse debounced values & check edges */
		uint8_t state = (button_in >> (i+GP_IN_BUTTON_DEBOUNCE_POS)) & 1;
		btn_fe[i] |= btn_state[i] & ~state;	// falling
		btn_re[i] |= ~btn_state[i] & state;	// rising
		btn_state[i] = state;
	}
	
	/* update encoder */
	enc_val += (int8_t)((button_in >> GP_IN_BUTTON_ENCVAL_POS) & GP_IN_BUTTON_ENCVAL_MASK);
#endif
}
