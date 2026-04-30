#include "beep.h"
#include "stm32f4xx.h"

uint8_t BEEP_state = 0;

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
