/*
 * main.c - top level of dspod_app project
 * 10-06-25 E. Brombaugh
 */

#include "main.h"
#include "pico/unique_id.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"
#include "hardware/structs/xip.h"
#include "hardware/dma.h"
#include "led.h"
#include "cyclesleep.h"
#include "i2s_fulldup.h"
#include "nau88c22.h"
#include "gfx.h"
#include "st7789.h"
#include "adc.h"
#include "button.h"
#include "gateio.h"
#include "menu.h"
#include "audio.h"

/* build version in simple format */
const char *fwVersionStr = "V0.1";

/* build time */
const char *bdate = __DATE__;
const char *btime = __TIME__;

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 50
#endif

int main() {
	int32_t i;
	pico_unique_board_id_t id_out;
	
	/* init the diagnostic port */
	stdio_init_all();
	sleep_ms(500);	// let USB serial on host wake up
	printf("\n\nDSPOD App %s starting\n\r", fwVersionStr);
	printf("CHIP_ID: 0x%08X\n\r", *(volatile uint32_t *)(SYSINFO_BASE));
	printf("BOARD_ID: 0x");
	pico_get_unique_board_id(&id_out);
	for(i=0;i<PICO_UNIQUE_BOARD_ID_SIZE_BYTES;i++)
		printf("%02X", id_out.id[i]);
	printf("\n");
	printf("clk_sys = %dkHz\n", frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS));
	printf("Build Date: %s\n\r", bdate);
	printf("Build Time: %s\n\r", btime);
	printf("\n");
	
	/* cyclesleep */
	cyccnt_enable();
	printf("Started 0 cycle counter\n\r");

	/* blink the LED */
    led_init();
	printf("LED initialized\n\r");

	/* Init LCD */
	gfx_init(&ST7789_drvr);
	menu_splash(fwVersionStr, bdate, btime);
	ST7789_setBacklight(1);
	printf("GFX Initialized\n");

	/* button input */
	button_init();
	printf("Button Initialized\n");
	
	/* Gate Output */
	gate_init();
	printf("Gate Initialized\n");
	
	/* start ADC polling in background */
	ADC_init();
	printf("ADC initialized\n\r");
	
	/* codec */
	printf("NAU88C22 Codec init %s\n", NAU88C22_Init() ? "failed" : "passed");
	
	/* init Audio */
	init_i2s_fulldup();
	printf("I2S Initialized\n");
	
	/* unmute */
	Audio_Mute(0);

	/* let splash linger a bit then init the menu system */
	sleep_ms(3000);
	menu_init();
	printf("Menu intialized.\n");

	printf("Looping...\n\r");
	bool led_state = false;
	uint32_t led_goal = cyclegoal_ms(250);
	uint32_t menu_goal = cyclegoal_ms(33);
    while (true)
	{
		if(!cyclecheck(led_goal))
		{
			/* toggle LED */
			set_led(led_state);
			led_state = !led_state;
			
			/* report button values */
			//printf("%1d %1d %1d %1d %1d %1d\r", button_get(BTN_UNUSED), button_get(BTN_TAP), button_get(BTN_ENC), button_get(BTN_PHSB), button_get(BTN_PHSA), button_get(BTN_GATEIN));
			
			led_goal = cyclegoal_ms(250);
		}
		
		if(!cyclecheck(menu_goal))
		{
			/* handle menu */
			menu_process();
			
			menu_goal = cyclegoal_ms(33);
		}
    }
}
