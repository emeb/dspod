/*
 * main.c - top level of dspod_i2s project
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
#include "hardware/dma.h"
#include "cyclesleep.h"
#include "i2s_fulldup.h"
#include "nau88c22.h"

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
	printf("\n\nDSPOD I2S %s starting\n\r", fwVersionStr);
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
		
		/* measure audio duty cycle */
		printf("CPU load: %6.2f %%\r", i2s_cpu_load());
    }
}
