#ifndef __EEPROM_H
#define __EEPROM_H
#include "i2c.h"

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767

#define FM24CL04		0xA8

#define EEP_DEFAULT_TYPE AT24C02

#define EEROM_WRITE(deviceid) (deviceid | 0x00)
#define EEROM_READ(deviceid) (deviceid | 0x01)

typedef enum{
    EEROM_OK = 0x00,
    EEROM_ERR_OOM = 0x01,
    EEROM_ERR_UNINITIALIZED = 0x02, 
} eEepromError_t;

typedef enum{
    EEROM_STATE_RESET,
    EEROM_STATE_RDY,
} EepromState_t;

// at24c02
typedef struct{
    INT8U State;
    INT8U DevAddr;
    INT8U BytesPerPage; // 8
    INT16U PageNbr; // 256
    INT16U Size;
} sEepromInfo_t;

void EEPROM_Init(void);
eEepromError_t EEPROM_ReadOneByte(INT16U devaddress, INT8U *pData);
eEepromError_t EEPROM_WriteOneByte(INT16U devaddress, INT8U pData);
eEepromError_t EEPROM_Write(INT16U devaddress, INT8U *pData, INT16U size);
eEepromError_t EEPROM_Read(INT16U devaddress, INT8U *pData, INT16U size);
INT8U EEPROM_Check(void);
#endif
