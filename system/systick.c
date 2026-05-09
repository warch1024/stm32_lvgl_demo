#include "systick.h"
#include "stm32f4xx.h"

void systick_init(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//使用21M的时钟
}




