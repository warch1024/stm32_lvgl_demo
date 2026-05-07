#include "uart.h"
#include "stdio.h"
#include <string.h>
#include "led.h"
#include "beep.h"
#include "motor.h"
#include <stdlib.h>
#include "CO2_sensor.h"
#include "stm32f4xx.h"
#include "DFA_event_queue.h"
#include "DX_BT24.h"
#include "esp8266.h"
char uart1_cmd_cache[MAX_CMD_LEN];
uint8_t uart1_cmd_cache_idx = 0;



void usart1_init(uint32_t baudrate)
{
    // 初始化使能USART1 pa9 pa10
    // 使能USART1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    // 使能GPIOA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    // 初始化使能GPIOA
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 配置GPIOA9为USART1的AF引脚
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    // 配置GPIOA10为USART1的AF引脚
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
    // 初始化使能USART1
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = baudrate;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStruct);// 使能USART1

    //接收中断配置
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    // 使能接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    // 使能USART1
    USART_Cmd(USART1, ENABLE);
}

 int8_t usart1_send_byte(uint8_t byte)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);    // 等待发送完成
    USART_SendData(USART1, byte); // 发送字节
    // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);    // 等待发送完成
    return 1;
}

 int8_t usart1_send_string(char* str)
{
    while(*str != '\0'){
        usart1_send_byte(*str++);
    }
    return 1;
}
void USART1_IRQHandler(void)    // 串口1的中断服务函数
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)   //判断是否是接收中断发生
    {
        // 处理接收中断
        uint8_t recv_byte = USART_ReceiveData(USART1); // 接收字节
        if(USE_TRIE_OPTIMIZATION){
            Trie_Match_Byte(recv_byte);
        }else{
            DFA_Match_Byte(recv_byte);// 收到字节后，将字节添加到事件队列中        
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);  // 清除接收中断标志位
    }
}
void usart2_init(uint32_t baudrate){
    // 初始化使能USART2 pa2 pa3
    // 使能USART1时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    // 使能GPIOA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    // 初始化使能GPIOA
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 配置GPIOA2为USART1的AF引脚
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    // 配置GPIOA3为USART2的AF引脚
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
    // 初始化使能USART1
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = baudrate;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    //初始化USART2
    USART_Init(USART2, &USART_InitStruct);// 使能USART2

    //接收中断配置
    NVIC_InitTypeDef NVIC_InitStruct;
    //接收中断配置usatr2
    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    // 使能接收中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    // 使能USART2
    USART_Cmd(USART2, ENABLE);
}
inline int8_t usart2_send_byte(uint8_t byte){
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);    // 等待发送完成
    USART_SendData(USART2, byte); // 发送字节
    return 1;
}
int8_t usart2_send_bytes(uint8_t* buf,uint32_t len){
    while(len--){
        usart2_send_byte(*buf++);
    }
    return 1;
}
int8_t usart2_send_str(char* str){
    while(*str != '\0'){
        usart2_send_byte(*str++);
    }
    return 1;
}
//usart3相关功能
void usart3_init(uint32_t baudrate){
    // 初始化使能USART3 pa10 pa11
    // 使能USART3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    // 使能GPIOA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    // 初始化使能GPIOA
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 配置GPIOA9为USART1的AF引脚
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    // 配置GPIOA10为USART1的AF引脚
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
    // 初始化使能USART3
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = baudrate;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3, &USART_InitStruct);// 使能USART3

    //接收中断配置
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    // 使能接收中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    // 使能USART3
    USART_Cmd(USART3, ENABLE);
}

inline int8_t usart3_send_byte(uint8_t byte){
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);    // 等待发送完成
    USART_SendData(USART3, byte); // 发送字节
    return 1;
}

int8_t usart3_send_bytes(uint8_t* buf,uint32_t len){
    while(len--){
        usart3_send_byte(*buf++);
    }
    return 1;
}

int8_t usart3_send_str(char* str){
    while(*str != '\0'){
        usart3_send_byte(*str++);
    }
    return 1;
}









