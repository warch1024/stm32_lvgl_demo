#include "led.h"
#include "stm32f4xx.h"


uint8_t LED1_state = 0;
uint8_t LED2_state = 0;
uint8_t LED3_state = 0;
uint8_t LED4_state = 0;



void led_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_led_init;
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
   
    GPIO_led_init.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_led_init.GPIO_OType = GPIO_OType_PP;
    GPIO_led_init.GPIO_Speed = GPIO_Speed_50MHz;
    //初始化LED和beep
    GPIO_led_init.GPIO_PuPd = GPIO_PuPd_DOWN; //默认下拉
    
    GPIO_led_init.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14; //LED1, LED2
    GPIO_Init(GPIOE, &GPIO_led_init);
    GPIO_led_init.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 ; //LED3, LED4
    GPIO_Init(GPIOF, &GPIO_led_init);

   LED1_OFF;
   LED2_OFF;
   LED3_OFF;
   LED4_OFF;
    
}



