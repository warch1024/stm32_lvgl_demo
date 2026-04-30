#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "stm32f4xx.h"

#define MOTOR_TIM_PERIOD 1000

#define MOTOR1_ON motor1_set_duty(1)
#define MOTOR1_OFF motor1_set_duty(0)
#define MOTOR_SWITCH(num, state) MOTOR##num##_##state

#define MOTOR1_SPEED_RATIO(speed_ratio) motor1_set_duty((speed_ratio))
#define MOTOR1_SPEED_PERCENT(speed_percentage) motor1_set_duty((speed_percentage/100.0))


//设置通风速度比例
void ventilation(float speed_ratio);

void motor1_speed_regulation_init(void);
void motor1_set_duty(float duty_ratio);
#endif



