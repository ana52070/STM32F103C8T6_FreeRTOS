#include "stm32f10x.h"                  // Device header
#include "serial.h"
#include <stdio.h>
#include "Delay.h"
#include <string.h>


#define REV_OK		0	//������ɱ�־
#define REV_WAIT	1	//����δ��ɱ�־


uint16_t  U1_RX_Cnt;		//����״̬���
uint8_t  U1_RX_Cmd;
uint8_t  U1_RX_Error;



//ģ���ʼ������
void NBIOT_Init(void)
{
	Usart1_Init();		//���ڳ�ʼ��
	
}



char test[255];

char AtStrBuf[255];   								//��ӡ������
#define BUFLEN 255
unsigned short U1_RX_CntPre = 0;

//void NBIOT_Init(void)
//{
//	//1)ATָ��������ģ�����	
//	while(AIR724_SendCmd("AT", "OK", 200))          //����ָ�������OK����ģ�������ͨ��\r\n
//		Delay_ms(100);
//	while(AIR724_SendCmd("AT+CPIN?", "READY", 200))  //���SIM���Ƿ���λ
//		Delay_ms(500);
//	while(AIR724_SendCmd("AT+CSQ", "+CSQ", 200))     //��ѯ�ź�����
//		Delay_ms(500);
//	while(AIR724_SendCmd("AT+CREG?", "0,1", 200))    //��ѯ����״̬
//		Delay_ms(500);
//}

_Bool NBIOT_WaitRecive(void)
{
	if(U1_RX_Cnt == 0) 						//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;
		
	if(U1_RX_Cnt == U1_RX_CntPre)			//�����һ�ε�ֵ�������ͬ����˵���������
	{
		U1_RX_Cnt = 0;							//��0���ռ���
			
		return REV_OK;							//���ؽ�����ɱ�־
	}
		
	U1_RX_CntPre = U1_RX_Cnt;				//��Ϊ��ͬ
	
	return REV_WAIT;							//���ؽ���δ��ɱ�־
}

void NBIOT_Clear(void)
{
	memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF));
	U1_RX_Cnt = 0;

}



void extractErrorString(const char *input, char *output) 
{
    // ���Ҵ��������ʼλ��
    const char *error_prefix = "ERROR:";
    const char *error_start = strstr(input, error_prefix);

    if (error_start != NULL) {
        // ���ҵ����з����ַ�������λ��
        const char *error_end = strchr(error_start, '\n');
        if (error_end == NULL) {
            error_end = error_start + strlen(error_start);
        }

        // ��������ַ����ĳ���
        size_t length = error_end - error_start;

        // �������ַ������Ƶ����������
        strncpy(output, error_start, length);
        output[length] = '\0'; // ��ӿ���ֹ��
    } else {
        // ���û���ҵ������룬���Ϊ���ַ���
        strcpy(output, "");
    }
}


_Bool NBIOT_SendCmd(char *cmd, char *res, u16 time)
{	
	USART1_SendATCommand((const char*)cmd);
	while(time--)
	{	
		if(NBIOT_WaitRecive() == REV_OK)							//����յ�����
		{	

			extractErrorString((const char*)USART_RX_BUF,test);


			if(strstr((const char *)USART_RX_BUF, res)!= NULL)		//����������ؼ���
			{
				NBIOT_Clear();									//��ջ���
				return 0;
			}
		}
		Delay_ms(10);
	}
	return 1;
}



/**
  * ��    ����USART1�жϺ���
  * ��    ������
  * �� �� ֵ����
  * ע������˺���Ϊ�жϺ�����������ã��жϴ������Զ�ִ��
  *           ������ΪԤ����ָ�����ƣ����Դ������ļ�����
  *           ��ȷ����������ȷ���������κβ��죬�����жϺ��������ܽ���
  */
void USART1_IRQHandler(void)
{
   int res;
   if (USART1->SR & (1 << 5)) // ���յ�����
	{
		res = USART1->DR;
		if(U1_RX_Cnt<=1024)
		{
			USART_RX_BUF[U1_RX_Cnt] = (unsigned char)res;
			U1_RX_Cnt ++;
		}
	}
}
	

