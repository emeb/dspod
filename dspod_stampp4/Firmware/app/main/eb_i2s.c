/*
 * eb_i2s.c - async I2S driver for dspod stamp p4. Requires IDF > V6.1
 * 09-27-26 E. Brombaugh
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "eb_i2s.h"

/* Hardware I/O defines */
#define I2S_STD_MCLK_IO1        GPIO_NUM_31      // I2S master clock io number
#define I2S_STD_BCLK_IO1        GPIO_NUM_35      // I2S bit clock io number
#define I2S_STD_WS_IO1          GPIO_NUM_36      // I2S word select io number
#define I2S_STD_DOUT_IO1        GPIO_NUM_33      // I2S data out io number
#define I2S_STD_DIN_IO1         GPIO_NUM_41      // I2S data in io number

#define GPIO_RX_DIAG_PIN        GPIO_NUM_1       // Realtime Diag pin for RX
#define GPIO_AUDIO_DIAG_PIN     GPIO_NUM_7       // Realtime Diag pin for Audio
#define GPIO_TX_DIAG_PIN        GPIO_NUM_8       // Realtime Diag pin for TX

/* tag for logging */
static const char* TAG = "eb_i2s";

/* I2S channel handlers */
static i2s_chan_handle_t	tx_chan;        // I2S tx channel handler
static i2s_chan_handle_t	rx_chan;        // I2S rx channel handler
static TaskHandle_t s_task_handle;

/*
 * Asynchronous callbacks
 */
int rx_cnt = 0, tx_cnt = 0;
int16_t *tx_buffer, *rx_buffer, temp_buf[128];
uint32_t tx_sz, rx_sz;
uint32_t cp0_regs[18];
void (*audio_cb)(int16_t *dst, int16_t *src, uint32_t len);

/*
 * RX IRQ callback - this is where all the work is done
 * handling data generation in RX IRQ has ~3ms latency
 */
bool i2s_async_rx_cb(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx)
{
    BaseType_t mustYield = pdFALSE;

	/* raise RT diag */
	gpio_set_level(GPIO_RX_DIAG_PIN, 1);
	
	/* get buffer & size */
	rx_buffer = (int16_t *)event->dma_buf;
	rx_sz = event->size;
	
    /* notify foreground task that it's time to run the audio */
    vTaskNotifyGiveFromISR(s_task_handle, &mustYield);
	
	rx_cnt++;

	/* drop RT diag */
	gpio_set_level(GPIO_RX_DIAG_PIN, 0);

	return (mustYield == pdTRUE);
}

/*
 * TX IRQ callback - unused for now
 * handling data generation in TX IRQ has ~5ms latency
 */
bool i2s_async_tx_cb(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx)
{
	/* raise RT diag */
	gpio_set_level(GPIO_TX_DIAG_PIN, 1);
	
	/* get dest buffer & size */
	tx_buffer = (int16_t *)event->dma_buf;
	tx_sz = event->size;

	/* copy temp buf to dest buf */
	memcpy(tx_buffer, temp_buf, event->size);

	tx_cnt++;
	
	/* drop RT diag */
	gpio_set_level(GPIO_TX_DIAG_PIN, 0);

	return false;
}

/*
 * I2S task - runs separately to simplify callbacks
 */
void eb_i2s_task(void * pvParameters)
{
	ESP_LOGI(TAG, "eb_i2s_task starting...");
	
	/* set up callback */
	audio_cb = (void (*)(int16_t *dst, int16_t *src, uint32_t len)) pvParameters;
	
	/* GPIO for RT diagnostic */
    gpio_config_t io_conf = {
		.intr_type = GPIO_INTR_DISABLE,
		.mode = GPIO_MODE_OUTPUT,
		.pin_bit_mask = (1ULL<<GPIO_TX_DIAG_PIN) | (1ULL<<GPIO_RX_DIAG_PIN) | (1ULL<<GPIO_AUDIO_DIAG_PIN),
		.pull_down_en = 0,
		.pull_up_en = 0
	};
    gpio_config(&io_conf);
	
    /* Set the I2S channel configuration */
    i2s_chan_config_t chan_cfg = {
		.id = I2S_NUM_AUTO,			// Get first avail
		.role = I2S_ROLE_MASTER,	// Generate clocks
		.dma_desc_num = 2,			// Number of DMA buffers
		.dma_frame_num = 64,		// Number of samples per DMA buffer IRQ
		.auto_clear = false,		// don't zero memory in case of err
	};
	ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &tx_chan, &rx_chan));

    /* Set configuration of standard mode */
    i2s_std_config_t std_cfg = {
        .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(48000),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_STD_MCLK_IO1,    // some codecs may require mclk signal, this example doesn't need it
            .bclk = I2S_STD_BCLK_IO1,
            .ws   = I2S_STD_WS_IO1,
            .dout = I2S_STD_DOUT_IO1,
            .din  = I2S_STD_DIN_IO1,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false,
            },
        },
    };
	
    /* Initialize the channels */
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(rx_chan, &std_cfg));
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_chan, &std_cfg));
	
	/* hook up async callbacks */
	i2s_event_callbacks_t evt_rx_cb = {
		.on_recv = i2s_async_rx_cb,
		.on_recv_q_ovf = NULL,
		.on_sent = NULL,
		.on_send_q_ovf = NULL
	};
	i2s_channel_register_event_callback(rx_chan, &evt_rx_cb, NULL);
	i2s_event_callbacks_t evt_tx_cb = {
		.on_recv = NULL,
		.on_recv_q_ovf = NULL,
		.on_sent = i2s_async_tx_cb,
		.on_send_q_ovf = NULL
	};
	i2s_channel_register_event_callback(tx_chan, &evt_tx_cb, NULL);

	/* for messaging between RX ISR and here */
	s_task_handle = xTaskGetCurrentTaskHandle();
	
	/* enable channels - order doesn't matter */
    ESP_ERROR_CHECK(i2s_channel_enable(rx_chan));
    ESP_ERROR_CHECK(i2s_channel_enable(tx_chan));
	
	/* loop forever to handle audio callbacks */
	while(1)
	{
		/* wait for ISR to flag DMA ready */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		
		/* run the audio callback */
		gpio_set_level(GPIO_AUDIO_DIAG_PIN, 1);
		audio_cb(temp_buf, rx_buffer, rx_sz);
		gpio_set_level(GPIO_AUDIO_DIAG_PIN, 0);
	}
}

/*
 * I2S periph setup for Standard (PCM) mode
 */
esp_err_t i2s_init(void (*ap_cb)(int16_t *dst, int16_t *src, uint32_t len))
{
	BaseType_t xReturned = pdFAIL;
    TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
                    eb_i2s_task,     /* Function that implements the task. */
                    "I2S_Task",      /* Text name for the task. */
                    4096       ,     /* Stack size in words, not bytes. */
                    ( void * ) ap_cb,/* Parameter passed into the task. */
                    10,              /* Priority at which the task is created. 0-24 */
                    &xHandle );      /* Used to pass out the created task's handle. */

	if(xReturned == pdPASS)
		return ESP_OK;
	else
		return ESP_FAIL;
}

/*
 * diags
 */
void i2s_diag(void)
{
	ESP_LOGI(TAG, "RX:%d, 0x%08X, %d TX:%d, 0x%08X, %d",
	rx_cnt, (unsigned int)rx_buffer, (int)rx_sz,
	tx_cnt, (unsigned int)tx_buffer, (int)tx_sz);
}