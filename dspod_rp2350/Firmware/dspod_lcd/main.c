/*
 * main.c - top level of dspod_lcd project
 * 08-11-25 E. Brombaugh
 */

#include <stdio.h>
#include "main.h"
#include "pico/unique_id.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"
#include "hardware/structs/xip.h"
#include "cyclesleep.h"
#include "gfx.h"
#include "st7789.h"

/* build version in simple format */
const char *fwVersionStr = "V0.1";

/* build time */
const char *bdate = __DATE__;
const char *btime = __TIME__;

#define PICO_DEFAULT_LED_PIN 25
#ifndef LED_DELAY_MS
#define LED_DELAY_MS 50
#endif

// Initialize the GPIO for the LED
void pico_led_init(void) {
#ifdef PICO_DEFAULT_LED_PIN
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#endif
}

// Turn the LED on or off
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    // Just set the GPIO on or off
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#endif
}

int main() {
	int32_t i;
	pico_unique_board_id_t id_out;
	
	/* init the diagnostic port */
	stdio_init_all();
	sleep_ms(500);	// let USB serial on host wake up
	printf("\n\nDSPOD LCD %s starting\n\r", fwVersionStr);
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
#if 0
	printf("Test offsets.\n");
	gfx_drawline(0, 0, 319, 169);
	gfx_drawline(319, 0, 0, 169);
	gfx_drawstr(0, 0, "0, 0");
	gfx_drawstr(160, 85, "160, 85");
	gfx_drawstr(255, 161, "255, 161");
#else
	printf("Rounded rects\n");
	GFX_RECT rect = {2,2,317,167};
	gfx_fillroundedrect(&rect, 20);
	
	rect.x0 = 50;
	rect.y0 = 50;
	rect.x1 = 150;
	rect.y1 = 150;
	gfx_set_forecolor(GFX_CYAN);
	gfx_fillroundedrect(&rect, 20);
	
	rect.x0 = 200;
	rect.y0 = 20;
	rect.x1 = 250;
	rect.y1 = 160;
	gfx_set_forecolor(GFX_MAGENTA);
	gfx_fillroundedrect(&rect, 50);
	
	rect.x0 = 20;
	rect.y0 = 10;
	rect.x1 = 170;
	rect.y1 = 40;
	gfx_set_forecolor(GFX_BLUE);
	gfx_fillroundedrect(&rect, 5);
#endif
	ST7789_setBacklight(1);
	printf("GFX Initialized\n");

	/* blink the LED */
    pico_led_init();
	printf("LED initialized\n\r");

	printf("Looping...\n\r");
    while (true) {
		/* blink led */
        pico_set_led(true);
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        sleep_ms(LED_DELAY_MS);
    }
}
