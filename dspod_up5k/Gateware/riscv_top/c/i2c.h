/*
 * i2c.h - I2C IP core driver
 * 06-23-20 E. Brombaugh
 */

#ifndef __i2c__
#define __i2c__

#include "main.h"

/* i2c functions */
void i2c_init(I2C_TypeDef *s);
int8_t i2c_mtx(I2C_TypeDef *s, uint8_t addr, uint8_t *data, uint8_t sz);
int8_t i2c_mrx(I2C_TypeDef *s, uint8_t addr, uint8_t *data, uint8_t sz);
int8_t i2c_mtxrx(I2C_TypeDef *s, uint8_t addr, uint8_t *txdata, uint8_t txsz, uint8_t *rxdata, uint8_t rxsz);
int8_t i2c_srx(I2C_TypeDef *s, uint8_t addr, uint8_t *data, uint8_t sz);

#endif

