#ifndef __UART_H__
#define __UART_H__
#include "stm32f4xx.h"
#define MAX_CMD_LEN 100
#define ERR_MSG(msg)  usart1_send_string(msg)
void usart1_init(uint32_t baudrate);

void usart3_init(uint32_t baudrate);
int8_t usart3_send_byte(uint8_t byte);
int8_t usart3_send_bytes(uint8_t* buf,uint32_t len);
int8_t usart3_send_str(char* str);

void usart2_init(uint32_t baudrate);
int8_t usart2_send_byte(uint8_t byte);
int8_t usart2_send_bytes(uint8_t* buf,uint32_t len);
int8_t usart2_send_str(char* str);

int8_t usart1_send_byte(uint8_t byte);
int8_t usart1_send_string(char* str);

#endif


