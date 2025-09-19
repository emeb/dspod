/*
 * button.c - user input button for RP2040
 * 12-19-21 E. Brombaugh
 */

#include <stdio.h>
#include "debounce.h"
#include "button.h"

#define BUTTON_PIN 14

debounce_state btn_dbs[NUM_BTNS];
uint8_t btn_fe[NUM_BTNS], btn_re[NUM_BTNS];
struct repeating_timer btn_rt;
uint8_t btn_idxs[NUM_BTNS] =
{
	9,		// unused on dspod_module
	10,		// tap button
	11,		// encoder button
	12,		// encoder phase B
	13,		// encoder phase A
	14,		// gate input
};
int16_t enc_val;

/*
 * button scanning callback
 */
bool button_callback(repeating_timer_t *rt)
{
	/* debounce all inputs */
	for(int i=0;i<NUM_BTNS;i++)
	{
		debounce(&btn_dbs[i], (!gpio_get(btn_idxs[i])));
		btn_fe[i] |= btn_dbs[i].fe;
		btn_re[i] |= btn_dbs[i].re;
	}
	
	/* decode phases - only one edge allowed per */
	int16_t enc_del = 0;
	if(btn_dbs[BTN_PHSA].re)
	{
        if(btn_dbs[BTN_PHSB].state == 0)
            enc_del++;
        else
            enc_del--;
    }
    else if(btn_dbs[BTN_PHSA].fe)
    {
        if(btn_dbs[BTN_PHSB].state == 1)
            enc_del++;
        else
            enc_del--;
    }
    else if(btn_dbs[BTN_PHSB].re)
	{
        if(btn_dbs[BTN_PHSA].state == 1)
            enc_del++;
        else
            enc_del--;
    }
    else if(btn_dbs[BTN_PHSB].fe)
    {
        if(btn_dbs[BTN_PHSA].state == 0)
            enc_del++;
        else
            enc_del--;
    }
	
#if 0
	/* check for changes & timing */
	if(enc_del)
	{
		/* accelerate based on timing */
		if(enc_timestamp < 20)
			enc_del <<= 3;
		else if(enc_timestamp < 50)
			enc_del <<= 2;
		else if(enc_timestamp < 100)
			enc_del <<= 1;
		
		enc_val += enc_del;
		enc_timestamp = 0;
	}
	else
		enc_timestamp++;
#else
	enc_val += enc_del;
#endif

    return true;
}

/*
 * init button scanning
 */
void button_init(void)
{
	for(int i=0;i<NUM_BTNS;i++)
	{
		/* button is input w/ pullup */
		gpio_init(btn_idxs[i]);
		gpio_set_dir(btn_idxs[i], GPIO_IN);
		gpio_set_pulls(btn_idxs[i], 1, 0);
	
		/* init debounce */
		init_debounce(&btn_dbs[i], 15);
		btn_fe[i] = 0;
		btn_re[i] = 0;
	}
	
	/* encoder */
	enc_val = 0;
	
	/* start 1ms button timer */
	add_repeating_timer_ms(
		1,	// 1ms
		button_callback,
		NULL,
		&btn_rt);
}

/*
 * check status of button
 */
uint8_t button_get(uint8_t idx)
{
	return btn_dbs[idx].state;
}

/*
 * check for falling edge of button
 */
uint8_t button_fe(uint8_t idx)
{
	uint8_t result = btn_fe[idx];
	btn_fe[idx] = 0;
	return result;
}

/*
 * check for rising edge of button
 */
uint8_t button_re(uint8_t idx)
{
	uint8_t result = btn_re[idx];
	btn_re[idx] = 0;
	return result;
}

/*
 * get state of encoder shaft
 */
int16_t button_encoder(void)
{
	int16_t result = enc_val;
	enc_val = 0;
	return result;
}
