/*
 * main.c - top level of dspod_test project
 * 08-11-25 E. Brombaugh
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
	printf("\n\nDSPOD Test %s starting\n\r", fwVersionStr);
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

	/* Init LCD */
	gfx_init(&ST7789_drvr);
#if 1
	printf("Test offsets.\n");
	gfx_drawline(0, 0, 319, 169);
	gfx_drawline(319, 0, 0, 169);
	gfx_drawstr(0, 0, "0, 0");
	gfx_drawstr(160, 85, "160, 85");
	gfx_drawstr(255, 161, "255, 161");
#else
	printf("Bounding boxes\n");
	GFX_RECT rect = {0,0,319,169};
	gfx_drawrect(&rect);
#endif
	ST7789_setBacklight(1);
	printf("GFX Initialized\n");

#if 1
	/* codec */
	printf("NAU88C22 Codec init %s\n", NAU88C22_Init() ? "failed" : "passed");
	
#if 1
	/* init Audio */
	init_i2s_fulldup();
	printf("I2S Initialized\n");
#else
	printf("dma_hw->ints0 = 0x%08X\n\r", &(dma_hw->ints0));
	printf("dma_hw->irq_ctrl[0].ints = 0x%08X\n\r", &(dma_hw->irq_ctrl[0].ints));
#endif
#endif

	/* blink the LED */
    led_init();
	printf("LED initialized\n\r");

	/* start ADC polling in background */
	ADC_init();
	printf("LED initialized\n\r");
	
	/* button input */
	button_init();
	printf("Button Initialized\n");
	
	/* Gate Output */
	gate_init();
	printf("Gate Initialized\n");
	
	printf("Looping...\n\r");
    while (true) {
		/* blink led */
        set_led(true);
		set_gate(true);
        sleep_ms(LED_DELAY_MS);
        set_led(false);
		set_gate(false);
        sleep_ms(LED_DELAY_MS);
		
		/* measure audio duty cycle */
		//printf("CPU load: %6.2f %%\r", i2s_cpu_load());
		
		/* report ADC values */
		//printf("%5d %5d %5d %5d\r", ADC_val[0], ADC_val[1], ADC_val[2], ADC_val[3]);
		
		/* report button values */
		//printf("%1d %1d %1d %1d %1d %1d\r", button_get(BTN_UNUSED), button_get(BTN_TAP), button_get(BTN_ENC), button_get(BTN_PHSB), button_get(BTN_PHSA), button_get(BTN_GATEIN));
		
		/* report endocer changes */
		int16_t enc = button_encoder();
		if(enc)
			printf("%5d\r", enc);
    }
}
