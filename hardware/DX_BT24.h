#ifndef __DX_BT24_H__
#define __DX_BT24_H__
#include "stm32f4xx.h"
#define DEBUG(ch) usart3_send_byte(ch);    \
    //  usart3_send_byte('\n');

void dx_bt24_init(uint32_t baudrate);
int8_t usart3_send_byte(uint8_t byte);


#endif



