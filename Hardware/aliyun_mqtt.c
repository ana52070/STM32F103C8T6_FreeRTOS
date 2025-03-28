#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Serial.h"
void aliyun_mqtt_Init(void)
{
	//模块初始化
	NBIOT_Init();
	
	//1)AT指令
	while(NBIOT_SendCmd("ATE0", "OK", 200))          //关闭回显
		Delay_ms(500);
	
	Delay_ms(300);
	
	
	
	//MQTT初始化
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


//发消息AT指令：
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
    // 查找特定的模式
    const char *pattern = "+CIPGSMLOC: ";
    const char *pattern_start = strstr(input, pattern);

    if (pattern_start != NULL) 
    {
        // 从找到的模式开始截取并解析
        pattern_start += strlen(pattern);

        // 使用strtok分割字符串
        char *end_ptr;
        char temp[100];
        strncpy(temp, pattern_start, sizeof(temp) - 1);
        temp[sizeof(temp) - 1] = '\0'; // 确保字符串以空字符结尾

        // 跳过第一个部分
        strtok(temp, ","); // 跳过 "0"

        // 获取纬度
        char *lat_str = strtok(NULL, ",");
        if (lat_str != NULL) {
            *latitude = strtod(lat_str, &end_ptr);
        }

        // 获取经度
        char *lon_str = strtok(NULL, ",");
        if (lon_str != NULL) {
            *longitude = strtod(lon_str, &end_ptr);
        }

        return 1;
    } 
    else 
    {
        // 找不到模式时的处理
        *latitude = 0.0;
        *longitude = 0.0;
        return 0;
    }
}


uint8_t extractLocation(const char *input, double *latitude, double *longitude) 
{
    // 初始化输出值
    *latitude = 0.0;
    *longitude = 0.0;
    int i;

    // 检查输入字符串是否以"+CGNSINF: "开头
    const char *prefix = "+CGNSINF: ";
    if (strncmp(input, prefix, strlen(prefix)) != 0) {
        return 0;  // 输入字符串不匹配，不做处理
    }

    // 找到第一个逗号
    const char *token = input + strlen(prefix);
    token = strchr(token, ',');
    if (token == NULL) {
        return 0;  // 解析错误
    }

    // 移动到定位状态字符
    token++;

    // 判断定位状态
    if (*token == '0') {
        return 0;  // 定位失败，latitude 和 longitude 已经被初始化为0.0
    } else if (*token != '1') {
        return 0;  // 非预期的定位状态
    }

    // 定位成功，继续提取经纬度
    for (i = 0; i < 2; i++) {
        token = strchr(token, ',') + 1;
        if (token == NULL) {
            return 0;  // 解析错误
        }
    }

    // 提取纬度
    *latitude = strtod(token, NULL);

    // 找到下一个逗号后的经度
    token = strchr(token, ',') + 1;
    if (token == NULL) {
        return 0;  // 解析错误
    }

    // 提取经度
    *longitude = strtod(token, NULL);
	return 1;
}



uint8_t findValue(const char *input, const char *key, int *value) 
{
    // 查找key的起始位置
    const char *key_start = strstr(input, key);

    if (key_start != NULL) {
        // 查找key后面的"value":
        const char *value_start = strstr(key_start, "\"value\":");

        if (value_start != NULL) {
            // 移动指针到"value"后面的数字部分
            value_start += strlen("\"value\":");

            // 提取value
            *value = atoi(value_start);

            return 1; // 找到key并提取了value
        }
    }

    return 0; // 没有找到key或者value
}


