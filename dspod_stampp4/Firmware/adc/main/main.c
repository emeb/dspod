/*
 * main.c - top level of dspod_stampp4 adc test
 * 09-29-26 E. Brombaugh
 */

#include <stdio.h>
#include <math.h>
#include "main.h"
#include "eb_adc.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_clk_tree.h"

#define LED_GPIO 26

/* tag for logging */
static const char *TAG = "main";

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
	uint32_t cpu_freq;
	esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_CPU, 0, &cpu_freq);
	
	/* start logging of main app */
	printf("\n\ndspod_stampp4 adc %s starting\n\r", fwVersionStr);
	printf("Build Date: %s\n\r", bdate);
	printf("Build Time: %s\n\r", btime);
	printf("CPU Freq: %lu Hz\n\r", cpu_freq);
	printf("\n");
	
	/* init audio */
    ESP_LOGI(TAG, "Init ADC");
	eb_adc_init();
    ESP_LOGI(TAG, "back from Init ADC");
	
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
		printf("Fsamp = %lu\t", cpu_freq / adc_sample_period);
        for(int i=0;i<ADC_NUMVALS;i++)
			printf("% 5d ", adc_val[i]);
		printf("\n");
		
#ifdef DO_STATS
		if(hist_done == 1)
		{
			int i, j;
#if 0
			printf("ADC hist:\n\r");
			for(j=1;j<NUM_HIST;j++)
			{
				printf("%d\t", j);
				for(int i=0;i<ADC_NUMVALS;i++)
					printf("%d\t", adc_hist[i][j]);
				printf("\n\r");
			}
#endif
			printf("ADC mean: ");
			float adc_mean[ADC_NUMVALS], adc_rms[ADC_NUMVALS];
			for(i=0;i<ADC_NUMVALS;i++)
			{
				adc_mean[i] = 0.0f;
				for(j=1;j<NUM_HIST;j++)
				{
					adc_mean[i] += adc_hist[i][j];
				}
				adc_mean[i] /= (float)(NUM_HIST-1);
				printf("%f\t", adc_mean[i]);
			}
			printf("\n\r");
			
			printf("ADC std dev: ");
			for(i=0;i<ADC_NUMVALS;i++)
			{			
				adc_rms[i] = 0.0f;
				for(j=1;j<NUM_HIST;j++)
				{
					adc_rms[i] += powf((float)adc_hist[i][j]-adc_mean[i], 2.0f);
				}
				adc_rms[i] = sqrtf(adc_rms[i]/(NUM_HIST-1));
				printf("%f ", adc_rms[i]);
			}
			printf("\n\r");
			hist_done = 0;
		}
#endif		
		gpio_set_level(LED_GPIO, led_state);
		led_state ^= 1;
		
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
