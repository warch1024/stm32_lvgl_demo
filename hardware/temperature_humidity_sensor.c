#include "temperature_humidity_sensor.h"
#include "systick.h"
#include "stdio.h"
void temperature_humidity_sensor_init(void){
    // 初始化温度湿度传感器
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
}
void temperature_humidity_set_IO_mode(GPIOMode_TypeDef mode){
    // 设置温度湿度传感器io模式
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = TEMPERATURE_HUMIDITY_SENSOR_GPIO_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = mode;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(TEMPERATURE_HUMIDITY_SENSOR_GPIO, &GPIO_InitStructure);
}



inline int8_t temperature_humidity_sensor_check(void){
    // 设置温度湿度传感器io模式
    uint32_t timeout = 0, high_timeout = 0;
    temperature_humidity_set_IO_mode(GPIO_Mode_OUT);   //设置为输出模式
    // TEMPERATURE_HUMIDITY_SENSOR_GPIO_SWITCH(HIGH);
    //低电平持续大于10ms
    TEMPERATURE_HUMIDITY_SENSOR_GPIO_OUT_LOW;
    delay_ms(20);
    //再拉高持续20-40us
    TEMPERATURE_HUMIDITY_SENSOR_GPIO_OUT_HIGH;
    delay_us(30);
    //进入接收模式等待响应
    temperature_humidity_set_IO_mode(GPIO_Mode_IN);   //设置为输入模式
    //等待被拉低,超时100ms
    while(TEMPERATURE_HUMIDITY_SENSOR_GPIO_IN_STATUS == 1 && timeout++ < 1000000){
        delay_us(1);
    }
    if(timeout > 100000){// 没有响应,超时100ms
        // printf("temperature_humidity_sensor_check_response timeout:%d us\r\n", timeout);
        return -1;
    }
    timeout = 0;
    //传感器响应低，检测拉低时间是否在80us左右
    while(!TEMPERATURE_HUMIDITY_SENSOR_GPIO_IN_STATUS && timeout < 100000){
        timeout ++;
        delay_us(1);
    }
    if(timeout > 100000){// 超时
        // printf("temperature_humidity_sensor_check_response_low timeout:%d us\r\n", timeout);
        return -2;
    }
    // 传感器响应高，检测拉高时间是否在80us左右
    while(TEMPERATURE_HUMIDITY_SENSOR_GPIO_IN_STATUS && high_timeout < 100000){
        high_timeout ++;
        delay_us(1);
    }
    if(high_timeout > 100000){// 超时
        // printf("temperature_humidity_sensor_check_response_high timeout:%d us\r\n", high_timeout);
        return -3;
    }
    return 0;   // 检查通过
}


int32_t temperature_humidity_sensor_measure(void){
    // 测量温度湿度传感器
    return 0;
}




