#ifndef __BEEP_H__
#define __BEEP_H__
#include "stm32f4xx.h"

extern uint8_t BEEP_state;

#define BEEP1_OFF GPIO_ResetBits(GPIOF, GPIO_Pin_8);
#define BEEP1_ON GPIO_SetBits(GPIOF, GPIO_Pin_8);

#define BEEP_SWITCH(num, state) BEEP##num##_##state
void beep_gpio_init(void);
#endif


