#ifndef __NBIOT_H
#define __NBIOT_H



void NBIOT_Init(void);
_Bool NBIOT_WaitRecive(void);
void NBIOT_Clear(void);
_Bool NBIOT_SendCmd(char *cmd, char *res, u16 time);



#endif
