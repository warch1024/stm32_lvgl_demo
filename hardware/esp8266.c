#include "esp8266.h"
#include "stm32f4xx.h"
#include "uart.h"
#include <string.h>
#include "DFA_event_queue.h"
#include "tools.h"

static char uart3_cmd_cache[MAX_CMD_LEN];
static uint8_t uart3_cmd_cache_idx = 0;

void esp8266_init(uint32_t baudrate)
{
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
#include "USART_config.h"
#if defined(USART3_TO_MODULE) && (USART3_TO_MODULE == ESP8266)
// 连接ESP8266模块
void USART3_IRQHandler(void)    // 串口3的中断服务函数
{
    if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)   //判断是否是接收中断发生
    {
        // 处理接收中断
        uint16_t recv_byte = USART_ReceiveData(USART3); // 接收字节
        usart1_send_byte(recv_byte);
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);  // 清除接收中断标志位
    }
}
#endif


int8_t send_byte_to_esp8266(uint8_t byte)
{
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);    // 等待发送完成
    USART_SendData(USART3, byte); // 发送字节
    return 1;
}

int8_t send_string_to_esp8266(char* str)
{
    while(*str != '\0'){
        send_byte_to_esp8266(*str++);
    }
    return 1;
}


void esp8266_net_init(void)
{
	delay(2000);
    send_string_to_esp8266( "+++");
	delay(2000);
	send_string_to_esp8266("AT+RST\r\n");
	delay(3000);
	
	//设置工作模式为STA
	send_string_to_esp8266("AT+CWMODE=3\r\n");
	delay(3000);
	
	// //连接WiFi
	// send_string_to_esp8266("AT+CWJAP=\"microsoftWindowsPhone\",\"15550073646\"\r\n");
	// delay(10000);
	
	//连接TCP服务器 10.249.157.105
	// send_string_to_esp8266("AT+CIPSTART=\"TCP\",\"10.6.26.179\",8888\r\n");
	// delay(5000);
	
	//设置IP模式为数据
	send_string_to_esp8266("AT+CIPMODE=1\r\n");
	delay(3000);
	
	//发送数据
	send_string_to_esp8266("AT+CIPSEND\r\n");
	delay(3000);
}
