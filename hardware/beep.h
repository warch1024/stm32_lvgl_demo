#ifndef __BEEP_H__
#define __BEEP_H__
#include "stm32f4xx.h"

extern uint8_t BEEP_state, BEEP_PWM_frequency;
extern float BEEP_PWM_duty;

#define BEEP1_OFF GPIO_ResetBits(GPIOF, GPIO_Pin_8);
#define BEEP1_ON GPIO_SetBits(GPIOF, GPIO_Pin_8);

#define BEEP_SWITCH(num, state) BEEP##num##_##state

void beep_gpio_init(void);
void beep_pwm_init(void);
void beep_set_duty(float duty_ratio);    //占空比为0-1

//蜂鸣器PWM调制
extern uint32_t BEEP_TIM_PWM_PERIOD;
#define BEEP_DUTY_RATIO(speed_ratio) beep_set_duty((speed_ratio))
#define BEEP_DUTY_PERCENT(speed_percentage) beep_set_duty((speed_percentage/100.0))
uint32_t beep_set_pwm_frequency(uint32_t frequency);    //设置蜂鸣器PWM频率，单位为Hz,返回实际设置的周期，单位为0.1ms


#endif


