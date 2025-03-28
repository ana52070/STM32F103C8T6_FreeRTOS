#include "stm32f10x.h"                  // Device header
#include "NRF24L01_Ins.h"
#include "Delay.h"



const uint8_t RX_ADDRESS_Pub[] = {0xff,0xff,0xff,0xff,0xff};//����ͨ��
const uint8_t RX_ADDRESS_A[] = {0xfe,0xaa,0xaa,0xaa,0x01};//Aͨ��
const uint8_t RX_ADDRESS_B[] = {0xfe,0xbb,0xbb,0xbb,0x01};//Bͨ��
const uint8_t RX_ADDRESS_C[] = {0xfe,0xcc,0xcc,0xcc,0x01};//Cͨ��
const uint8_t RX_ADDRESS_F[] = {0xfe,0xff,0xff,0xdd,0x01};//Fͨ��

 
const uint8_t TX_ADDRESS_Pub[] = {0xff,0xff,0xff,0xff,0xff};//����ͨ��
const uint8_t TX_ADDRESS_A[] = {0xfe,0xaa,0xaa,0xaa,0x01};//Aͨ��
const uint8_t TX_ADDRESS_B[] = {0xfe,0xbb,0xbb,0xbb,0x01};//Bͨ��
const uint8_t TX_ADDRESS_C[] = {0xfe,0xcc,0xcc,0xcc,0x01};//Cͨ��
const uint8_t TX_ADDRESS_F[] = {0xfe,0xff,0xff,0xdd,0x01};//Fͨ��



void W_MOSI(uint8_t Value)
{
	GPIO_WriteBit(MOSI_Port,MOSI_Pin,(BitAction)Value);
}

void W_SCK(uint8_t Value)
{
	GPIO_WriteBit(SCK_Port,SCK_Pin,(BitAction)Value);
}

void W_CSN(uint8_t Value)
{
	GPIO_WriteBit(CSN_Port,CSN_Pin,(BitAction)Value);
}

void W_CE(uint8_t Value)
{
	GPIO_WriteBit(CE_Port,CE_Pin,(BitAction)Value);
}

uint8_t R_IRQ(void)
{
	return GPIO_ReadInputDataBit(IRQ_Port,IRQ_Pin);
}

uint8_t R_MISO(void)
{
	return GPIO_ReadInputDataBit(MISO_Port,MISO_Pin);
}

void NRF24L01_Pin_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//���ó��������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=MOSI_Pin;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(MOSI_Port,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=SCK_Pin;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(SCK_Port,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=CSN_Pin;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(CSN_Port,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=CE_Pin;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(CE_Port,&GPIO_InitStructure);
	
	
	//���ó���������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=IRQ_Pin;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(IRQ_Port,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=MISO_Pin;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(MISO_Port,&GPIO_InitStructure);
	
}

//SPI����һ���ֽ�
uint8_t SPI_SwapByte(uint8_t Byte)
{
	uint8_t i,ByteReceive=0x00;
	for(i=0;i<8;i++)
	{
		W_MOSI(Byte&(0x80>>i));
		W_SCK(1);
		if(R_MISO()==1)
		{
			ByteReceive=ByteReceive|(0x80>>i);
		}
		W_SCK(0);
	}
	return ByteReceive;
}

void W_Reg(uint8_t Reg,uint8_t Value)
{
	W_CSN(0);//��ʾѡ��NRF24L01
	SPI_SwapByte(Reg);//�����ĵ�һ���ֽھ���ָ��
	SPI_SwapByte(Value);//�����ĵڶ����ֽھ��ǽ���������
	W_CSN(1);//ֹͣѡ��NRF24L01
	
}

uint8_t R_Reg(uint8_t Reg)
{
	uint8_t Value;
	W_CSN(0);//��ʾѡ��NRF24L01
	SPI_SwapByte(Reg);//�����ĵ�һ���ֽھ���ָ��
	Value=SPI_SwapByte(NOP);//�����ĵڶ����ֽھ��ǽ���������
	W_CSN(1);//ֹͣѡ��NRF24L01
	return Value;
}

void W_Buf(uint8_t Reg , uint8_t* Buf, uint8_t Len)
{
	uint8_t i;
	W_CSN(0);//ѡ��NRF24L01
	SPI_SwapByte(Reg);
	for(i=0;i<Len;i++)
	{
		SPI_SwapByte(Buf[i]);
	}
	W_CSN(1);//ֹͣѡ��NRF24L01
}

void R_Buf(uint8_t Reg , uint8_t* Buf, uint8_t Len)
{
	uint8_t i;
	W_CSN(0);//ѡ��NRF24L01
	SPI_SwapByte(Reg);
	for(i=0;i<Len;i++)
	{
		Buf[i]=SPI_SwapByte(NOP);
	}
	W_CSN(1);//ֹͣѡ��NRF24L01
}


void NRF24L01_Init(void)
{
	NRF24L01_Pin_Init();
	
	W_CE(0);
	
	W_Buf(W_REGISTER+TX_ADDR, (uint8_t*)TX_ADDRESS_F, 5);//���÷��͵�ַ
	W_Buf(W_REGISTER+RX_ADDR_P0,(uint8_t*)RX_ADDRESS_F, 5);//���ý���ͨ��0

	
	W_Reg(W_REGISTER+CONFIG,0x0F);//���óɽ���ģʽ
	W_Reg(W_REGISTER+EN_AA,0x3F);//ͨ��0�����Զ�Ӧ��
	W_Reg(W_REGISTER+RF_CH,0x00);//����ͨ��Ƶ��2.4G
	
	W_Reg(W_REGISTER+RX_PW_P0,32);//���ý���ͨ��0���յ����ݿ��32�ֽ�
	
	W_Reg(W_REGISTER+EN_RXADDR,0x3F);//����ͨ��0ʹ��
	W_Reg(W_REGISTER+SETUP_RETR,0x1A);//����580us�ط�ʱ����,�ط�10��
	W_Reg(FLUSH_RX,NOP);
	
	W_CE(1);
	

	
}

u8 NRF24L01_RxPacket(u8 *Buf)
{
	uint8_t Status;		    							   
	
	Status = R_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	W_Reg(W_REGISTER+STATUS,Status); //���TX_DS��MAX_RT�жϱ�־
	if(Status&RX_OK)//���յ�����
	{
		switch((Status >> 1) & 0x07)
		{
			case 0x00:R_Buf(R_RX_PAYLOAD,Buf,32);//��ȡ����
				break;
			case 0x01: R_Buf(R_RX_PAYLOAD,Buf,32);
				break;
			case 0x02: R_Buf(R_RX_PAYLOAD,Buf,32);
				break;
			case 0x03:R_Buf(R_RX_PAYLOAD,Buf,32);//��ȡ����
				break;
			default: ;
				break;
		}
		
		W_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}



uint8_t Send(uint8_t* Buf)
{
	uint8_t Status;
	W_Buf(W_TX_PAYLOAD, Buf, 32);//�ڷ������ݻ���������Ҫ���͵�����
	
	W_CE(0);
	W_Reg(W_REGISTER+CONFIG,0x0E);
	W_CE(1);

	while(R_IRQ()==1);//�ȴ��ж�
	Status= R_Reg(R_REGISTER+STATUS);
	
	if(Status & MAX_TX)//������ʹﵽ������
	{
		W_Reg(FLUSH_TX,NOP);//����������ݻ�����
		W_Reg(W_REGISTER+STATUS,Status);//�ж�λд1����ж�
		return MAX_TX;
	}
	if(Status & TX_OK)//������ͳɹ�,���յ�Ӧ���ź�
	{
		W_Reg(W_REGISTER+STATUS,Status);//����ж�
		return TX_OK;
	}
	return TX_OK;
}


void RE_CONFIG(char ADDR)
{
	
	W_CE(0);
	if(ADDR == 'A')
	{
		W_Buf(W_REGISTER+TX_ADDR, (uint8_t*)TX_ADDRESS_A, 5);//���÷��͵�ַ
		W_Buf(W_REGISTER+RX_ADDR_P0,(uint8_t*)RX_ADDRESS_A, 5);//���ý���ͨ��0
	}
	else if(ADDR == 'B')
	{
		W_Buf(W_REGISTER+TX_ADDR, (uint8_t*)TX_ADDRESS_B, 5);//���÷��͵�ַ
		W_Buf(W_REGISTER+RX_ADDR_P0,(uint8_t*)RX_ADDRESS_B, 5);//���ý���ͨ��0
	}
	else if(ADDR == 'C')
	{
		W_Buf(W_REGISTER+TX_ADDR, (uint8_t*)TX_ADDRESS_C, 5);//���÷��͵�ַ
		W_Buf(W_REGISTER+RX_ADDR_P0,(uint8_t*)RX_ADDRESS_C, 5);//���ý���ͨ��0
	}
	else if(ADDR == 'F')
	{
		W_Buf(W_REGISTER+TX_ADDR, (uint8_t*)TX_ADDRESS_F, 5);//���÷��͵�ַ
		W_Buf(W_REGISTER+RX_ADDR_P0,(uint8_t*)RX_ADDRESS_F, 5);//���ý���ͨ��0
	}
	
	W_Reg(W_REGISTER+CONFIG,0x0F);//���óɽ���ģʽ
	W_Reg(W_REGISTER+EN_AA,0x3F);//ͨ��0�����Զ�Ӧ��
	W_Reg(W_REGISTER+RF_CH,0x00);//����ͨ��Ƶ��2.4G
	
	W_Reg(W_REGISTER+RX_PW_P0,32);//���ý���ͨ��0���յ����ݿ��32�ֽ�
	
	W_Reg(W_REGISTER+EN_RXADDR,0x3F);//����ͨ��0ʹ��
	W_Reg(W_REGISTER+SETUP_RETR,0x1A);//����580us�ط�ʱ����,�ط�10��
	W_Reg(FLUSH_RX,NOP);
	
	W_CE(1);
	
}








