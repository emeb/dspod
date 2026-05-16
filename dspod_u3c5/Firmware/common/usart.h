/*
 * usart.h - uart diag support for u3c5 nucleo
 * 03-26-26 E. Brombaugh
 */

#ifndef __usart__
#define __usart__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32u3xx_hal.h"

void setup_usart(void);
void usart_putc(void* p, char c);

#ifdef __cplusplus
}
#endif

#endif
