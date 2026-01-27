/*
 * main.c - top level of dspod_esp32s3 encoder test
 * 01-19-26 E. Brombaugh
 */

#include <stdio.h>
#include "main.h"
#include "encoder.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define LED_GPIO 18

/* raw or processed inputs */
#define PROC_INPUT

/* tag for logging */
static const char *TAG = "adc";

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
	printf("\n\ndspod_esp32s3 encoder %s starting\n\r", fwVersionStr);
	printf("Build Date: %s\n\r", bdate);
	printf("Build Time: %s\n\r", btime);
	printf("\n");
	
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

#ifdef PROC_INPUT
	/* init encoder processing */
    ESP_LOGI(TAG, "Init encoder");
	encoder_init();
#else
	/* raw inputs */
    ESP_LOGI(TAG, "Raw inputs");
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pull_up_en = 1;
	
	io_conf.pin_bit_mask = (1ULL<<47); // btn1
    gpio_config(&io_conf);	
	
	io_conf.pin_bit_mask = (1ULL<<33); // btn2
    gpio_config(&io_conf);
	
	io_conf.pin_bit_mask = (1ULL<<34), // btn3
    gpio_config(&io_conf);
	
	io_conf.pin_bit_mask = (1ULL<<35), // btn4
    gpio_config(&io_conf);
#endif

	/* foreground loop just handles menu */
    ESP_LOGI(TAG, "Looping...");
	uint8_t led_state = 1;
    while(1)
	{
#if 1
#if 1
		printf("% 5d ", encoder_get_enc());
		printf("% 2d ", encoder_get_button(ENC_E));
		printf("% 2d ", encoder_get_button(TAP));
#else
		printf("% 2d ", encoder_btn_raw(ENC_A));
		printf("% 2d ", encoder_btn_raw(ENC_B));
		printf("% 2d ", encoder_btn_raw(ENC_E));
		printf("% 2d ", encoder_btn_raw(TAP));
#endif		
#else
		printf("% 2d ", gpio_get_level(47));
		printf("% 2d ", gpio_get_level(33));
		printf("% 2d ", gpio_get_level(34));
		printf("% 2d ", gpio_get_level(35));
#endif
		printf("\n");
		
		gpio_set_level(LED_GPIO, led_state);
		led_state ^= 1;
		
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
