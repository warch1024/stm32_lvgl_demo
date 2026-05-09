#ifndef __EVENT_HANDLERS_H__
#define __EVENT_HANDLERS_H__

void default_handler(int param_val);
void led_on(int param_val);
void led_off(int param_val);
void beep_on(int param_val);
void beep_off(int param_val);
void fan_on(int param_val);
void fan_off(int param_val);
void fan_speed(int param_val);
void co2_auto(int param_val);
void co2_off(int param_val);
void co2_get_value(int param_val);
void esp8266_net_init_task(int param_val);

void submit_time_hour(int param_val);
void submit_time_min(int param_val);
void submit_time_sec(int param_val);
void submit_date_year(int param_val);
void submit_date_month(int param_val);
void submit_date_day(int param_val);
void submit_date_week(int param_val);
void set_time(int param_val);
void set_date(int param_val);
void submit_alarm_hour(int param_val);
void submit_alarm_min(int param_val);
void submit_alarm_sec(int param_val);
void set_alarm(int param_val);
void alarm_handler(int param_val);

#endif


