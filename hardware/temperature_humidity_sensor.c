#include "temperature_humidity_sensor.h"
#include "systick.h"
#include "stdio.h"
temperature_humidity_sensor_data current_TH_data;

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
    // 测量温度湿度传感器数据
    uint32_t timeout = 0, high_timeout = 0;
    uint8_t data[5] = {0};//    传来的数据从高到低，共40位数据5字节
    int8_t ret = temperature_humidity_sensor_check();    //复用通信头
    if(ret != 0){
        return ret;
    }
    //现在已经被拉低，时间50us左右。50us低+26-28us高 = bit0；50us低+70us高 = bit1，共40位数据5字节

    for(int i = 0; i < 40; i++, timeout = 0, high_timeout = 0){
        //读取拉低时间
        while(!TEMPERATURE_HUMIDITY_SENSOR_GPIO_IN_STATUS && timeout < 100000){
        timeout ++;
        delay_us(1);
        }
        if(timeout > 100000){// 超时
            // printf("temperature_humidity_sensor_check_response_high timeout:%d us\r\n", timeout);
            return -2;
        }
        //读取拉高时间，如果35us后还是高电平说明是bit1，否则是bit0
        while(TEMPERATURE_HUMIDITY_SENSOR_GPIO_IN_STATUS && high_timeout < 100000){
            high_timeout ++;
            delay_us(1);
        }
        if(high_timeout > 30){   //此时为bit1
            *(data + i/8) |= (1 << (7 - i % 8));
        }
        //此时被拉低
    }
    //读取完成
    current_TH_data.temperature = data[2];
    current_TH_data.humidity = data[0];
    current_TH_data.checksum = data[4];
    return 0;
}




