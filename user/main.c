#include"stm32f4xx.h"
#include"key_interrupt.h"
#include"led.h"
#include"tools.h"
#include"mq_2.h"
#include "systick.h"
#include "timer.h"
#include "beep.h"
#include "pwm.h"
#include "motor.h"
#include "uart.h"
#include "retarget.h"
#include "CO2_sensor.h"
#include "DX_BT24.h"
#include "DFA_event_queue.h"
#include "esp8266.h"
#include "USART_config.h"
#include "esp8266_mqtt.h"
#include "RS485.h"
#include "ultra_sound.h"

void init(void){
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // 初始化Trie树
	led_gpio_init();
	key_gpio_init();
    key_interrupt_init();
    // mq2_gpio_init();
    systick_init();
    // timer_init();
    beep_gpio_init();
    // timer14_pwm_init();// led1pwm
    // timer1_pwm_init();//led4pwm
    // motor1_speed_regulation_init();
    usart1_init(115200);

    // if(USART2_TO_MODULE == USART2_CO2_SENSOR){
    //     CO2_sensor_init(9600);
    // }
    // else{
    //     RS485_init();
    // }
    // if(USART3_TO_MODULE == USART3_DX_BT24){
    //     dx_bt24_init(9600);
    // }
    // else if(USART3_TO_MODULE == USART3_ESP8266){
    //     // esp8266_usart_init(115200);
    // }
    //初始化ESP8266 MQTT
    // while(esp8266_mqtt_init()){
	// 	printf("esp8266_mqtt_init ...\r\n");
	// 	delay_ms(500);
	// }
    // printf("MQTT connect to broker success\r\n");
    ultra_sound_init();



    trie_init();
}
void check_mqtt_topic_msg(void){
    uint32_t delay_1ms_cnt=0;
    //检查接收到数据
    if(g_esp8266_rx_end && g_esp8266_transparent_transmission_sta){
        for(int32_t i=0;i<g_esp8266_rx_cnt;i++){
            if(USE_TRIE_OPTIMIZATION){
                Trie_Match_Byte(g_esp8266_rx_buf[i]);
            }
            else{
                DFA_Match_Byte(g_esp8266_rx_buf[i]);// 收到字节后，将字节添加到事件队列中        
            }
        }
        //清空接收缓冲区、接收计数值、接收结束标志位
        memset((void *)g_esp8266_rx_buf,0,sizeof g_esp8266_rx_buf);
        g_esp8266_rx_cnt=0;
        g_esp8266_rx_end=0;
    }
    delay_1ms_cnt++;
    delay_ms(1);
    //1秒时间到达
    if(delay_1ms_cnt >= 1000){
        delay_1ms_cnt=0;
        mqtt_heart_and_report();
    }
}

int main(){
    init();
    char str[] = "Hello World!\n";
    // check_mqtt_topic_msg();  // 检查MQTT主题消息
    while(1){
        if(KEY1_State == 1){
            while(usart1_send_byte('X') != 1);
            while(usart1_send_string(str) != 1);
            printf("KEY1 Pressed\n");
        }
        if(KEY2_State == 1){
            if(LED2_state = ~LED2_state){
                LED2_OFF;
            }
            else{
                LED2_ON;
            }
        }
        if(KEY3_State == 1){
            int32_t distance = ultra_sound_measure();
            if(distance >= 0){
                printf("distance = %d mm\r\n", distance);
            }
            else{
                printf("ultra_sound_measure fail\r\n");
            }
        }
        //根据CO2浓度设置通风速度比例
        run_event_task();//运行事件队列中的事件
        clear_key_state();//清除按键状态
    }
}


