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
    motor1_speed_regulation_init();
    usart_init(115200);
    CO2_sensor_init(9600);
    if(USART3_TO_MODULE == DX_BT24){
        dx_bt24_init(9600);
    }
    else if(USART3_TO_MODULE == ESP8266){
        esp8266_init(115200);
    }
    trie_init();
}


int main(){
    init();
    char str[] = "Hello World!\n";
    while(1){
        if(KEY1_State == 1){
            while(usart1_send_byte('X') != 1);
            while(usart1_send_string(str) != 1);
            printf("KEY1 Pressed\n");
        }
        if(KEY2_State == 1){
            motor1_set_duty(0.5 * MOTOR_TIM_PERIOD);
            TIM_SetCompare1(TIM14, 0.5 * TIM14_TIM_PERIOD);
        }
        if(KEY3_State == 1){
            motor1_set_duty(0.15 * MOTOR_TIM_PERIOD);
            TIM_SetCompare1(TIM14, 0.15 * TIM14_TIM_PERIOD);
        }
        if(KEY4_State == 1){
            motor1_set_duty(0 * MOTOR_TIM_PERIOD);
            TIM_SetCompare1(TIM14, 0 * TIM14_TIM_PERIOD);
        }
        //根据CO2浓度设置通风速度比例
        run_event_task();//运行事件队列中的事件
        clear_key_state();//清除按键状态
    }
}


