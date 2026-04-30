#ifndef __CO2_SENSOR_H__
#define __CO2_SENSOR_H__
#include "stm32f4xx.h"


// extern char CO2_sensor_cache[6];
// extern int CO2_sensor_index;
extern int CO2_density;
extern enum CO2_SENSOR_CONTROL_MODE CO2_sensor_control_mode;

enum CO2_SENSOR_CONTROL_MODE{
    CO2_SENSOR_AUTO_CONTROL = 0,
    CO2_SENSOR_OFF_CONTROL = 1,
};
extern char CO2_sensor_mode;
void CO2_sensor_init(uint32_t baudrate);
void CO2_sensor_ventilation_control(int CO2_density);
#endif


