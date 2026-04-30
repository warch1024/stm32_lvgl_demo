#pragma import (__use_no_semihosting_swi)
#include "retarget.h"
#include "stm32f4xx.h"
#include "uart.h"
 #include <stdio.h>
// #include <stdlib.h>

struct __FILE __stdout;
struct __FILE __stdin;
int fputc(int ch, FILE* f)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);
    USART_SendData(USART1, ch);
    return ch;
}
void _ttywrch(int ch)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);
    USART_SendData(USART1, ch);
}

void _sys_exit(int return_code)
{
    label : goto label;
}

