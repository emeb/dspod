/*
 * eb_i2s.c - async I2S driver for S3GTA. Requires IDF > V5.1
 * 12-03-24 E. Brombaugh
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
#define I2S_STD_MCLK_IO1        GPIO_NUM_15      // I2S master clock io number
#define I2S_STD_BCLK_IO1        GPIO_NUM_13      // I2S bit clock io number
#define I2S_STD_WS_IO1          GPIO_NUM_11      // I2S word select io number
#define I2S_STD_DOUT_IO1        GPIO_NUM_12      // I2S data out io number
#define I2S_STD_DIN_IO1         GPIO_NUM_10      // I2S data in io number

#define GPIO_TX_DIAG_PIN        GPIO_NUM_36      // Realtime Diag pin for TX
#define GPIO_RX_DIAG_PIN        GPIO_NUM_37      // Realtime Diag pin for RX

/* tag for logging */
static const char* TAG = "eb_i2s";

/* I2S channel handlers */
static i2s_chan_handle_t	tx_chan;        // I2S tx channel handler
static i2s_chan_handle_t	rx_chan;        // I2S rx channel handler

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
	/* raise RT diag */
	gpio_set_level(GPIO_RX_DIAG_PIN, 1);
	
	/* enable and save FPU - save/restore uses 600ns more (1.7us for bare) */
	xthal_set_cpenable(1);
	//xthal_save_cp0(cp0_regs);
	
	/* get buffer & size */
	rx_buffer = *((int16_t **)event->data);
	rx_sz = event->size / sizeof(int16_t);
	
	/* use temp buffer so tx completes fast */
	audio_cb(temp_buf, rx_buffer, event->size);
	
	/* restore and disable FPU */
	//xthal_restore_cp0(cp0_regs);
	xthal_set_cpenable(0);

	rx_cnt++;

	/* drop RT diag */
	gpio_set_level(GPIO_RX_DIAG_PIN, 0);

	return false;
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
	tx_buffer = *((int16_t **)event->data);
	tx_sz = event->size / sizeof(int16_t);

	/* copy temp buf to dest buf */
	memcpy(tx_buffer, temp_buf, event->size);

	tx_cnt++;
	
	/* drop RT diag */
	gpio_set_level(GPIO_TX_DIAG_PIN, 0);

	return false;
}

/*
 * I2S periph setup for Standard (PCM) mode
 */
void i2s_init(void (*ap_cb)(int16_t *dst, int16_t *src, uint32_t len))
{
	/* set up callback */
	audio_cb = ap_cb;
	
	/* GPIO for RT diagnostic */
    gpio_config_t io_conf = {
		.intr_type = GPIO_INTR_DISABLE,
		.mode = GPIO_MODE_OUTPUT,
		.pin_bit_mask = (1ULL<<GPIO_TX_DIAG_PIN) | (1ULL<<GPIO_RX_DIAG_PIN),
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

	/* enable channels - order doesn't matter */
    ESP_ERROR_CHECK(i2s_channel_enable(rx_chan));
    ESP_ERROR_CHECK(i2s_channel_enable(tx_chan));
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