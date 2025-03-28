#ifndef __ESP8266_
#define __ESP8266_

void ESP8266_connect_wifi(char* ssid,char* passward);
void ESP8266_connect_mqtt(void);
void send_data(uint8_t tempX,uint8_t humi,uint8_t voice,uint8_t rader,uint8_t mqtt,uint8_t bee_state,uint8_t fire,uint8_t smoke,uint8_t tempZ);
void send_check_in(uint8_t check_id);
void send_check_off(uint8_t check_id);
#endif
