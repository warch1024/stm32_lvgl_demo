#ifndef __SYSTICK_H__
#define __SYSTICK_H__
#include "stm32f4xx.h"

void systick_init(void);
// void delay_ms(uint32_t ms);
// void delay_us(uint32_t us);
static inline void delay_ms(uint32_t ms)
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//关闭计数器
    SysTick->LOAD = 21000 * ms - 1;//加载21M时钟的ms时间
    SysTick->VAL = 0;   //重置计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//使能计数器
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0); //等待计数器到0
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭计数器
}
static inline void delay_us(uint32_t us)
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//关闭计数器
    SysTick->LOAD = 21 * us - 1;//加载21M时钟的us时间
    SysTick->VAL = 0;   //重置计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//使能计数器
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0); //等待计数器到0
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭计数器
}
#endif


