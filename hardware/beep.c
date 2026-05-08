#include "beep.h"
#include "stm32f4xx.h"
#include "pwm.h"

uint8_t BEEP_state = 0, BEEP_PWM_frequency = 2 ;
float BEEP_PWM_duty = 0.5;
uint32_t BEEP_TIM_PWM_PERIOD = 0;   //蜂鸣器PWM周期，1=0.1ms

void beep_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_bEEP_init;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    GPIO_bEEP_init.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_bEEP_init.GPIO_OType = GPIO_OType_PP;
    GPIO_bEEP_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_bEEP_init.GPIO_PuPd = GPIO_PuPd_DOWN; //默认下拉
    GPIO_bEEP_init.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOF, &GPIO_bEEP_init);
}

void beep_pwm_init(void){   //tim13-ch1对应GPIOF_8蜂鸣器
    AHB1_gpio_pwm_init(RCC_AHB1Periph_GPIOF, GPIO_Pin_8, GPIOF);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13);// 配置GPIOF8为TIM13通道1的AF引脚
    // 配置定时器13为PWM模式，pc8连接time13ch1
    APB1_timer_pwm_init(RCC_APB1Periph_TIM13, TIM13, TIM_OCMode_PWM1, TIM_OCPolarity_High, 500, 1000, 1);
    BEEP_TIM_PWM_PERIOD = 1000;
}
void beep_set_duty(float duty_ratio)    //占空比为0-1，调节蜂鸣器音量
{
    // 设置占空比
    TIM_SetCompare1(TIM13, duty_ratio * BEEP_TIM_PWM_PERIOD);
}

// 设置蜂鸣器PWM频率，单位为Hz,返回实际设置的周期，单位为0.1ms，调节蜂鸣器频率
uint32_t beep_set_pwm_frequency(uint32_t frequency)
{
    BEEP_TIM_PWM_PERIOD = 10000.0/(frequency);
    APB1_timer_pwm_set_period(TIM13, BEEP_TIM_PWM_PERIOD);
    return BEEP_TIM_PWM_PERIOD;
}


