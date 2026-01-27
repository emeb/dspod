/*
 * main.c - top level of dspod_esp32s3 i2s_codec test
 * 01-16-26 E. Brombaugh
 */

#include <stdio.h>
#include "main.h"
#include "gfx.h"
#include "st7789_drvr.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define LED_GPIO 18

/* tag for logging */
static const char *TAG = "lcd";

/* build version in simple format */
const char *fwVersionStr = "V0.1";

/* build time */
const char *bdate = __DATE__;
const char *btime = __TIME__;

/*
 * entry point
 */
void app_main(void)
{
	/* start logging of main app */
	printf("\n\ndspod_esp32s3 lcd %s starting\n\r", fwVersionStr);
	printf("Build Date: %s\n\r", bdate);
	printf("Build Time: %s\n\r", btime);
	printf("\n");
	
	/* init the LCD */
    ESP_LOGI(TAG, "Init GFX with ST7789");
	gfx_init(&st7789_drvr);
#if 0
#if 1
	/* offsets & colors */
	printf("Offsets & colors \n");
	gfx_drawstrctr(0, 0, "0,0");
	gfx_set_forecolor(GFX_RED);
	gfx_drawstrctr(10, 10, "10,10");
	gfx_set_forecolor(GFX_GREEN);
	gfx_drawstrctr(20, 20, "20,20");
	gfx_set_forecolor(GFX_BLUE);
	gfx_drawstrctr(50, 50, "50,50");
	gfx_set_forecolor(GFX_LPURPLE);
	gfx_drawstrctr(100, 100, "100,100");
#else
	/* edges */
	printf("Edges\n");
	gfx_drawhline(0, 0, 319);
	gfx_drawhline(169, 0, 319);
	gfx_drawvline(0, 0, 169);
	gfx_drawvline(319, 0, 169);
#endif
#else
	/* rounded rects and colors */
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
	st7789_setBacklight(1);
	
	/* LED initialization with the GPIO */
    ESP_LOGI(TAG, "Init LED on GPIO %d", LED_GPIO);
    gpio_config_t io_conf = {
		.intr_type = GPIO_INTR_DISABLE,
		.mode = GPIO_MODE_OUTPUT,
		.pin_bit_mask = (1ULL<<LED_GPIO),
		.pull_down_en = 0,
		.pull_up_en = 0
	};
    gpio_config(&io_conf);

	/* foreground loop just handles menu */
    ESP_LOGI(TAG, "Looping...");
	uint8_t led_state = 1;
    while(1)
	{
		//st7789_setBacklight(led_state);
		gpio_set_level(LED_GPIO, led_state);
		led_state ^= 1;
		
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
