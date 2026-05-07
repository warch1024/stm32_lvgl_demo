#ifndef __ULTRA_SOUND_H__
#define __ULTRA_SOUND_H__
#include "stm32f4xx.h"

#define ULTRA_SOUND_TRIGGER         GPIO_SetBits(GPIOE, GPIO_Pin_6);
#define ULTRA_SOUND_TRIGGER_CLEAR   GPIO_ResetBits(GPIOE, GPIO_Pin_6);
#define ULTRA_SOUND_ECHO_STATUS     (GPIOA->IDR & GPIO_Pin_8)

//参数：TRIGGER和TRIGGER_CLEAR
#define ULTRA_SOUND_TRIGGER_SWITCH(state)   ULTRA_SOUND_##state     // 切换超声波传感器触发引脚状态

void ultra_sound_init(void);
int32_t ultra_sound_measure(void);







#endif


