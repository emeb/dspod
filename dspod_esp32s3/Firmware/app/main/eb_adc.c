/*
 * eb_adc.c - my high-level ADC driver. Mimics background ADC operation.
 * 01-16-22 E. Brombaugh
 */
 
/* choose one-shot or continuous mode */
//#define ONE_SHOT

 
#include <stdio.h>
#include "main.h"
#ifdef ONE_SHOT
#include "esp_adc/adc_oneshot.h"
#else
#include "esp_adc/adc_continuous.h"
#endif
#include "driver/gpio.h"
#include "esp_timer.h"
#include "hal/i2s_hal.h"
#include "hal/i2s_types.h"
#include <soc/sens_reg.h>
#include <soc/sens_struct.h>
#include <soc/spi_reg.h>
#include <soc/spi_struct.h>
#include "eb_adc.h"

static const char* TAG = "eb_adc";
int32_t adc_iir[ADC_NUMVALS];
volatile int16_t ADC_val[ADC_NUMVALS];

/*
 * dspod channel mapping:
 * POT 1 - CV1 - GPIO5 - CHL4
 * POT 2 - CV2 - GPIO6 - CHL5
 * POT 3 - CV3 - GPIO7 - CHL6
 * POT 4 - CV4 - GPIO8 - CHL7
 */
const uint8_t adc_chls[ADC_NUMVALS] =
{
	ADC_CHANNEL_4,
	ADC_CHANNEL_5,
	ADC_CHANNEL_6,
	ADC_CHANNEL_7,
};

/*
 * IIR filter for 12-bit ADC values 
 */
#define IIR_COEF 4
inline int16_t adc_IIR_filter(int32_t *filt_state, int16_t in)
{
	*filt_state += ((in<<IIR_COEF) - *filt_state )>>IIR_COEF;
	return *filt_state >> (IIR_COEF);
}

#ifdef ONE_SHOT
/* one-shot mode is manually addressed & started by timer */
static uint8_t adc_idx;

/*
 * low-level: set channel, wait for ready & start conversion
 * cuts time from 45us to 15us
 */
void IRAM_ATTR my_adc1_get_raw_begin(uint8_t chl)
{
	SENS.sar_meas1_ctrl2.sar1_en_pad = (1 << chl);
	/* note that this reg is undocumented! */
	while (HAL_FORCE_READ_U32_REG_FIELD(SENS.sar_slave_addr1, meas_status) != 0) {}
	SENS.sar_meas1_ctrl2.meas1_start_sar = 0;
	SENS.sar_meas1_ctrl2.meas1_start_sar = 1;
}

/*
 * low-level: wait for eoc & fetch result
 */
int16_t IRAM_ATTR my_adc1_get_raw_end(void)
{
    while (SENS.sar_meas1_ctrl2.meas1_done_sar == 0);
    return 4095-HAL_FORCE_READ_U32_REG_FIELD(SENS.sar_meas1_ctrl2, meas1_data_sar);
}

/*
 * ADC timer callback
 */
void IRAM_ATTR adc_timer_callback(void *arg)
{
	/* set ISR active flag */
	//gpio_set_level(DIAG_1, 1);
	
	/* don't wait for conversion - get previous result */
	adc_val[adc_idx] = adc_IIR_filter(&adc_iir[adc_idx], my_adc1_get_raw_end());

	/* advance channel */
	adc_idx = (adc_idx + 1)%ADC_NUMVALS;
	
	/* start next conversion */
	my_adc1_get_raw_begin(adc_chls[adc_idx]);
	
	/* clear ISR active flag */
	//gpio_set_level(DIAG_1, 0);
}

/*
 * one-shot init
 */
esp_err_t eb_adc_init(void)
{
	uint8_t i;
	esp_timer_handle_t timer_handle;
	
	/* setup state */
	adc_idx = 0;
	
	/* single conversions w/ timer using new one-shot API */
    ESP_LOGI(TAG, "PIO mode w/ new one-shot driver");
	/* init ADC1 */
	adc_oneshot_unit_handle_t adc1_handle;
	adc_oneshot_unit_init_cfg_t init_config1 = {
		.unit_id = ADC_UNIT_1,
		.ulp_mode = ADC_ULP_MODE_DISABLE,
	};
	ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));
	
	/* set up channels: GPIO 5-8 are ADC1 chls 4-7 */
	adc_oneshot_chan_cfg_t config = {
		.bitwidth = SOC_ADC_RTC_MAX_BITWIDTH,	// defaults to 12 bit on S3
		.atten = ADC_ATTEN_DB_12,
	};
	for(i=0;i<ADC_NUMVALS;i++)
		ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, adc_chls[i], &config));

	/* read a channel to start ADC */
	int dummy;
	ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, adc_chls[0], &dummy));
	
	/* Timer for controlling ADC sampling intervals */
	esp_timer_create_args_t timer_args = {
		.callback = adc_timer_callback,
		.arg = NULL,
		.name = "adc_timer",
		.skip_unhandled_events = true
	};
	esp_timer_create(&timer_args, &timer_handle);
	esp_timer_start_periodic(timer_handle, 1000);
	
	return ESP_OK;
}
#else
/* continuous mode is addressed by HW and DMA runs in background */
static adc_continuous_handle_t adc_cont_handle = NULL;
static uint8_t adc_rev_chl[20] = {0};

/*
 * continuous end-of-frame callback
 */
bool IRAM_ATTR adc_eof_callback(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data)
{
	adc_continuous_data_t parsed_data[ADC_NUMVALS];
	uint32_t num_samples;
	
	/* get data from this conv */
	adc_continuous_read_parse(handle, parsed_data, ADC_NUMVALS, &num_samples, 10000);
	
	/* isolate the stuff we care about */
	for(int i=0;i<num_samples;i++)
	{
		int idx = adc_rev_chl[parsed_data[i].channel];
		ADC_val[idx] = adc_IIR_filter(&adc_iir[idx], 4095-parsed_data[i].raw_data);
	}
	
	return 0;
}

/*
 * continuous init
 */
esp_err_t eb_adc_init(void)
{
	/* set up continuous conv pool with four frames of conversions */
	adc_continuous_handle_cfg_t adc_config = {
		.max_store_buf_size = 4*ADC_NUMVALS*sizeof(adc_digi_output_data_t),
		.conv_frame_size = ADC_NUMVALS*sizeof(adc_digi_output_data_t),
	};
	ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &adc_cont_handle));
	
	/* init the pattern */
	adc_digi_pattern_config_t pattern[ADC_NUMVALS];
	for(int i=0;i<ADC_NUMVALS;i++)
	{
		pattern[i].atten = ADC_ATTEN_DB_12;
		pattern[i].channel = adc_chls[i];
		pattern[i].unit = ADC_UNIT_1;
		pattern[i].bit_width = SOC_ADC_RTC_MAX_BITWIDTH;
		adc_rev_chl[adc_chls[i]] = i;
	}
	
	/* set up pattern, freq & mode */
	adc_continuous_config_t config = {
		.pattern_num = ADC_NUMVALS,
		.adc_pattern = pattern,
		.sample_freq_hz = 1000,
		.conv_mode = ADC_CONV_SINGLE_UNIT_1,
		.format = ADC_DIGI_OUTPUT_FORMAT_TYPE2	// only type supported on S3
	};
	ESP_ERROR_CHECK(adc_continuous_config(adc_cont_handle, &config));
	
	/* set up callback for end-of-frame */
	adc_continuous_evt_cbs_t cbs =
	{
		.on_conv_done = adc_eof_callback,
		.on_pool_ovf = NULL
	};
	ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(adc_cont_handle, &cbs, NULL));
	
	/* start conversions */
	return adc_continuous_start(adc_cont_handle);
}
#endif