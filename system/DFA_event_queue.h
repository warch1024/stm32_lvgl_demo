#ifndef __DFA_EVENT_QUEUE_H__
#define __DFA_EVENT_QUEUE_H__
#include "stm32f4xx.h"


typedef struct DFA_Cmd_t{
    const char *cmd;      // 命令前缀
    uint8_t has_param;    // 1=带参数 0=不带参数
} DFA_cmd_t;

// 命令对应的所有事件定义
typedef enum Event_Type_t{
    EVT_DEFAULT = 0,
    EVT_LED_ON, //带参数
    EVT_LED_OFF, //带参数
    EVT_BEEP_ON, //带参数
    EVT_BEEP_OFF, //带参数
    EVT_FAN_ON, //带参数
    EVT_FAN_OFF, //带参数
    EVT_FAN_SPEED,   // 带参数
    EVT_CO2_AUTO, //带参数
    EVT_CO2_OFF,    //带参数
} event_type_t;
//事件处理函数类型
typedef void (*event_handler_t)(int);
//事件队列结构体
typedef struct Event_Queue_t{
    event_handler_t evt;
    int param_val;
    struct Event_Queue_t *next;
}event_queue_t;

void Match_Event_Handler(event_type_t evt, int param_val);
void DFA_Match_Byte(uint8_t ch);
void Event_Push(event_handler_t evt, int param_val, 
    event_queue_t **event_queue, event_queue_t **event_queue_tail);
int8_t run_event_task(void);

#endif


