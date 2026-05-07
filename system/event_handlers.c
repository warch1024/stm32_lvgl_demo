#include "event_handlers.h"
#include "led.h"
#include "beep.h"
#include "motor.h"
#include "CO2_sensor.h"
#include "uart.h"
#include "stdio.h"
#include "esp8266.h"

void default_handler(int param_val){
    // ... 你可以自己加
    ERR_MSG("Default Handler\r\n");
}
void led_on(int param_val){
    // ... 你可以自己加
    switch(param_val){
        case 1: LED_SWITCH(1, ON); break;
        case 2: LED_SWITCH(2, ON); break;
        case 3: LED_SWITCH(3, ON); break;
        case 4: LED_SWITCH(4, ON); break;
        default: break;
    }
}
void led_off(int param_val){
    // ... 你可以自己加
    switch(param_val){
        case 1: LED_SWITCH(1, OFF); break;
        case 2: LED_SWITCH(2, OFF); break;
        case 3: LED_SWITCH(3, OFF); break;
        case 4: LED_SWITCH(4, OFF); break;
        default: break;
    }
}
void beep_on(int param_val){
    // ... 你可以自己加
    switch(param_val){
        case 1: BEEP_SWITCH(1, ON); break;
        // case 2: BEEP_SWITCH(2, ON); break;
        // case 3: BEEP_SWITCH(3, ON); break;
        // case 4: BEEP_SWITCH(4, ON); break;
        default: break;
    }
}
void beep_off(int param_val){
    // ... 你可以自己加
    switch(param_val){
        case 1: BEEP_SWITCH(1, OFF); break;
        // case 2: BEEP_SWITCH(2, OFF); break;
        // case 3: BEEP_SWITCH(3, OFF); break;
        // case 4: BEEP_SWITCH(4, OFF); break;
        default: break;
    }
}

void fan_on(int param_val){
    // ... 你可以自己加
    switch(param_val){
        case 1: MOTOR_SWITCH(1, ON); break;
        default: break;
    }
}
void fan_off(int param_val){
    // ... 你可以自己加
    switch(param_val){
        case 1: MOTOR_SWITCH(1, OFF); break;
        default: break;
    }
}
void fan_speed(int param_val){
    // ... 你可以自己加
    MOTOR1_SPEED_PERCENT(param_val);

}
void co2_auto(int param_val){
    // ... 你可以自己加
    CO2_sensor_control_mode = CO2_SENSOR_AUTO_CONTROL;
    ERR_MSG("CO2 Sensor Auto Control\r\n");
}
void co2_off(int param_val){
    // ... 你可以自己加
    CO2_sensor_control_mode = CO2_SENSOR_OFF_CONTROL;
    ERR_MSG("CO2 Sensor Off Control\r\n");
}

void co2_get_value(int param_val){
    // ... 你可以自己加
    if(CO2_sensor_control_mode == CO2_SENSOR_AUTO_CONTROL){
        usart1_send_string("CO2 Value: ");
        printf("%d", CO2_density);
        usart1_send_string("\r\n");
    }
    else{
        usart1_send_string("CO2 sensor was off");
        usart1_send_string("\r\n");
    }
}
void esp8266_net_init_task(int param_val){
    // ... 你可以自己加

}





