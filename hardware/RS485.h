#ifndef __RS485_H__
#define __RS485_H__
#include "stm32f4xx.h"

#define RS485_RX_ENABLE GPIO_ResetBits(GPIOG, GPIO_Pin_8);
#define RS485_TX_ENABLE GPIO_SetBits(GPIOG, GPIO_Pin_8);

#define RS485_TX_RX_SWITCH(state) RS485_##state##_ENABLE
void RS485_init(void);
void rs485_send_bytes(uint8_t *bytes, uint16_t len);

#endif


