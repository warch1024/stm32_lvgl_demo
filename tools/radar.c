#include "radar.h"
#include "led.h"
#include "ultra_sound.h"
#include "beep.h"
#include "pwm.h"

void radar_init(void){
    // 初始化雷达
    ultra_sound_init(); 
    led_gpio_init();
    beep_gpio_init();
    beep_pwm_init();
}
void radar_distance_show(void){
    // 显示测量的雷达距离
    int32_t distance = -1;
    distance = ultra_sound_measure();
    if(distance>=200){
        LED1_OFF;
        LED2_OFF;
        LED3_OFF;
        LED4_OFF;
        return;
    }
    if(distance>=150 && distance<200){
        LED1_ON;
        LED2_OFF;
        LED3_OFF;
        LED4_OFF;
        return;
    }
    if(distance>=100 && distance<150){
        LED1_ON;
        LED2_ON;
        LED3_OFF;
        LED4_OFF;
        return;
    }
    if(distance>=50 && distance<100){
        LED1_ON;
        LED2_ON;
        LED3_ON;
        LED4_OFF;
        return;
    }
    if(distance<50){
        LED1_ON;
        LED2_ON;
        LED3_ON;
        LED4_ON;
        return;
    }
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    LED4_OFF;
}

void radar_distance_beeping(void){
    // 测量雷达距离并蜂鸣
    static uint8_t radar_frequency = 2;

    int32_t distance = -1;
    distance = ultra_sound_measure();
    distance -= 20;
    if(distance < 0){
        BEEP_DUTY_PERCENT(100);
    }
    else{
        BEEP_DUTY_PERCENT(100 - (150 - distance) / 150);
       }
}





