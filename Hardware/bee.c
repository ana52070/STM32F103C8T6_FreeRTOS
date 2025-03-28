#include "stm32f10x.h"                  // Device header

/**
  * ��    ����LED��ʼ��
  * ��    ������
  * �� �� ֵ����
  */
void bee_Init(void)
{
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//����GPIOA��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//��PA1��PA2���ų�ʼ��Ϊ�������
	
	/*����GPIO��ʼ�����Ĭ�ϵ�ƽ*/
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);				//����PA1��PA2����Ϊ�ߵ�ƽ
}

/**
  * ��    ����LED1����
  * ��    ������
  * �� �� ֵ����
  */
void bee_OFF(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);		//����PA1����Ϊ�͵�ƽ
}

/**
  * ��    ����LED1�ر�
  * ��    ������
  * �� �� ֵ����
  */
void bee_ON(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_15);		//����PA1����Ϊ�ߵ�ƽ
}


