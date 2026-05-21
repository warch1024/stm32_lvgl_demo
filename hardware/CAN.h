#ifndef __CAN_H__
#define __CAN_H__
#include "stm32f4xx.h"

void CAN_init(void);
u8 can1_send_message(u8 *data, u8 len, u32 message_id);

#endif



