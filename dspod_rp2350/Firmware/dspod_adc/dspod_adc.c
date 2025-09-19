/*
 * dspod_adc.c - test out 4 channel ADC in background
 *
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/unique_id.h"
#include "cyclesleep.h"
#include "adc.h"

/* build version in simple format */
const char *fwVersionStr = "V0.1";

/* build time */
const char *bdate = __DATE__;
const char *btime = __TIME__;

#define PICO_DEFAULT_LED_PIN 25
#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
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

// start here
int main() {
	int32_t i;
	pico_unique_board_id_t id_out;
	
	/* init the diagnostic port */
	stdio_init_all();
	sleep_ms(500);	// let USB serial on host wake up
	printf("\n\nDSPOD ADC %s starting\n\r", fwVersionStr);
	printf("CHIP_ID: 0x%08X\n\r", *(volatile uint32_t *)(SYSINFO_BASE));
	printf("BOARD_ID: 0x");
	pico_get_unique_board_id(&id_out);
	for(i=0;i<PICO_UNIQUE_BOARD_ID_SIZE_BYTES;i++)
		printf("%02X", id_out.id[i]);
	printf("\n");
	printf("Build Date: %s\n\r", bdate);
	printf("Build Time: %s\n\r", btime);
	printf("\n");
	
	/* cyclesleep */
	printf("Setting up cycle counter\n\r");
	cyccnt_enable();
	
	/* start ADC polling in background */
	ADC_init();

	/* blink the LED */
    pico_led_init();
	printf("LED initialized\n\r");

	printf("Looping...\n\r");
    while (true) {
        pico_set_led(true);
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        sleep_ms(LED_DELAY_MS);
		
		/* report ADC channels */
		printf("%5d %5d %5d %5d\r", ADC_val[0], ADC_val[1], ADC_val[2], ADC_val[3]);
    }
}
