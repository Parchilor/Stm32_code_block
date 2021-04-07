#ifndef __CRC_H_
#define __CRC_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "FsmPort.h"

// #define POLY 0x8005

/******************************************************************************
 * Name:CRC-4/ITU x4+x+1
 * Poly:0x03
 * Init:0x00
 * Refin: True
 * Refout:True
 * Xorout:0x00
 * Note:
 *****************************************************************************/
INT8U crc4_itu(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-5/EPC x5+x3+1
 * Poly:0x09
 * Init:0x09
 * Refin: False
 * Refout:False
 * Xorout:0x00
 * Note:
 *****************************************************************************/
INT8U crc5_epc(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-5/ITU x5+x4+x2+1
 * Poly:0x15
 * Init:0x00
 * Refin: True
 * Refout:True
 * Xorout:0x00
 * Note:
 *****************************************************************************/
INT8U crc5_itu(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-5/USB x5+x2+1
 * Poly:0x05
 * Init:0x1F
 * Refin: True
 * Refout:True
 * Xorout:0x1F
 * Note:
 *****************************************************************************/
INT8U crc5_usb(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-6/ITU x6+x+1
 * Poly:0x03
 * Init:0x00
 * Refin: True
 * Refout:True
 * Xorout:0x00
 * Note:
 *****************************************************************************/
INT8U crc6_itu(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-7/MMC x7+x3+1
 * Poly:0x09
 * Init:0x00
 * Refin: False
 * Refout:False
 * Xorout:0x00
 * Use: MultiMediaCard,SD,ect.
 *****************************************************************************/
INT8U crc7_mmc(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-8 x8+x2+x+1
 * Poly:0x07
 * Init:0x00
 * Refin: False
 * Refout:False
 * Xorout:0x00
 * Note:
 *****************************************************************************/
INT8U crc8(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-8/ITU x8+x2+x+1
 * Poly:0x07
 * Init:0x00
 * Refin: False
 * Refout:False
 * Xorout:0x55
 * Alias: CRC-8/ATM
 *****************************************************************************/
INT8U crc8_itu(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-8/ROHCx8+x2+x+1
 * Poly:0x07
 * Init:0xFF
 * Refin: True
 * Refout:True
 * Xorout:0x00
 * Note:
 *****************************************************************************/
INT8U crc8_rohc(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-8/MAXIM x8+x5+x4+1
 * Poly:0x31
 * Init:0x00
 * Refin: True
 * Refout:True
 * Xorout:0x00
 * Alias: DOW-CRC,CRC-8/IBUTTON
 * Use: Maxim(Dallas)'s some devices,e.g. DS18B20
 *****************************************************************************/
INT8U crc8_maxim(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-16/IBMx16+x15+x2+1
 * Poly:0x8005
 * Init:0x0000
 * Refin: True
 * Refout:True
 * Xorout:0x0000
 * Alias: CRC-16,CRC-16/ARC,CRC-16/LHA
 *****************************************************************************/
uint16_t crc16_ibm(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-16/MAXIMx16+x15+x2+1
 * Poly:0x8005
 * Init:0x0000
 * Refin: True
 * Refout:True
 * Xorout:0xFFFF
 * Note:
 *****************************************************************************/
uint16_t crc16_maxim(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-16/USBx16+x15+x2+1
 * Poly:0x8005
 * Init:0xFFFF
 * Refin: True
 * Refout:True
 * Xorout:0xFFFF
 * Note:
 *****************************************************************************/
uint16_t crc16_usb(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-16/MODBUS x16+x15+x2+1
 * Poly:0x8005
 * Init:0xFFFF
 * Refin: True
 * Refout:True
 * Xorout:0x0000
 * Note:
 *****************************************************************************/
uint16_t crc16_modbus(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-16/CCITTx16+x12+x5+1
 * Poly:0x1021
 * Init:0x0000
 * Refin: True
 * Refout:True
 * Xorout:0x0000
 * Alias: CRC-CCITT,CRC-16/CCITT-TRUE,CRC-16/KERMIT
 *****************************************************************************/
uint16_t crc16_ccitt(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-16/CCITT-FALSE x16+x12+x5+1
 * Poly:0x1021
 * Init:0xFFFF
 * Refin: False
 * Refout:False
 * Xorout:0x0000
 * Note:
 *****************************************************************************/
uint16_t crc16_ccitt_false(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-16/X25x16+x12+x5+1
 * Poly:0x1021
 * Init:0xFFFF
 * Refin: True
 * Refout:True
 * Xorout:0XFFFF
 * Note:
 *****************************************************************************/
uint16_t crc16_x25(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-16/XMODEM x16+x12+x5+1
 * Poly:0x1021
 * Init:0x0000
 * Refin: False
 * Refout:False
 * Xorout:0x0000
 * Alias: CRC-16/ZMODEM,CRC-16/ACORN
 *****************************************************************************/
uint16_t crc16_xmodem(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-16/DNPx16+x13+x12+x11+x10+x8+x6+x5+x2+1
 * Poly:0x3D65
 * Init:0x0000
 * Refin: True
 * Refout:True
 * Xorout:0xFFFF
 * Use: M-Bus,ect.
 *****************************************************************************/
uint16_t crc16_dnp(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-32x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
 * Poly:0x4C11DB7
 * Init:0xFFFFFFF
 * Refin: True
 * Refout:True
 * Xorout:0xFFFFFFF
 * Alias: CRC_32/ADCCP
 * Use: WinRAR,ect.
 *****************************************************************************/
INT32U crc32(INT8U *data, INT16U length);

/******************************************************************************
 * Name:CRC-32/MPEG-2x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
 * Poly:0x4C11DB7
 * Init:0xFFFFFFF
 * Refin: False
 * Refout:False
 * Xorout:0x0000000
 * Note:
 *****************************************************************************/
INT32U crc32_mpeg_2(INT8U *data, INT16U length);


#ifdef __cplusplus
 }
#endif
#endif