/*
 * eb_adc.c - my high-level ADC driver. Mimics background ADC operation.
 * 01-16-22 E. Brombaugh
 */
 
/* choose one-shot or continuous mode */
//#define ONE_SHOT

/* choose filtering on/off */
#define ADC_FILT_ENA

#include <stdio.h>
#include "main.h"
#include "freertos/semphr.h"
#include "esp_adc/adc_continuous.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "hal/i2s_hal.h"
#include "hal/i2s_types.h"
#include <soc/spi_reg.h>
#include <soc/spi_struct.h>
#include "eb_adc.h"

#define ADC_READ_LEN 64

static const char* TAG = "eb_adc";
volatile int16_t adc_val[ADC_NUMVALS];
volatile esp_cpu_cycle_count_t prev_ccnt, adc_sample_period;
static TaskHandle_t s_task_handle;

#ifdef DO_STATS
volatile int16_t adc_hist[ADC_NUMVALS][NUM_HIST], hist_cnt = 0, hist_done = 0;
#endif

/*
 * dspod channel mapping:
 * POT 1 - CV1 - GPIO23 - CHL4
 * POT 2 - CV2 - GPIO22 - CHL5
 * POT 3 - CV3 - GPIO21 - CHL6
 * POT 4 - CV4 - GPIO20 - CHL7
 */

#ifdef ADC_FILT_ENA
/*
 * IIR filter for 12-bit ADC values 
 */
#define IIR_COEF 4
static int32_t adc_iir[ADC_NUMVALS];
inline int16_t adc_IIR_filter(int32_t *filt_state, int16_t in)
{
	*filt_state += ((in<<IIR_COEF) - *filt_state )>>IIR_COEF;
	return *filt_state >> (IIR_COEF);
}
#endif

/*
 * continuous end-of-frame callback
 */
bool IRAM_ATTR adc_eof_callback(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data)
{
    BaseType_t mustYield = pdFALSE;
	
    /* notify foreground task that EOF has happened */
    vTaskNotifyGiveFromISR(s_task_handle, &mustYield);

    return (mustYield == pdTRUE);
}

/*
 * continuous init & processing task
 */
void eb_adc_cont_task(void * pvParameters)
{
    ESP_LOGI(TAG, "eb_adc_cont_task()...");
	
    esp_err_t ret;
    uint32_t ret_num = 0;
	adc_channel_t channel[4] =
	{
		ADC_CHANNEL_7,
		ADC_CHANNEL_6,
		ADC_CHANNEL_5,
		ADC_CHANNEL_4
	};
	uint8_t adc_rev_chl[32] = {0};
    uint8_t result[ADC_READ_LEN] = {0};
    memset(result, 0xcc, ADC_READ_LEN);

	s_task_handle = xTaskGetCurrentTaskHandle();
	
    adc_continuous_handle_t handle = NULL;
    uint8_t channel_num = sizeof(channel) / sizeof(adc_channel_t);
	
    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = 1024,
        .conv_frame_size = ADC_READ_LEN,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &handle));

    adc_continuous_config_t dig_cfg = {
        .sample_freq_hz = 20 * 1000,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
    };

    adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX] = {0};
    dig_cfg.pattern_num = channel_num;
    for(int i = 0; i < channel_num; i++)
	{
        adc_pattern[i].atten = ADC_ATTEN_DB_12;
        adc_pattern[i].channel = channel[i] & 0x7;
        adc_pattern[i].unit = ADC_UNIT_1;
        adc_pattern[i].bit_width = SOC_ADC_DIGI_MAX_BITWIDTH;
		adc_rev_chl[channel[i]] = i;
    }
    dig_cfg.adc_pattern = adc_pattern;
    ESP_ERROR_CHECK(adc_continuous_config(handle, &dig_cfg));

    adc_continuous_evt_cbs_t cbs =
	{
        .on_conv_done = adc_eof_callback,
    };
    ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(handle, &cbs, NULL));
    ESP_ERROR_CHECK(adc_continuous_start(handle));
	
	/* loop */
	while(1)
	{
		/* wait for ISR to flag EOF */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		
		while(1)
		{
			/* try to get data */
            ret = adc_continuous_read(handle, result, ADC_READ_LEN, &ret_num, 0);

            if(ret == ESP_OK)
			{
				/* data good - process result */
				esp_cpu_cycle_count_t curr_ccnt = esp_cpu_get_cycle_count();
				adc_sample_period = curr_ccnt - prev_ccnt;
				prev_ccnt = curr_ccnt;
				
                adc_continuous_data_t parsed_data[ret_num / SOC_ADC_DIGI_RESULT_BYTES];
                uint32_t num_parsed_samples = 0;

                esp_err_t parse_ret = adc_continuous_parse_data(handle, result, ret_num, parsed_data, &num_parsed_samples);
                if(parse_ret == ESP_OK)
				{
					/* isolate the stuff we care about */
					for(int i=0;i<num_parsed_samples;i++)
					{
						int idx = adc_rev_chl[parsed_data[i].channel];
						int cal_val = 3351-parsed_data[i].raw_data;	// offse + invert
						cal_val = cal_val + ((cal_val*56)>>8);		// scale
						adc_val[idx] = adc_IIR_filter(&adc_iir[idx], cal_val);
#ifdef DO_STATS
						if(hist_done == 0)
							adc_hist[idx][hist_cnt] = adc_val[idx];
#endif
					}

#ifdef DO_STATS
					if(hist_done == 0)
					{
						if(hist_cnt<NUM_HIST)
							hist_cnt++;
						else
						{
							hist_cnt = 0;
							hist_done = 1;
						}	
					}
#endif
				}
			}
			else if(ret == ESP_ERR_TIMEOUT)
			{
                /* no available data - wait for next EOC */
                break;
			}
		}
	}
}

/*
 * start ADC task to init & handle processing
 */
esp_err_t eb_adc_init(void)
{
	BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
                    eb_adc_cont_task,/* Function that implements the task. */
                    "ADC_Task",      /* Text name for the task. */
                    4096       ,     /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle );      /* Used to pass out the created task's handle. */

	if(xReturned == pdPASS)
		return ESP_OK;
	else
		return ESP_FAIL;
}
