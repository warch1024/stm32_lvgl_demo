#include "systick.h"
#include "stm32f4xx.h"

void systick_init(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//使用21M的时钟
}

void delay_ms(uint32_t ms)
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//关闭计数器
    SysTick->LOAD = 21000 * ms - 1;//加载21M时钟的ms时间
    SysTick->VAL = 0;   //重置计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//使能计数器
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0); //等待计数器到0
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭计数器
}
void delay_us(uint32_t us)
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//关闭计数器
    SysTick->LOAD = 21 * us - 1;//加载21M时钟的us时间
    SysTick->VAL = 0;   //重置计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//使能计数器
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0); //等待计数器到0
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭计数器
}


