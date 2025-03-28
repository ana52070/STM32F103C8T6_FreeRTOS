#include "stm32f10x.h"                  // Device header
#include "ESP8266.h"
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"
char* data;
void ESP8266_connect_wifi(char* ssid,char* passward)
{
	Serial_Init();
	Delay_s(1);
	Serial_SendString("AT+RST\r\n");
	Delay_s(1);
	Serial_SendString("AT+CWMODE=1\r\n");
	Delay_s(1);
	Serial_SendString("AT+CIPSNTPCFG=1,8,\"cn.ntp.org.cn\",\"ntp.sjtu.edu.cn\"\r\n");
	Delay_s(1);
	Serial_SendString("AT+CWJAP=\"");
	Serial_SendString(ssid);
	Serial_SendString("\",\"");
	Serial_SendString(passward);
	Serial_SendString("\"\r\n");
	Delay_s(1);
	Delay_s(1);
	Delay_s(1);

}
void ESP8266_connect_mqtt(void)
{
	Serial_SendString("AT+MQTTUSERCFG=0,1,\"NULL\",\"SmartLab-mcu&k0lrjXSIdGe\",\"b10317109493ce70ea4f9aa3a709ec64cd434bdbb2d8e90f90069ae4d25376f0\",0,0,\"\"\r\n");
	Delay_s(1);
	Serial_SendString("AT+MQTTCLIENTID=0,\"k0lrjXSIdGe.SmartLab-mcu|securemode=2\\,signmethod=hmacsha256\\,timestamp=1727445963404|\"\r\n");
	Delay_s(1);
	Serial_SendString("AT+MQTTCONN=0,\"iot-06z00b4dgddc29n.mqtt.iothub.aliyuncs.com\",1883,1\r\n");
	Delay_s(1);
}

void send_data(uint8_t tempX,uint8_t humi,uint8_t voice,uint8_t rader,uint8_t mqtt,uint8_t bee_state,uint8_t fire,uint8_t smoke,uint8_t tempZ)
{
	char send_str[200] = {0};
	sprintf(send_str,"AT+MQTTPUB=0,\"/sys/k0lrjXSIdGe/SmartLab-mcu/thing/event/property/post\",\"{\\\"params\\\":{\\\"fire\\\":%d\\,\\\"bee_state\\\":%d\\,\\\"mqtt\\\":%d\\,\\\"rader\\\":%d\\,\\\"MQ2\\\":%d\\,\\\"voice\\\":%d\\,\\\"humi\\\":%d\\,\\\"temp\\\":%d\\,\\\"phone\\\":%d}}\",1,0\r\n",fire,bee_state,mqtt,rader,smoke,voice,humi,tempZ,tempX);
	Serial_SendString(send_str);
	//Serial_SendString("\"\r\n");
}

void send_check_in(uint8_t check_id)
{
	char send_str[110] = {0};
	sprintf(send_str,"AT+MQTTPUB=0,\"/sys/k0lrjXSIdGe/SmartLab-mcu/thing/event/property/post\",\"{\\\"params\\\":{\\\"check_in\\\":%d}}\",1,0\r\n",check_id);
	Serial_SendString(send_str);
	//Serial_SendString("\"\r\n");
}

void send_check_off(uint8_t check_id)
{
	char send_str[110] = {0};
	sprintf(send_str,"AT+MQTTPUB=0,\"/sys/k0lrjXSIdGe/SmartLab-mcu/thing/event/property/post\",\"{\\\"params\\\":{\\\"check_off\\\":%d}}\",1,0\r\n",check_id);
	Serial_SendString(send_str);
	//Serial_SendString("\"\r\n");
}

