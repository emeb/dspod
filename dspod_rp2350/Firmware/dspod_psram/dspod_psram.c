/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/unique_id.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"
#include "hardware/structs/xip.h"
#include "cyclesleep.h"

/* build version in simple format */
const char *fwVersionStr = "V0.1";

/* build time */
const char *bdate = __DATE__;
const char *btime = __TIME__;

#define PICO_DEFAULT_LED_PIN 25
#ifndef LED_DELAY_MS
#define LED_DELAY_MS 50
#endif

/*
 * Measure various clocks w/ internal counter
 */
void measure_freqs(void) {
	uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
	uint f_pll_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
	uint f_xosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_XOSC_CLKSRC);
	uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
	uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
	uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
	uint f_clk_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
	uint f_clk_adc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);
#ifdef CLOCKS_FC0_SRC_VALUE_CLK_RTC
	uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);
#endif

	printf("pll_sys = %dkHz\n", f_pll_sys);
	printf("pll_usb = %dkHz\n", f_pll_usb);
	printf("xosc = %dkHz\n", f_xosc);
	printf("rosc = %dkHz\n", f_rosc);
	printf("clk_sys = %dkHz\n", f_clk_sys);
	printf("clk_peri = %dkHz\n", f_clk_peri);
	printf("clk_usb = %dkHz\n", f_clk_usb);
	printf("clk_adc	= %dkHz\n", f_clk_adc);
#ifdef CLOCKS_FC0_SRC_VALUE_CLK_RTC
	printf("clk_rtc = %dkHz\n", f_clk_rtc);
#endif
}

// APS6404L has 8MB = 2^21 x 32-bits
#define MEMTEST_LEN (1<<21)

/*
 * enable and test QSPI PSRAM
 */
void test_psram(void)
{
    gpio_set_function(8, GPIO_FUNC_XIP_CS1); // CS for PSRAM on dspod
    xip_ctrl_hw->ctrl|=XIP_CTRL_WRITABLE_M1_BITS;
	printf("PSRAM CS enabled and set writable.\n\r");
	
	/* loop over words */
	printf("Writing 0x%08X words to PSRAM.\n\r", MEMTEST_LEN);
	uint32_t *psram = (uint32_t *)0x11000000;
	for(uint32_t cnt = 0;cnt < MEMTEST_LEN;cnt++)
	{
		psram[cnt] = cnt;
	}
	printf("Checking 0x%08X words from PSRAM.\n\r", MEMTEST_LEN);
	uint32_t errcnt = 0;
	for(uint32_t cnt = 0;cnt < MEMTEST_LEN;cnt++)
	{
		if(psram[cnt] != cnt)
			errcnt++;
	}
	printf("errcnt = %d.\n\r", errcnt);
}

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
	printf("\n\nDSPOD PSRAM %s starting\n\r", fwVersionStr);
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
	printf("CYCCNT = 0x%08X\n\r", cyclegoal(0));
	printf("CYCCNT = 0x%08X\n\r", cyclegoal(0));
	cyclesleep_delay(1000);
	printf("delayed 1s\n\r");

	/* clocks! */
	printf("Checking clock freqs...\n\r");
	measure_freqs();
	
	/* PSRAM! */
	printf("Testing PSRAM...\n\r");
	test_psram();
	
	/* blink the LED */
    pico_led_init();
	printf("LED initialized\n\r");

	printf("Looping...\n\r");
    while (true) {
        pico_set_led(true);
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        sleep_ms(LED_DELAY_MS);
    }
}
