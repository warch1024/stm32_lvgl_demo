#include "CO2_sensor.h"
#include "uart.h"
#include <stdio.h>
#include "motor.h"
#include "USART_config.h"

static char CO2_sensor_cache[6];
static int CO2_sensor_index = 0;
int CO2_density = 0;
enum CO2_SENSOR_CONTROL_MODE CO2_sensor_control_mode = CO2_SENSOR_AUTO_CONTROL;

void CO2_sensor_init(uint32_t baudrate)
{
    usart2_init(baudrate);
}
#if defined(USART2_TO_MODULE) && (USART2_TO_MODULE == USART2_CO2_SENSOR)
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
#endif

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