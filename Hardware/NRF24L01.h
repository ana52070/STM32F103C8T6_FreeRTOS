#ifndef _NRF24L01_H
#define _NRF24L01_H

void W_Reg(uint8_t Reg,uint8_t Value);
uint8_t R_Reg(uint8_t Reg);
void W_Buf(uint8_t Reg , uint8_t* Buf, uint8_t Len);
void R_Buf(uint8_t Reg , uint8_t* Buf, uint8_t Len);
void NRF24L01_Init(void);
void Receive(uint8_t* Buf);
uint8_t Send(uint8_t* Buf);
u8 NRF24L01_RxPacket(u8 *Buf);
void RE_CONFIG(char ADDR);

#endif









