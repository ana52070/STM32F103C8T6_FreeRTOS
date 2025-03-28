#ifndef __ALIYUN_MQTT_H
#define __ALIYUN_MQTT_H


#include "stm32f10x.h"                  // Device header


void aliyun_mqtt_Init(void);
void aliyun_send_data(uint8_t tempX,uint8_t humi,uint8_t voice,uint8_t rader,uint8_t mqtt,uint8_t bee_state,uint8_t fire,uint8_t smoke,uint8_t tempZ);
uint8_t extractCoordinates(const char *input, double *latitude, double *longitude);
uint8_t extractLocation(const char *input, double *latitude, double *longitude) ;
uint8_t findValue(const char *input, const char *key, int *value);
void aliyun_send_check_in(uint8_t check_id);
void aliyun_send_check_off(uint8_t check_id);


#endif
