/*
 * main.c - top level of dspod_esp32s3 effects application
 * 01-16-26 E. Brombaugh
 */

#include <stdio.h>
#include "main.h"
#include "gfx.h"
#include "st7789_drvr.h"
#include "eb_nau88c22.h"
#include "eb_i2s.h"
#include "audio.h"
#include "eb_adc.h"
#include "encoder.h"
#include "menu.h"
#ifdef MULTICORE
#include "multicore_audio.h"
#endif
#include "esp_log.h"
#include "driver/gpio.h"

#define LED_GPIO 18

/* tag for logging */
static const char *TAG = "fx_app_main";

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
	printf("\n\ndspod_esp32s3 effects app %s starting\n\r", fwVersionStr);
	printf("Build Date: %s\n\r", bdate);
	printf("Build Time: %s\n\r", btime);
	printf("\n");
	
	/* init ADC */
    ESP_LOGI(TAG, "Init ADC");
	eb_adc_init();
	
	/* init audio */
#ifdef MULTICORE
    ESP_LOGI(TAG, "Init Multi-core Audio");
	multicore_audio_init();
#else
    ESP_LOGI(TAG, "Init Single-core Audio");
	Audio_Init(48000);
#endif
	
	/* init codec */
    ESP_LOGI(TAG, "Init nau88c22 Codec");
	eb_nau88c22_Init();
	eb_nau88c22_Reset();
	
	/* init encoder processing */
    ESP_LOGI(TAG, "Init encoder");
	encoder_init();

	/* init the LCD */
    ESP_LOGI(TAG, "Init GFX with ST7789");
	gfx_init(&st7789_drvr);
	menu_splash(fwVersionStr, bdate, btime);
	st7789_setBacklight(1);
	
	/* splash delay */
	vTaskDelay(pdMS_TO_TICKS(2000));
	
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

	/* init menu */
    ESP_LOGI(TAG, "Init menu");
	menu_init();
	Audio_Mute(0);
	
	/* foreground loop just handles menu */
    ESP_LOGI(TAG, "Looping...");
	uint8_t led_state = 1;
	uint8_t led_cnt = 0;
    while(1)
	{
		/* handle menu */
		menu_process();
		
		/* flash LED at ~ 200ms */
		led_cnt++;
		if(led_cnt == 6)
		{
			gpio_set_level(LED_GPIO, led_state);
			led_state ^= 1;
			led_cnt = 0;
		}
		
        vTaskDelay(pdMS_TO_TICKS(33));
    }
}
