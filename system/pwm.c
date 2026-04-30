#include "pwm.h"


void timer14_pwm_init(void)
{
    AHB1_gpio_pwm_init(RCC_AHB1Periph_GPIOF, GPIO_Pin_9, GPIOF);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);// 配置GPIOF9为TIM14通道2的AF引脚
    // 配置定时器14为PWM模式
    APB1_timer_pwm_init(RCC_APB1Periph_TIM14, TIM14, TIM_OCMode_PWM1, TIM_OCPolarity_Low, 500, TIM14_TIM_PERIOD, 1);
}
void timer1_pwm_init(void)//e14连接led4
{
    AHB1_gpio_pwm_init(RCC_AHB1Periph_GPIOE, GPIO_Pin_14, GPIOE);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);// 配置GPIOE14为TIM1通道2的AF引脚
    // 配置定时器1为PWM模式
    APB2_timer_pwm_init(RCC_APB2Periph_TIM1, TIM1, TIM_OCMode_PWM1, TIM_OCPolarity_Low, 500, 1000, 4);//led4接TIM1通道4
}


void timer14_pwm_set_duty(uint16_t duty)
{
    // 设置占空比
    TIM_SetCompare1(TIM14, duty);
}

//封装函数，初始化AHB1端口的PWM引脚为AF模式，速度为50MHz，无上拉下拉
void AHB1_gpio_pwm_init(uint32_t RCC_AHB1Periph_GPIOx, uint16_t GPIO_Pin_x, GPIO_TypeDef *GPIOx)
{
    // 使能GPIOx
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);
    //配置GPIOx为AF模式，速度为50MHz，无上拉下拉
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_x;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOx, &GPIO_InitStruct);// 初始化GPIOF9为AF模式
}

//封装函数，参数为定时器x总线、定时器结构体，输出比较模式、极性、占空比, 周期(单位:us)
// 返回值: 无
// 备注: 1. 周期为1000us，2-7， 11-14挂在APB1总线上
// 2. 占空比为0-1000
void APB1_timer_pwm_init(uint32_t RCC_APB1Periph_TIMx, TIM_TypeDef *TIMx, uint16_t TIM_OCMode_x,
    uint16_t TIM_OCPolarity_x, uint32_t TIM_Pulse_cnt, uint32_t TIM_Period, uint8_t OCx)
{
    // 初始化定时器x为PWM模式
    // 使能定时器4
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx, ENABLE);
    
    // 配置定时器x
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 84 - 1;
    TIM_TimeBaseInitStruct.TIM_Period = TIM_Period - 1;   // 周期为TIM_Periodus
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct);

    // 配置通道1为PWM模式
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_x;// 模式1 上升沿
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_x;
    TIM_OCInitStruct.TIM_Pulse = TIM_Pulse_cnt;   //根据周期和占空比计算脉冲宽度
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;  // 使能输出
    switch(OCx){
        case 1:
            TIM_OC1Init(TIMx, &TIM_OCInitStruct);//初始化通道
            break;
        case 2:
            TIM_OC2Init(TIMx, &TIM_OCInitStruct);//初始化通道
            break;
        case 3:
            TIM_OC3Init(TIMx, &TIM_OCInitStruct);//初始化通道
            break;
        case 4:
            TIM_OC4Init(TIMx, &TIM_OCInitStruct);//初始化通道
            break;
    }
    //使能预装载和重装载
    switch(OCx){
        case 1:
            TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case 2:
            TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case 3:
            TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case 4:
            TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
    }
    TIM_ARRPreloadConfig(TIMx, ENABLE);//使能重装载
    // 启动定时器x
    TIM_Cmd(TIMx, ENABLE);

}
//封装函数，参数为定时器x总线、定时器结构体，输出比较模式、极性、占空比, 周期(单位:us),初始化通道OCx
// 返回值: 无
// 备注: 1. 周期为1000us，1，8-11挂在APB2总线上，1，8高级定时器
// 2. 占空比为0-1000
void APB2_timer_pwm_init(uint32_t RCC_APB2Periph_TIMx, TIM_TypeDef *TIMx, uint16_t TIM_OCMode_x,
    uint16_t TIM_OCPolarity_x, uint32_t TIM_Pulse_cnt, uint32_t TIM_Period, uint8_t OCx)
{
    // 初始化定时器x为PWM模式
    // 使能定时器x，
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIMx, ENABLE);
    
    // 配置定时器x
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 168 - 1;
    TIM_TimeBaseInitStruct.TIM_Period = TIM_Period - 1;   // 周期为TIM_Periodus
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct);

    // 配置通道1为PWM模式
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_x;// 模式1 上升沿
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_x;
    TIM_OCInitStruct.TIM_Pulse = TIM_Pulse_cnt;   //根据周期和占空比计算脉冲宽度
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;  // 使能输出
    switch(OCx){
        case 1:
            TIM_OC1Init(TIMx, &TIM_OCInitStruct);//初始化通道
            break;
        case 2:
            TIM_OC2Init(TIMx, &TIM_OCInitStruct);//初始化通道
            break;
        case 3:
            TIM_OC3Init(TIMx, &TIM_OCInitStruct);//初始化通道
            break;
        case 4:
            TIM_OC4Init(TIMx, &TIM_OCInitStruct);//初始化通道
            break;
    }
    //使能预装载和重装载
    switch(OCx){
        case 1:
            TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case 2:
            TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case 3:
            TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case 4:
            TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
    }
    TIM_ARRPreloadConfig(TIMx, ENABLE);//使能重装载
    if(TIMx  == TIM1 || TIMx == TIM8){
        TIM_CtrlPWMOutputs(TIMx,ENABLE);
    }
    // 启动定时器x
    TIM_Cmd(TIMx, ENABLE);

}



