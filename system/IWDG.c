#include "IWDG.h"
#include "stm32f4xx.h"
#include "stdio.h"

void IWDG_Init(uint16_t ReloadValue){
    //独立看门狗是受到保护的,现在进行解锁动作 
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    //设置看门狗的时钟=32KHz/ 256=125Hz 
    IWDG_SetPrescaler(IWDG_Prescaler_256);

    //设置看门狗的超时时间,也就是设置它的计数值
    // 当前看门狗的时钟为125Hz,然后设置超时时间为1秒,那么重载值为125-1
    // 当前看门狗的时钟为125Hz,然后设置超时时间为2秒,那么重载值为250-1
    IWDG_SetReload(ReloadValue - 1);
    //Reload IWDG counter,重载独立看门狗的计数值,说白了就是喂狗 
    IWDG_ReloadCounter();
    //Enable IWDG (the LSI oscillator will be enabled by hardware),使能独立看门狗 
    IWDG_Enable();
    IWDG_Reload();
}

void IWDG_Reload(void){
    //喂狗
    IWDG_ReloadCounter();
}

void IWDG_CheckResetFlag(void){
        //检查当前复位是否有独立看门狗导致
    /* 检查当前复位是否有独立看门狗导致 */

    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)
        printf("iwdg reset cpu\r\n");
    else
        printf("normal reset cpu\r\n");
    // 清除复位标志位
    RCC_ClearFlag();
}





