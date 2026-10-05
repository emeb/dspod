/*
 * eb_i2s.h - async I2S driver for dspod stamp p4. Requires IDF > V6.1
 * 09-27-26 E. Brombaugh
 */

#ifndef __eb_i2s__
#define __eb_i2s__

esp_err_t i2s_init(void (*ap_cb)(int16_t *dst, int16_t *src, uint32_t len));
void i2s_diag(void);

#endif
