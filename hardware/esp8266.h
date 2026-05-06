
#ifndef __ESP8266_H__
#define __ESP8266_H__
#include "stm32f4xx.h"

void esp8266_init(uint32_t baudrate);
int8_t send_byte_to_esp8266(uint8_t byte);
int8_t send_string_to_esp8266(char* str);
void esp8266_net_init(void);
#endif



