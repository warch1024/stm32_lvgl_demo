#include "timer.h"
#include "stm32f4xx.h"
#include "led.h"
#include "tools.h"
#include "beep.h"
#include "mq_2.h"
#include "esp8266_mqtt.h"

// 初始化LVGL心跳定时器
void lvgl_tick_init(void){
    //时钟使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseInitTypeDef timer_init_struct;
    // 初始化定时器3
    timer_init_struct.TIM_Prescaler = 8400 -1;//84MHz / 8400 = 10KHz
    timer_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
    timer_init_struct.TIM_CounterMode = TIM_CounterMode_Up;
    timer_init_struct.TIM_Period = 10 - 1;// （1/10K）* 10 = 1ms
    TIM_TimeBaseInit(TIM3, &timer_init_struct);

    NVIC_InitTypeDef nvic_init_struct;
    // 初始化中断通道3
    nvic_init_struct.NVIC_IRQChannel = TIM3_IRQn;  // 更新中断通道
    nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init_struct.NVIC_IRQChannelSubPriority = 0;
    nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init_struct);

    // 初始化中断方式
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);  // 使能更新中断
    
    // 开启定时器3
    TIM_Cmd(TIM3, ENABLE);  // 使能定时器3
}

//增加LVGL心跳配置

// 定时器3更新中断服务函数
void TIM3_IRQHandler(void)
{
    // 处理定时器3更新中断
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        lv_tick_inc(1);
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  // 清除更新中断标志位
}


void timer_init(void)
{
    //时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

    // 初始化定时器1
    TIM_TimeBaseInitTypeDef timer_init_struct;
    timer_init_struct.TIM_Prescaler = 16800 -1;//168M / 16800 = 10KHz
    timer_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
    timer_init_struct.TIM_CounterMode = TIM_CounterMode_Up;
    timer_init_struct.TIM_Period = 10000 - 1;// （1/10K）* 10K = 1s
    TIM_TimeBaseInit(TIM1, &timer_init_struct);
    // 初始化定时器2
    timer_init_struct.TIM_Prescaler = 8400 -1;//84MHz / 8400 = 10KHz
    timer_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
    timer_init_struct.TIM_CounterMode = TIM_CounterMode_Up;
    timer_init_struct.TIM_Period = 3000 - 1;// （1/10K）* 3K = 300ms
    TIM_TimeBaseInit(TIM2, &timer_init_struct);
    // 初始化定时器3
    timer_init_struct.TIM_Prescaler = 8400 -1;//84MHz / 8400 = 10KHz
    timer_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
    timer_init_struct.TIM_CounterMode = TIM_CounterMode_Up;
    timer_init_struct.TIM_Period = 5000 - 1;// （1/10K）* 5K = 500ms
    TIM_TimeBaseInit(TIM3, &timer_init_struct);
    // 初始化定时器8
    timer_init_struct.TIM_Prescaler = 16800 -1;//168M / 16800 = 10KHz
    timer_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
    timer_init_struct.TIM_CounterMode = TIM_CounterMode_Up;
    timer_init_struct.TIM_Period = 20000 - 1;// （1/10K）* 20K = 2s
    TIM_TimeBaseInit(TIM8, &timer_init_struct);

    // 初始化中断通道1
    NVIC_InitTypeDef nvic_init_struct;
    nvic_init_struct.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;  // 更新中断通道
    nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init_struct.NVIC_IRQChannelSubPriority = 0;
    nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init_struct);
    // 初始化中断通道2
    nvic_init_struct.NVIC_IRQChannel = TIM2_IRQn;  // 更新中断通道
    nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 01;
    nvic_init_struct.NVIC_IRQChannelSubPriority = 0;
    nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init_struct);
    // 初始化中断通道3
    nvic_init_struct.NVIC_IRQChannel = TIM3_IRQn;  // 更新中断通道
    nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 02;
    nvic_init_struct.NVIC_IRQChannelSubPriority = 0;
    nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init_struct);
    // 初始化中断通道8
    nvic_init_struct.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;  // 更新中断通道
    nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 0x00;
    nvic_init_struct.NVIC_IRQChannelSubPriority = 0;
    nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init_struct);
    // 初始化中断方式
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);  // 使能更新中断
    // TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  // 使能更新中断
    // TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);  // 使能更新中断
    // TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);  // 使能更新中断
    
    // 开启定时器1,2,3,8
    TIM_Cmd(TIM1, ENABLE);  // 使能定时器1
    // TIM_Cmd(TIM2, ENABLE);  // 使能定时器2
    // TIM_Cmd(TIM3, ENABLE);  // 使能定时器3
    // TIM_Cmd(TIM8, ENABLE);  // 使能定时器8
}


// 定时器1更新中断服务函数
void TIM1_UP_TIM10_IRQHandler(void)
{
    // 处理定时器1更新中断
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {

        // 发送心跳包
        //  mqtt_heart_and_report();
    }
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  // 清除更新中断标志位
}
// 定时器2更新中断服务函数
void TIM2_IRQHandler(void)
{
    // 处理定时器2更新中断
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        // 处理更新中断
        if(LED2_state = ~LED2_state){
            LED2_OFF;
        }
        else{
            LED2_ON;
        }
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  // 清除更新中断标志位
}


// 定时器8更新中断服务函数
void TIM8_UP_TIM13_IRQHandler(void)
{
    // 处理定时器8更新中断
    if(TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
    {
        // 处理更新中断
        if(read_mq_2() == 0){
            BEEP_SWITCH(1, ON);
        }
        else{
            BEEP_SWITCH(1, OFF);
        }
    }
    TIM_ClearITPendingBit(TIM8, TIM_IT_Update);  // 清除更新中断标志位
}


