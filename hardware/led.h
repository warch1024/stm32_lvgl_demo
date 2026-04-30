#ifndef __LED_H__
#define __LED_H__
#include "stm32f4xx.h"

// 定义LED状态变量
extern uint8_t LED1_state;
extern uint8_t LED2_state;
extern uint8_t LED3_state;
extern uint8_t LED4_state;

void led_gpio_init(void);
//led1
#define LED1_ON GPIO_ResetBits(GPIOF, GPIO_Pin_9);
#define LED1_OFF GPIO_SetBits(GPIOF, GPIO_Pin_9);
//led2
#define LED2_ON GPIO_ResetBits(GPIOF, GPIO_Pin_10);
#define LED2_OFF GPIO_SetBits(GPIOF, GPIO_Pin_10);
//led3
#define LED3_ON GPIO_ResetBits(GPIOE, GPIO_Pin_13);
#define LED3_OFF GPIO_SetBits(GPIOE, GPIO_Pin_13);
//led4
#define LED4_ON GPIO_ResetBits(GPIOE, GPIO_Pin_14);
#define LED4_OFF GPIO_SetBits(GPIOE, GPIO_Pin_14);

#define LED_SWITCH(num, state) LED##num##_##state
#endif

