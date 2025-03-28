#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Serial.h"
void aliyun_mqtt_Init(void)
{
	//ģ���ʼ��
	NBIOT_Init();
	
	//1)ATָ��
	while(NBIOT_SendCmd("ATE0", "OK", 200))          //�رջ���
		Delay_ms(500);
	
	Delay_ms(300);
	
	
	
	//MQTT��ʼ��
	Delay_ms(300);
	
	while(NBIOT_SendCmd("AT+CSTT=\"\",\"\",\"\"", "OK", 200))    
		Delay_ms(500);
	
	Delay_ms(300);
	
	while(NBIOT_SendCmd("AT+CIICR","OK", 200))    
		Delay_ms(500);
	
	Delay_ms(300);
	
	while(NBIOT_SendCmd("AT+MCONFIG=\"FESA234FBDS24|securemode=3,signmethod=hmacsha1,timestamp=789|\",\"SmartLab-mcu&k0lrjXSIdGe\",\"61c9b9aa12f55ee0ff04c9800b4941aac037dc37\""
,"OK", 200))    
		Delay_ms(500);
	
	Delay_ms(300);
	
	while(NBIOT_SendCmd("AT+MIPSTART=\"k0lrjXSIdGe.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883","OK", 200))    
		Delay_ms(500);
	
	Delay_ms(500);
	while(NBIOT_SendCmd("AT+MCONNECT=0,600","OK", 200))
		Delay_ms(500); 
	
	
}


//����ϢATָ�
//AT+MPUB="/sys/k0lrjQ3ceCX/mcumcu/thing/event/property/post",0,0,"{\\22id\\22:1722878948774,\\22params\\22:{\\22dw_type\\22:1,\\22GPS_JD\\22:66.6666,\\22GPS_WD\\22:33.3333},\\22version\\22:\\221.0\\22,\\22method\\22:\\22thing.event.property.post\\22}"
//AT+MPUB="/sys/k0lrjXSIdGe/SmartLab-mcu/thing/event/property/post",0,0,"{\\22id\\22:1726403120237,\\22params\\22:{\\22temp\\22:%f,\\22humi\\22:%d,\\22voice\22:%d,\\22rader\22:%d,\\22mqtt\22:%d,\\22bee_state\22:%d},\\22version\\22:\\221.0\\22,\\22method\\22:\\22thing.event.property.post\\22}"

void aliyun_send_data(uint8_t tempX,uint8_t humi,uint8_t voice,uint8_t rader,uint8_t mqtt,uint8_t bee_state,uint8_t fire,uint8_t smoke,uint8_t tempZ)
{
	char send_str[1024] = {0};
	//{"fire":99,"bee_state":99,"mqtt":99,"rader":99,"MQ2":99,"voice":99,"humi":99,"temp":99}
	sprintf(send_str,"AT+MPUB=\"/sys/k0lrjXSIdGe/SmartLab-mcu/thing/event/property/post\",0,0,\"{\\22id\\22:1726732452464,\\22params\\22:{\\22fire\\22:%d,\\22bee_state\\22:%d,\\22mqtt\\22:%d,\\22rader\\22:%d,\\22MQ2\\22:%d,\\22voice\\22:%d,\\22humi\\22:%d,\\22temp\\22:%d,\\22phone\\22:%d},\\22version\\22:\\221.0\\22,\\22method\\22:\\22thing.event.property.post\\22}\""
,fire,bee_state,mqtt,rader,smoke,voice,humi,tempZ,tempX);
	NBIOT_SendCmd(send_str, "OK", 200);
}

void aliyun_send_check_in(uint8_t check_id)
{
	char send_str[1024] = {0};
	sprintf(send_str,"AT+MPUB=\"/sys/k0lrjXSIdGe/SmartLab-mcu/thing/event/property/post\",0,0,\"{\\22id\\22:1726732452464,\\22params\\22:{\\22check_in\\22:%d},\\22version\\22:\\221.0\\22,\\22method\\22:\\22thing.event.property.post\\22}\""
,check_id);
	NBIOT_SendCmd(send_str, "OK", 200);
}

void aliyun_send_check_off(uint8_t check_id)
{
	char send_str[1024] = {0};
	sprintf(send_str,"AT+MPUB=\"/sys/k0lrjXSIdGe/SmartLab-mcu/thing/event/property/post\",0,0,\"{\\22id\\22:1726732452464,\\22params\\22:{\\22check_off\\22:%d},\\22version\\22:\\221.0\\22,\\22method\\22:\\22thing.event.property.post\\22}\""
,check_id);
	NBIOT_SendCmd(send_str, "OK", 200);
}


uint8_t extractCoordinates(const char *input, double *latitude, double *longitude) 
{
    // �����ض���ģʽ
    const char *pattern = "+CIPGSMLOC: ";
    const char *pattern_start = strstr(input, pattern);

    if (pattern_start != NULL) 
    {
        // ���ҵ���ģʽ��ʼ��ȡ������
        pattern_start += strlen(pattern);

        // ʹ��strtok�ָ��ַ���
        char *end_ptr;
        char temp[100];
        strncpy(temp, pattern_start, sizeof(temp) - 1);
        temp[sizeof(temp) - 1] = '\0'; // ȷ���ַ����Կ��ַ���β

        // ������һ������
        strtok(temp, ","); // ���� "0"

        // ��ȡγ��
        char *lat_str = strtok(NULL, ",");
        if (lat_str != NULL) {
            *latitude = strtod(lat_str, &end_ptr);
        }

        // ��ȡ����
        char *lon_str = strtok(NULL, ",");
        if (lon_str != NULL) {
            *longitude = strtod(lon_str, &end_ptr);
        }

        return 1;
    } 
    else 
    {
        // �Ҳ���ģʽʱ�Ĵ���
        *latitude = 0.0;
        *longitude = 0.0;
        return 0;
    }
}


uint8_t extractLocation(const char *input, double *latitude, double *longitude) 
{
    // ��ʼ�����ֵ
    *latitude = 0.0;
    *longitude = 0.0;
    int i;

    // ��������ַ����Ƿ���"+CGNSINF: "��ͷ
    const char *prefix = "+CGNSINF: ";
    if (strncmp(input, prefix, strlen(prefix)) != 0) {
        return 0;  // �����ַ�����ƥ�䣬��������
    }

    // �ҵ���һ������
    const char *token = input + strlen(prefix);
    token = strchr(token, ',');
    if (token == NULL) {
        return 0;  // ��������
    }

    // �ƶ�����λ״̬�ַ�
    token++;

    // �ж϶�λ״̬
    if (*token == '0') {
        return 0;  // ��λʧ�ܣ�latitude �� longitude �Ѿ�����ʼ��Ϊ0.0
    } else if (*token != '1') {
        return 0;  // ��Ԥ�ڵĶ�λ״̬
    }

    // ��λ�ɹ���������ȡ��γ��
    for (i = 0; i < 2; i++) {
        token = strchr(token, ',') + 1;
        if (token == NULL) {
            return 0;  // ��������
        }
    }

    // ��ȡγ��
    *latitude = strtod(token, NULL);

    // �ҵ���һ�����ź�ľ���
    token = strchr(token, ',') + 1;
    if (token == NULL) {
        return 0;  // ��������
    }

    // ��ȡ����
    *longitude = strtod(token, NULL);
	return 1;
}



uint8_t findValue(const char *input, const char *key, int *value) 
{
    // ����key����ʼλ��
    const char *key_start = strstr(input, key);

    if (key_start != NULL) {
        // ����key�����"value":
        const char *value_start = strstr(key_start, "\"value\":");

        if (value_start != NULL) {
            // �ƶ�ָ�뵽"value"��������ֲ���
            value_start += strlen("\"value\":");

            // ��ȡvalue
            *value = atoi(value_start);

            return 1; // �ҵ�key����ȡ��value
        }
    }

    return 0; // û���ҵ�key����value
}


