#ifndef __IWDG_H__
#define __IWDG_H__
#include "stm32f4xx.h"

void IWDG_Init(uint16_t ReloadValue);
void IWDG_Reload(void);
void IWDG_CheckResetFlag(void);



#endif









