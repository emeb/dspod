/*
 * eb_i2s.c - async I2S driver for S3GTA. Requires IDF > V5.1
 * 12-03-24 E. Brombaugh
 */

#ifndef __eb_i2s__
#define __eb_i2s__

void i2s_init(void (*ap_cb)(int16_t *dst, int16_t *src, uint32_t len));
void i2s_diag(void);

#endif
