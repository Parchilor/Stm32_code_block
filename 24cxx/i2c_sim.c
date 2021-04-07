#include "main.h"
#include "FsmPort.h"

#define SDA_IN()	MODIFY_REG(GPIOA->MODER, 0x03 << (9 << 1), 0x00 << (9 << 1))
#define SDA_OUT()	MODIFY_REG(GPIOA->MODER, 0x03 << (9 << 1), 0x01 << (9 << 1))

#define IIC_SCL(lv) do{HAL_GPIO_WritePin(FM_SCL_GPIO_Port, FM_SCL_Pin, (lv == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);}while(0)
#define IIC_SDA(lv) do{HAL_GPIO_WritePin(FM_SDA_GPIO_Port, FM_SDA_Pin, (lv == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);}while(0)
#define READ_SDA()  (HAL_GPIO_ReadPin(FM_SDA_GPIO_Port, FM_SDA_Pin) == GPIO_PIN_RESET ? 0 : 1)

#define DELAY_1US	(__NOP(), __NOP(), __NOP())// nOSTimeDly(1);
#define DELAY_2US	(DELAY_1US, DELAY_1US)// nOSTimeDly(2);
#define DELAY_4US	(DELAY_2US, DELAY_2US)// nOSTimeDly(4);

void System_I2C_Init(void)
{
	IIC_SCL(1);
	IIC_SDA(1);
}

void IIC_Start(void)
{
	SDA_OUT();
	IIC_SDA(1);
	IIC_SCL(1);
	DELAY_4US;
 	IIC_SDA(0);
	DELAY_4US;
	IIC_SCL(0);
}

void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL(0);
	IIC_SDA(0);
 	DELAY_4US;
	IIC_SCL(1);
	IIC_SDA(1);
	DELAY_4US;
}

INT8U IIC_Wait_Ack(void)
{
	INT8U ucErrTime = 0;
	SDA_IN();
	IIC_SDA(1);
	DELAY_1US;
	IIC_SCL(1);
	DELAY_1US;
	while(READ_SDA())
	{
		ucErrTime++;
		if(ucErrTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0);
	return 0;
}

void IIC_Ack(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(0);
	DELAY_2US;
	IIC_SCL(1);
	DELAY_2US;
	IIC_SCL(0);
}

void IIC_NAK(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(1);
	DELAY_2US;
	IIC_SCL(1);
	DELAY_2US;
	IIC_SCL(0);
}

void IIC_Send_Byte(INT8U txd)
{
    INT8U t;
	SDA_OUT();
    IIC_SCL(0);
    for(t = 0; t < 8; t++)
    {
		if(txd & 0x80) IIC_SDA(1);
		else IIC_SDA(0);
        txd <<= 1;
		DELAY_2US;
		IIC_SCL(1);
		DELAY_2US;
		IIC_SCL(0);
		DELAY_2US;
    }
}

INT8U IIC_Read_Byte(INT8U ack)
{
	INT8U i, receive = 0;
	SDA_IN();
    for(i = 0; i < 8; i++)
	{
        IIC_SCL(0);
        DELAY_2US;
		IIC_SCL(1);
		DELAY_1US;
        receive <<= 1;
		if(READ_SDA())
		{
			receive |= 0x01;
		}
		DELAY_1US;
    }
    if (!ack)
	{
		IIC_NAK();
	}
    else
	{
		IIC_Ack();
	}
    return receive;
}
