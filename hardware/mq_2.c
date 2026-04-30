#include "mq_2.h"
#include "stm32f4xx.h"
#include "led.h"
#include "tools.h"

void mq2_gpio_init(void)
{
    //初始化MQ-2传感器
    GPIO_InitTypeDef GPIO_mq_2_init;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    GPIO_mq_2_init.GPIO_Mode = GPIO_Mode_IN;
    GPIO_mq_2_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_mq_2_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_mq_2_init.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_mq_2_init);
}
int read_mq_2(void){
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9);
}

void show_mq_2(void){
    if(read_mq_2() == 0){
        LED_SWITCH(4, OFF);
        delay(100);
        LED_SWITCH(4, ON);
        delay(100);
    }
}
