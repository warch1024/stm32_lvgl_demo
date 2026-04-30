#include "motor.h"
#include "pwm.h"

void motor1_speed_regulation_init(void)
{
    AHB1_gpio_pwm_init(RCC_AHB1Periph_GPIOC, GPIO_Pin_6, GPIOC);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);// 配置GPIOC6为TIM3通道2的AF引脚
    // 配置定时器3为PWM模式，pc6连接time3ch1
    APB1_timer_pwm_init(RCC_APB1Periph_TIM3, TIM3, TIM_OCMode_PWM1, TIM_OCPolarity_High, 500, MOTOR_TIM_PERIOD, 1);
}
void motor1_set_duty(float duty_ratio)
{
    // 设置占空比
    TIM_SetCompare1(TIM3, duty_ratio * MOTOR_TIM_PERIOD);
}
//设置通风速度比例
void ventilation(float speed_ratio){
    MOTOR1_SPEED_RATIO(speed_ratio);
}




