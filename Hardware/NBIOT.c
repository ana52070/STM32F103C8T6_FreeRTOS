#include "stm32f10x.h"                  // Device header
#include "serial.h"
#include <stdio.h>
#include "Delay.h"
#include <string.h>


#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志


uint16_t  U1_RX_Cnt;		//接收状态标记
uint8_t  U1_RX_Cmd;
uint8_t  U1_RX_Error;



//模块初始化代码
void NBIOT_Init(void)
{
	Usart1_Init();		//串口初始化
	
}



char test[255];

char AtStrBuf[255];   								//打印缓存器
#define BUFLEN 255
unsigned short U1_RX_CntPre = 0;

//void NBIOT_Init(void)
//{
//	//1)AT指令入网、模块测试	
//	while(AIR724_SendCmd("AT", "OK", 200))          //测试指令，若返回OK，则模块可正常通信\r\n
//		Delay_ms(100);
//	while(AIR724_SendCmd("AT+CPIN?", "READY", 200))  //检查SIM卡是否在位
//		Delay_ms(500);
//	while(AIR724_SendCmd("AT+CSQ", "+CSQ", 200))     //查询信号质量
//		Delay_ms(500);
//	while(AIR724_SendCmd("AT+CREG?", "0,1", 200))    //查询入网状态
//		Delay_ms(500);
//}

_Bool NBIOT_WaitRecive(void)
{
	if(U1_RX_Cnt == 0) 						//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;
		
	if(U1_RX_Cnt == U1_RX_CntPre)			//如果上一次的值和这次相同，则说明接收完毕
	{
		U1_RX_Cnt = 0;							//清0接收计数
			
		return REV_OK;							//返回接收完成标志
	}
		
	U1_RX_CntPre = U1_RX_Cnt;				//置为相同
	
	return REV_WAIT;							//返回接收未完成标志
}

void NBIOT_Clear(void)
{
	memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF));
	U1_RX_Cnt = 0;

}



void extractErrorString(const char *input, char *output) 
{
    // 查找错误码的起始位置
    const char *error_prefix = "ERROR:";
    const char *error_start = strstr(input, error_prefix);

    if (error_start != NULL) {
        // 查找到换行符或字符串结束位置
        const char *error_end = strchr(error_start, '\n');
        if (error_end == NULL) {
            error_end = error_start + strlen(error_start);
        }

        // 计算错误字符串的长度
        size_t length = error_end - error_start;

        // 将错误字符串复制到输出缓冲区
        strncpy(output, error_start, length);
        output[length] = '\0'; // 添加空终止符
    } else {
        // 如果没有找到错误码，输出为空字符串
        strcpy(output, "");
    }
}


_Bool NBIOT_SendCmd(char *cmd, char *res, u16 time)
{	
	USART1_SendATCommand((const char*)cmd);
	while(time--)
	{	
		if(NBIOT_WaitRecive() == REV_OK)							//如果收到数据
		{	

			extractErrorString((const char*)USART_RX_BUF,test);


			if(strstr((const char *)USART_RX_BUF, res)!= NULL)		//如果检索到关键词
			{
				NBIOT_Clear();									//清空缓存
				return 0;
			}
		}
		Delay_ms(10);
	}
	return 1;
}



/**
  * 函    数：USART1中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
void USART1_IRQHandler(void)
{
   int res;
   if (USART1->SR & (1 << 5)) // 接收到数据
	{
		res = USART1->DR;
		if(U1_RX_Cnt<=1024)
		{
			USART_RX_BUF[U1_RX_Cnt] = (unsigned char)res;
			U1_RX_Cnt ++;
		}
	}
}
	

