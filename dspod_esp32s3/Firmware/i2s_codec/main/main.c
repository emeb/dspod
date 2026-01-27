/*
 * main.c - top level of dspod_esp32s3 i2s_codec test
 * 01-16-26 E. Brombaugh
 */

#include <stdio.h>
#include "main.h"
#include "eb_nau88c22.h"
#include "eb_i2s.h"
#include "audio.h"
#ifdef MULTICORE
#include "multicore_audio.h"
#endif
#include "esp_log.h"
#include "driver/gpio.h"

#define LED_GPIO 18

/* tag for logging */
static const char *TAG = "i2c_codec";

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
	printf("\n\ndspod_esp32s3 i2s_codec %s starting\n\r", fwVersionStr);
	printf("Build Date: %s\n\r", bdate);
	printf("Build Time: %s\n\r", btime);
	printf("\n");
	
	/* init audio */
    ESP_LOGI(TAG, "Init Audio");
#ifdef MULTICORE
	multicore_audio_init();
#else
	audio_init();
#endif
	
	/* init codec */
    ESP_LOGI(TAG, "Init nau88c22 Codec");
	eb_nau88c22_Init();
	eb_nau88c22_Reset();
	
	/* report buffer info */
	//i2s_diag();
	
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
        i2s_diag();
		
		gpio_set_level(LED_GPIO, led_state);
		led_state ^= 1;
		
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
