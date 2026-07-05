/*
 * main.c - top level of picorv32 firmware for dspod_up5k running from RAM
 * 06-11-26 E. Brombaugh
 */
#include "main.h"
#include "acia.h"
#include "spi.h"
#include "flash.h"
#include "psram.h"
#include "clkcnt.h"
#include "led.h"
#include "st7789.h"
#include "i2c.h"
#include "codec.h"
#include "button.h"
#include "menu.h"
#include "si5351.h"

/* build time */
const char *bdate = __DATE__;
const char *btime = __TIME__;
const char *fwVersionStr = "V0.1";

uint8_t dest[32768] __attribute__ ((section (".ram2_data")));

/*
 * main
 */
void main()
{	
	init_printf(0, acia_printf_putc);
	printf("\n\n\r-----------------------------\n\r");
	printf("dspod_up5k riscv_top RAM - starting up\n\r");
    printf("Version: %s\n\r", fwVersionStr);
    printf("Build Date: %s\n\r", bdate);
    printf("Build Time: %s\n\r", btime);
	printf("-----------------------------\n\r");
			
	/* init LEDs */
	LED_Init();
	printf("LEDs initialized\n\r");

	/* init both SPI ports */
	spi_init(SPI0);
	spi_init(SPI1);
	printf("SPI ports initialized\n\r");
	
	/* set up LCD SPI & GPIO */
	gfx_init(&ST7789_drvr);
	gfx_clrscreen();
	menu_splash(fwVersionStr, bdate, btime);
	ST7789_backlight(1);
	printf("LCD & GFX initialized\n\r");
	
	/* Init I2C */
	i2c_init(I2C0);
	printf("I2C0 Initialized\n\r");

	/* init Si5351 */
// #define AUDIOCLK 12288000 	// for exact 24kHz
#define AUDIOCLK 12000000		// for Midiverb 23.4375kHz
	if(si5351_init() == 0)
	{
		si5351_set_output_chl(0, AUDIOCLK, 0);
		printf("SI5351 initialized to %d Hz\n\r", AUDIOCLK);
	}
	
	/* init codec */
	if(Codec_Init() == 0)
		printf("Codec initialized\n\r");
	else
		printf("Codec init failed!\n\r");

	/* init the button handler */
	Button_Init();
	printf("Button initialized\n\r");
	
	/* Menu */
	clkcnt_delayms(2000);
	menu_init();
	printf("Menu initialized\n\r");
	
	/* unmute */
	Codec_Mute(CODEC_MUTE_OFF);
	
	printf("Looping...\n\r");
	uint32_t button_goal = clkcnt_goal_ms(1);
	uint32_t menu_goal = clkcnt_goal_ms(33);
	uint32_t led_goal = clkcnt_goal_ms(250);
	uint8_t LED = 0;
	while(1)
	{
		/* debounce the button inputs */
		if(clkcnt_check(button_goal))
		{
			button_goal = clkcnt_goal_ms(1);
			gp_out = gp_out | (1<<28);	// Gate out high
			Button_Handler();
			gp_out = gp_out & ~(1<<28);	// Gate out low

		}
		
		/* Handle the UI */
		if(clkcnt_check(menu_goal))
		{
			menu_goal = clkcnt_goal_ms(33);
			menu_process();
		}
		
		/* blink the LED */
		if(clkcnt_check(led_goal))
		{
			led_goal = clkcnt_goal_ms(250);
			
			LED_Set(LED);
			LED++;
			//printf("raw: %03X ", gp_in & 0x1ff);
			//printf("dbnc: %02X ", (gp_in >> 9) & 0x1f);
			//printf("enc_val: %02X\r", (gp_in>>14)&0xff);
			//printf("LR: %5d RR: %5d LT: %5d RT: %5d\r", LVL->L_RX, LVL->R_RX, LVL->L_TX, LVL->R_TX);
		}
	}
}
