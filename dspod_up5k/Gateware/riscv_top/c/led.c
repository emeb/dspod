/*
 * led.c - dspod_up5k LED setup
 * 06-18-26 E. Brombaugh
 */
 
#include "led.h"

/*
 * Set LEDs to initial state all off
 */
void LED_Init(void)
{
	LED_Off(LED_RED | LED_GREEN | LED_BLUE);
}

/*
 * Turn on selected LEDs, others unchanged
 */
void LED_On(uint8_t led)
{
	gp_out = gp_out | ((led&7)<<17);
}

/*
 * Turn off selected LEDs, others unchanged
 */
void LED_Off(uint8_t led)
{
	gp_out = gp_out & ~((led&7)<<17);
}

/*
 * Set selected LEDs on, others off 
 */
void LED_Set(uint8_t led)
{
	gp_out = (gp_out & ~(7<<17)) | ((led&7)<<17);
}

/*
 * Flip selected LED state
 */
void LED_Toggle(uint8_t led)
{
	gp_out = gp_out ^ ((led&7)<<17);
}

