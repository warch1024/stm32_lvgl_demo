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
#include "radar.h"
#include "temperature_humidity_sensor.h"
#include "IWDG.h"
#include "RTC.h"
#include "Flash.h"
#include "eeprom.h"
#include "oled_iic.h"
#include "spi_flash.h"
#include "SPI.h"
#include "RC522.h"
#include "rc522_app.h"
#include "SPI_LCD.h"
// #include "lcd_demo.h"
#include "lcd.h"

#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_demo_widgets.h"
#include "touch.h"



void init(void){
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// led_gpio_init();
    systick_init();
    usart1_init(115200);
    // LCD_Init();
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    lv_demo_widgets();
    lvgl_tick_init();
    printf("init done \r\n");
}


int main(){
    init();
    while(1){
        tp_dev.scan(0);
        lv_task_handler();
        delay_ms(5);
    }
    return 0;
}


