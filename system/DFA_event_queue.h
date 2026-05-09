#ifndef __DFA_EVENT_QUEUE_H__
#define __DFA_EVENT_QUEUE_H__
#include "stm32f4xx.h"

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
    EVT_CO2_GET_VALUE,   // 无参数
    EVT_ESP8266_INIT,   // 无参数
    EVT_SUBMIT_TIME_HOUR, //带参数
    EVT_SUBMIT_TIME_MIN, //带参数
    EVT_SUBMIT_TIME_SEC, //带参数
    EVT_SUBMIT_DATE_YEAR, //带参数
    EVT_SUBMIT_DATE_MONTH, //带参数
    EVT_SUBMIT_DATE_DAY, //带参数
    EVT_SUBMIT_DATE_WEEK, //带参数
    EVT_SET_TIME, //不带参数
    EVT_SET_DATE, //不带参数
    EVT_SUBMIT_ALARM_HOUR, //带参数
    EVT_SUBMIT_ALARM_MIN, //带参数
    EVT_SUBMIT_ALARM_SEC, //带参数
    EVT_SET_ALARM, //不带参数
    EVT_COUNT, // 事件总类型数量
} event_type_t;

#define CMD_COUNT EVT_COUNT

#if (CMD_COUNT >= 10)
    #define USE_TRIE_OPTIMIZATION 1
#endif
#define USE_TRIE_OPTIMIZATION 1
typedef struct DFA_Cmd_t{
    const char *cmd;      // 命令前缀
    uint8_t has_param;    // 1=带参数 0=不带参数
} DFA_cmd_t;

//事件处理函数类型
typedef void (*event_handler_t)(int);
//事件队列结构体
typedef struct Event_Queue_t{
    event_handler_t evt;
    int param_val;
    struct Event_Queue_t *next;
}event_queue_t;
// Trie树节点结构体
// 优化后：95个指针（126 - 32 + 1 = 95）
#define TRIE_CHAR_MIN 32  // 空格
#define TRIE_CHAR_MAX 126 // ~
#define TRIE_CHAR_RANGE (TRIE_CHAR_MAX - TRIE_CHAR_MIN + 1)  // 95
typedef struct Trie_Node_t {
    struct Trie_Node_t *children[TRIE_CHAR_RANGE];  // 95个可见ASCII字符
    event_type_t event;              // 匹配成功时的事件类型
    uint8_t has_param;               // 是否带参数
    uint8_t is_end;                  // 是否为命令结束节点
} trie_node_t;

void Trie_Match_Byte(uint8_t ch);
void DFA_Match_Byte(uint8_t ch);
int8_t run_event_task(void);
void trie_init(void);
void event_push_interface(event_handler_t evt, int param_val);//将事件推送到事件队列接口



#endif


