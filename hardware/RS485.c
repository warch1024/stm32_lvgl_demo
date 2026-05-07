#include "RS485.h"
#include "stm32f4xx.h"
#include "USART_config.h"
#include "uart.h"
#include "systick.h"
void RS485_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    usart2_init(115200);
    ERR_MSG("RS485_init success\r\n");
}
void rs485_send_bytes(uint8_t *bytes, uint16_t len){
    RS485_TX_ENABLE;
    delay_ms(1);
    usart2_send_bytes(bytes, len);
    delay_ms(1);
    RS485_RX_ENABLE;
    delay_ms(1);
}

#if defined(USART2_TO_MODULE) && (USART2_TO_MODULE == USART2_RS485)
//RS485模块使用串口2传输数据
void USART2_IRQHandler(void)    // 串口2的中断服务函数
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET){  // 判断是否是接收中断发生
        // 处理接收中断
        uint16_t recv_byte = USART_ReceiveData(USART2);  // 接收字节
        usart1_send_byte(recv_byte);//将接收字节发送到USART1
        // ...
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);  // 清除接收中断标志位
    }
}
#endif


