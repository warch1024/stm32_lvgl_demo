#include "CO2_sensor.h"
#include "uart.h"
#include <stdio.h>
#include "motor.h"

static char CO2_sensor_cache[6];
static int CO2_sensor_index = 0;
int CO2_density = 0;
enum CO2_SENSOR_CONTROL_MODE CO2_sensor_control_mode = CO2_SENSOR_AUTO_CONTROL;

void CO2_sensor_init(uint32_t baudrate)
{
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
//CO2传感器使用串口2传输数据
void USART2_IRQHandler(void)    // 串口2的中断服务函数
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)   //判断是否是接收中断发生
    {
        // 处理接收中断
        uint16_t recv_byte = USART_ReceiveData(USART2); // 接收字节
        CO2_sensor_cache[CO2_sensor_index++] = recv_byte;
        if(CO2_sensor_index >= 6){
            CO2_sensor_index = 0;
            //校验数据
            if((uint8_t)CO2_sensor_cache[5]== (uint8_t)(CO2_sensor_cache[0] + CO2_sensor_cache[1] +
                 CO2_sensor_cache[2] + CO2_sensor_cache[3] + CO2_sensor_cache[4])){
                CO2_density = CO2_sensor_cache[1]*256 + CO2_sensor_cache[2];
                CO2_sensor_ventilation_control(CO2_density);//根据CO2浓度设置通风速度比例
            }
            else{
                CO2_density = -1;
            }
        // 处理接收字节
        // ...
        }
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);  // 清除接收中断标志位
    }
}

void CO2_sensor_ventilation_control(int CO2_density){
    if(CO2_sensor_control_mode == CO2_SENSOR_AUTO_CONTROL){
        // 自动模式
        //根据CO2浓度设置通风速度比例
        if(CO2_density >= 1000){
            ventilation(1);
        }
        else if (CO2_density < 500){
            ventilation(0.15);
        }
        else{
            ventilation(CO2_density / 1000.0f);
        }
    }
    else {
        // 传感器不调节通风
    }
}