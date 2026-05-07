#ifndef __ESP8266_MQTT_H__
#define __ESP8266_MQTT_H__

#include "stm32f4xx.h"


//此处是阿里云服务器的登陆配置-------------------------------------注意修改为自己的云服务设备信息！！！！
#define MQTT_BROKERADDRESS 		"bemfa.com"
#define MQTT_CLIENTID 			"32824fa3f6a049beb1634c0c1efddfb5"
#define MQTT_USARNAME 			"smartdevice&a10tC4OAAPc"
#define MQTT_PASSWD 			"A254BC66A055ED2FE11939F7A416A1CEB078A6B1"
#define	MQTT_PUBLISH_TOPIC 		"rec"
#define MQTT_SUBSCRIBE_TOPIC 	"zuiyanlidefuqin"


#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
	

//MQTT连接服务器
extern int32_t mqtt_connect(char *client_id,char *user_name,char *password);

//MQTT消息订阅
extern int32_t mqtt_subscribe_topic(char *topic,uint8_t qos,uint8_t whether);

//MQTT消息发布
extern uint16_t mqtt_publish_data(char *topic, char *message, uint8_t qos);

//MQTT发送心跳包
extern void mqtt_send_heart(void);

extern int32_t esp8266_mqtt_init(void);

extern void mqtt_report_devices_status(void);
void mqtt_heart_and_report(void);
#endif




