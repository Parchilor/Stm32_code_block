#ifndef __ANALOG_I2C_H_
#define __ANALOG_I2C_H_

#include "FsmPort.h"

void System_I2C_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
INT8U IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAK(void);
void IIC_Send_Byte(INT8U txd);
INT8U IIC_Read_Byte(unsigned char ack);

#endif
