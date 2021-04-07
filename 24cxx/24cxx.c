#include <string.h>
#include "24cxx.h"
//128√e980

#define EEPROM_Dly	HAL_Delay

sEepromInfo_t heeprom = {
	EEROM_STATE_RESET,
	0,
	0,
	0
};

void EEPROM_Init(void)
{
	System_I2C_Init();
	switch(EEP_DEFAULT_TYPE)
	{
		case AT24C02:
			heeprom.State = EEROM_STATE_RDY;
			heeprom.DevAddr = 0xA0;
			heeprom.BytesPerPage = 8;
			heeprom.PageNbr = 256;
			heeprom.Size = 256;
			break;
		case FM24CL04:
			heeprom.State = EEROM_STATE_RDY;
			heeprom.DevAddr = 0xA8;
			heeprom.BytesPerPage = 8;
			heeprom.PageNbr = 256;
			heeprom.Size = 512;
			break;
		default:
			break;
	}
}

eEepromError_t EEPROM_ReadOneByte(INT16U devaddress, INT8U *pData)
{
	INT8U *temp = 0;
	if(EEROM_STATE_RESET == heeprom.State)
	{
		return EEROM_ERR_UNINITIALIZED;
	}
	if((devaddress >= heeprom.Size))
	{
		return EEROM_ERR_OOM;
	}
	IIC_Start();
	if(EEP_DEFAULT_TYPE > AT24C16)
	{
		IIC_Send_Byte(EEROM_WRITE(heeprom.DevAddr));
		IIC_Wait_Ack();
		IIC_Send_Byte(devaddress >> 8);
		IIC_Wait_Ack();
	}else
	{
		IIC_Send_Byte(EEROM_WRITE(heeprom.DevAddr) + ((devaddress / heeprom.PageNbr) << 1));
	}

	IIC_Wait_Ack();
	IIC_Send_Byte(devaddress % heeprom.PageNbr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(EEROM_READ(heeprom.DevAddr));
	IIC_Wait_Ack();
	temp = pData;
	*temp = IIC_Read_Byte(0);
	IIC_Stop();
	return EEROM_OK;
}

eEepromError_t EEPROM_WriteOneByte(INT16U devaddress, INT8U pData)
{
	if(EEROM_STATE_RESET == heeprom.State)
	{
		return EEROM_ERR_UNINITIALIZED;
	}
	if(devaddress >= heeprom.Size)
	{
		return EEROM_ERR_OOM;
	}
	IIC_Start();
	if(EEP_DEFAULT_TYPE > AT24C16)
	{
		IIC_Send_Byte(EEROM_WRITE(heeprom.DevAddr));
		IIC_Wait_Ack();
		IIC_Send_Byte(devaddress >>8);
	}else
	{
		IIC_Send_Byte(EEROM_WRITE(heeprom.DevAddr) + ((devaddress  / heeprom.PageNbr) << 1));
	}
	IIC_Wait_Ack();
	IIC_Send_Byte(devaddress % heeprom.PageNbr);
	IIC_Wait_Ack();
	IIC_Send_Byte(pData);
	IIC_Wait_Ack();
	IIC_Stop();
	EEPROM_Dly(5);
	return EEROM_OK;
}

eEepromError_t EEPROM_Write(INT16U devaddress, INT8U *pData, INT16U size)
{
	INT8U tmp;
	INT16U Count = size;
	if(EEROM_STATE_RESET == heeprom.State)
	{
		return EEROM_ERR_UNINITIALIZED;
	}
	if((devaddress >= heeprom.Size) ||
		((heeprom.Size - devaddress) < size))
	{
		return EEROM_ERR_OOM;
	}
	IIC_Start();
	if(EEP_DEFAULT_TYPE > AT24C16)
	{
		IIC_Send_Byte(EEROM_WRITE(heeprom.DevAddr));
		IIC_Wait_Ack();
		IIC_Send_Byte(devaddress >>8);
	}else
	{
		IIC_Send_Byte(EEROM_WRITE(heeprom.DevAddr) + ((devaddress  / heeprom.PageNbr) << 1));
	}
	IIC_Wait_Ack();
	IIC_Send_Byte(devaddress % heeprom.PageNbr);
	IIC_Wait_Ack();

	while(Count > 0)
	{
		--Count;
		tmp = *pData;
		IIC_Send_Byte(tmp);
		IIC_Wait_Ack();
		pData += 1;
		devaddress += 1;
		if((devaddress % heeprom.PageNbr == 0) && (Count > 0))
		{
			IIC_Stop();
			EEPROM_Dly(5);
			IIC_Start();
			if(EEP_DEFAULT_TYPE > AT24C16)
			{
				IIC_Send_Byte(EEROM_WRITE(heeprom.DevAddr));
				IIC_Wait_Ack();
				IIC_Send_Byte(devaddress >>8);
			}else
			{
				IIC_Send_Byte(EEROM_WRITE(heeprom.DevAddr) + ((devaddress  / heeprom.PageNbr) << 1));
			}
			IIC_Wait_Ack();
			IIC_Send_Byte(devaddress % heeprom.PageNbr);
			IIC_Wait_Ack();
		}
	}

	IIC_Stop();
	EEPROM_Dly(5);
	return EEROM_OK;
}

eEepromError_t EEPROM_Read(INT16U devaddress, INT8U *pData, INT16U size)
{
	INT8U *temp = 0;
	INT16U Count = size;
	if(EEROM_STATE_RESET == heeprom.State)
	{
		return EEROM_ERR_UNINITIALIZED;
	}
	if(devaddress >= heeprom.Size)
	{
		return EEROM_ERR_OOM;
	}

	IIC_Start();
	if(EEP_DEFAULT_TYPE > AT24C16)
	{
		IIC_Send_Byte(EEROM_WRITE(heeprom.DevAddr));
		IIC_Wait_Ack();
		IIC_Send_Byte(devaddress >> 8);
		IIC_Wait_Ack();
	}else
	{
		IIC_Send_Byte(EEROM_WRITE(heeprom.DevAddr) + ((devaddress / heeprom.PageNbr) << 1));
	}

	IIC_Wait_Ack();
	IIC_Send_Byte(devaddress % heeprom.PageNbr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(EEROM_READ(heeprom.DevAddr) + ((devaddress / heeprom.PageNbr) << 1));
	IIC_Wait_Ack();
	while(Count > 1)
	{
		--Count;
		temp = pData;
		*temp = IIC_Read_Byte(1);
		pData += 1;
	}

	--Count;
	temp = pData;
	*temp = IIC_Read_Byte(0);
	IIC_Stop();
	return EEROM_OK;
}

INT8U EEPROM_Check(void)
{
	INT8U temp, verify = 0x22;
	EEPROM_ReadOneByte(EEP_DEFAULT_TYPE, &temp);
	if(temp == verify)
	{
		return 0;
	}
	else
	{
		EEPROM_WriteOneByte(EEP_DEFAULT_TYPE, verify);
		EEPROM_ReadOneByte(EEP_DEFAULT_TYPE, &temp);
		if(temp == verify)
		{
			return 0;
		}
	}
	return 1;
}
