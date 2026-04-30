#ifndef __PWM_H__
#define __PWM_H__
#include "stm32f4xx.h"

#define TIM14_TIM_PERIOD 10000

#define TIM_OCxINIT(OCx)  TIM_OC##OCx##Init // 初始化通道OCx
#define TIM_OCxPRELOAD_CONFIG(OCx) TIM_OC##OCx##PreloadConfig  // 使能通道OCx的预装载
void timer14_pwm_init(void);
void timer1_pwm_init(void);
void timer14_pwm_set_duty(uint16_t duty);
void AHB1_gpio_pwm_init(uint32_t RCC_AHB1Periph_GPIOx, uint16_t GPIO_Pin_x, GPIO_TypeDef *GPIOx);

void APB1_timer_pwm_init(uint32_t RCC_APB1Periph_TIMx, TIM_TypeDef *TIMx, uint16_t TIM_OCMode_x,
    uint16_t TIM_OCPolarity_x, uint32_t TIM_Pulse_cnt, uint32_t TIM_Period, uint8_t OCx);

void APB2_timer_pwm_init(uint32_t RCC_APB2Periph_TIMx, TIM_TypeDef *TIMx, uint16_t TIM_OCMode_x,
    uint16_t TIM_OCPolarity_x, uint32_t TIM_Pulse_cnt, uint32_t TIM_Period, uint8_t OCx);

#endif

