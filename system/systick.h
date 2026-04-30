#ifndef __SYSTICK_H__
#define __SYSTICK_H__
#include "stm32f4xx.h"

void systick_init(void);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

#endif


