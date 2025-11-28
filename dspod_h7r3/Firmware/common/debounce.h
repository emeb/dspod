/*
 * debounce.h - simple button debouncer
 * E. Brombaugh 07-07-2019
 */

#ifndef __debounce__
#define __debounce__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32h7rsxx_hal.h"

typedef struct
{
	uint16_t pipe;
	uint8_t state;
	uint8_t prev_state;
	uint8_t re;
	uint8_t fe;
	uint16_t mask;
} debounce_state;

void init_debounce(debounce_state *dbs, uint8_t len);
void debounce(debounce_state *dbs, uint32_t in);

#ifdef __cplusplus
}
#endif

#endif
