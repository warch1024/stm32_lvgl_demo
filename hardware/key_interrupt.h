#ifndef __KEY_INTERRUPT_H__
#define __KEY_INTERRUPT_H__
#include "stm32f4xx.h"
extern uint8_t KEY1_State;
extern uint8_t KEY2_State;
extern uint8_t KEY3_State;
extern uint8_t KEY4_State;

void key_interrupt_init(void);
void key_gpio_init(void);
void clear_key_state(void);//清除按键状态
#endif



