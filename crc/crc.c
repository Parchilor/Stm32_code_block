#include <stdio.h>
#include "crc.h"

/******************************************************************************
 * Name:CRC-4/ITU x4+x+1
 * Poly:0x03
 * Init:0x00
 * Refin: True
 * Refout:True
 * Xorout:0x00
 * Note:
 *****************************************************************************/
INT8U crc4_itu(INT8U *data, INT16U length)
{
	INT8U i;
	INT8U crc = 0;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for (i = 0; i < 8; ++i)
		{
			if (crc & 1)
#ifndef POLY
				crc = (crc >> 1) ^ 0x0C;// 0x0C = (reverse 0x03)>>(8-4)
#else
				crc = (crc >> 1) ^ POLY;
#endif
			else
				crc = (crc >> 1);
		}
	}
	return crc;
}

/******************************************************************************
 * Name:CRC-5/EPC x5+x3+1
 * Poly:0x09
 * Init:0x09
 * Refin: False
 * Refout:False
 * Xorout:0x00
 * Note:
 *****************************************************************************/
INT8U crc5_epc(INT8U *data, INT16U length)
{
	INT8U i;
	INT8U crc = 0x48;// Initial value: 0x48 = 0x09<<(8-5)
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for ( i = 0; i < 8; i++ )
		{
			if ( crc & 0x80 )
#ifndef POLY
				crc = (crc << 1) ^ 0x48;// 0x48 = 0x09<<(8-5)
#else
				crc = (crc << 1) ^ POLY;
#endif
			else
				crc <<= 1;
		}
	}
	return crc >> 3;
}

/******************************************************************************
 * Name:CRC-5/ITU x5+x4+x2+1
 * Poly:0x15
 * Init:0x00
 * Refin: True
 * Refout:True
 * Xorout:0x00
 * Note:
 *****************************************************************************/
INT8U crc5_itu(INT8U *data, INT16U length)
{
	INT8U i;
	INT8U crc = 0;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for (i = 0; i < 8; ++i)
		{
			if (crc & 1)
#ifndef POLY
				crc = (crc >> 1) ^ 0x15;// 0x15 = (reverse 0x15)>>(8-5)
#else
				crc = (crc >> 1) ^ POLY;
#endif
			else
				crc = (crc >> 1);
		}
	}
	return crc;
}

/******************************************************************************
 * Name:CRC-5/USB x5+x2+1
 * Poly:0x05
 * Init:0x1F
 * Refin: True
 * Refout:True
 * Xorout:0x1F
 * Note:
 *****************************************************************************/
INT8U crc5_usb(INT8U *data, INT16U length)
{
	INT8U i;
	INT8U crc = 0x1F;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for (i = 0; i < 8; ++i)
		{
			if (crc & 1)
#ifndef POLY
				crc = (crc >> 1) ^ 0x14;// 0x14 = (reverse 0x05)>>(8-5)
#else
				crc = (crc >> 1) ^ POLY;//0x14;// 0x14 = (reverse 0x05)>>(8-5)
#endif
			else
				crc = (crc >> 1);
		}
	}
	return crc ^ 0x1F;
}

/******************************************************************************
 * Name:CRC-6/ITU x6+x+1
 * Poly:0x03
 * Init:0x00
 * Refin: True
 * Refout:True
 * Xorout:0x00
 * Note:
 *****************************************************************************/
INT8U crc6_itu(INT8U *data, INT16U length)
{
	INT8U i;
	INT8U crc = 0;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for	(i = 0; i < 8; ++i)
		{
			if (crc & 1)
#ifndef POLY
				crc = (crc >> 1) ^ 0x30;// 0x30 = (reverse 0x03)>>(8-6)
#else
				crc = (crc >> 1) ^ POLY;//0x30;// 0x30 = (reverse 0x03)>>(8-6)
#endif
			else
				crc = (crc >> 1);
		}
	}
	return crc;
}

/******************************************************************************
 * Name:CRC-7/MMC x7+x3+1
 * Poly:0x09
 * Init:0x00
 * Refin: False
 * Refout:False
 * Xorout:0x00
 * Use: MultiMediaCard,SD,ect.
 *****************************************************************************/
INT8U crc7_mmc(INT8U *data, INT16U length)
{
	INT8U i;
	INT8U crc = 0;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for ( i = 0; i < 8; i++ )
		{
			if ( crc & 0x80 )
#ifndef POLY
				crc = (crc << 1) ^ 0x12;// 0x12 = 0x09<<(8-7)
#else
				crc = (crc << 1) ^ POLY;//0x12;// 0x12 = 0x09<<(8-7)
#endif
			else
				crc <<= 1;
		}
	}
	return crc >> 1;
}

/******************************************************************************
 * Name:CRC-8 x8+x2+x+1
 * Poly:0x07
 * Init:0x00
 * Refin: False
 * Refout:False
 * Xorout:0x00
 * Note:
 *****************************************************************************/
INT8U crc8(INT8U *data, INT16U length)
{
	INT8U i;
	INT8U crc = 0;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for ( i = 0; i < 8; i++ )
		{
			if ( crc & 0x80 )
#ifndef POLY
				crc = (crc << 1) ^ 0x07;
#else
				crc = (crc << 1) ^ POLY;//0x07;
#endif
			else
				crc <<= 1;
		}
	}
	return crc;
}

/******************************************************************************
 * Name:CRC-8/ITU x8+x2+x+1
 * Poly:0x07
 * Init:0x00
 * Refin: False
 * Refout:False
 * Xorout:0x55
 * Alias: CRC-8/ATM
 *****************************************************************************/
INT8U crc8_itu(INT8U *data, INT16U length)
{
	INT8U i;
	INT8U crc = 0;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for ( i = 0; i < 8; i++ )
		{
			if ( crc & 0x80 )
#ifndef POLY
				crc = (crc << 1) ^ 0x07;
#else
				crc = (crc << 1) ^ POLY;//0x07;
#endif
			else
				crc <<= 1;
		}
	}
	return crc ^ 0x55;
}

/******************************************************************************
 * Name:CRC-8/ROHCx8+x2+x+1
 * Poly:0x07
 * Init:0xFF
 * Refin: True
 * Refout:True
 * Xorout:0x00
 * Note:
 *****************************************************************************/
INT8U crc8_rohc(INT8U *data, INT16U length)
{
	INT8U i;
	INT8U crc = 0xFF;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for (i = 0; i < 8; ++i)
		{
			if (crc & 1)
#ifndef POLY
				crc = (crc >> 1) ^ 0xE0;// 0xE0 = reverse 0x07
#else
				crc = (crc >> 1) ^ POLY;//0xE0;// 0xE0 = reverse 0x07
#endif
			else
				crc = (crc >> 1);
		}
	}
	return crc;
}

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
INT8U crc8_maxim(INT8U *data, INT16U length)
{
	INT8U i;
	INT8U crc = 0;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for (i = 0; i < 8; i++)
		{
			if (crc & 1)
#ifndef POLY
				crc = (crc >> 1) ^ 0x8C;// 0x8C = reverse 0x31
#else
				crc = (crc >> 1) ^ POLY;//0x8C;// 0x8C = reverse 0x31
#endif
			else
				crc >>= 1;
		}
	}
	return crc;
}

/******************************************************************************
 * Name:CRC-16/IBMx16+x15+x2+1
 * Poly:0x8005
 * Init:0x0000
 * Refin: True
 * Refout:True
 * Xorout:0x0000
 * Alias: CRC-16,CRC-16/ARC,CRC-16/LHA
 *****************************************************************************/
uint16_t crc16_ibm(INT8U *data, INT16U length)
{
	INT8U i;
	uint16_t crc = 0;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for (i = 0; i < 8; ++i)
		{
			if (crc & 1)
#ifndef POLY
				crc = (crc >> 1) ^ 0xA001;// 0xA001 = reverse 0x8005
#else
				crc = (crc >> 1) ^ POLY;//0xA001;// 0xA001 = reverse 0x8005
#endif
			else
				crc = (crc >> 1);
		}
	}
	return crc;
}

/******************************************************************************
 * Name:CRC-16/MAXIMx16+x15+x2+1
 * Poly:0x8005
 * Init:0x0000
 * Refin: True
 * Refout:True
 * Xorout:0xFFFF
 * Note:
 *****************************************************************************/
uint16_t crc16_maxim(INT8U *data, INT16U length)
{
	INT8U i;
	uint16_t crc = 0;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for (i = 0; i < 8; ++i)
		{
			if (crc & 1)
#ifndef POLY
				crc = (crc >> 1) ^ 0xA001;// 0xA001 = reverse 0x8005
#else
				crc = (crc >> 1) ^ POLY;//0xA001;// 0xA001 = reverse 0x8005
#endif
			else
				crc = (crc >> 1);
		}
	}
	return ~crc;// crc^0xffff
}

/******************************************************************************
 * Name:CRC-16/USBx16+x15+x2+1
 * Poly:0x8005
 * Init:0xFFFF
 * Refin: True
 * Refout:True
 * Xorout:0xFFFF
 * Note:
 *****************************************************************************/
uint16_t crc16_usb(INT8U *data, INT16U length)
{
	INT8U i;
	uint16_t crc = 0xffff;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for (i = 0; i < 8; ++i)
		{
			if (crc & 1)
#ifndef POLY
				crc = (crc >> 1) ^ 0xA001;// 0xA001 = reverse 0x8005
#else
				crc = (crc >> 1) ^ POLY;//0xA001;// 0xA001 = reverse 0x8005
#endif
			else
				crc = (crc >> 1);
		}
	}
	return ~crc;// crc^0xffff
}

/******************************************************************************
 * Name:CRC-16/MODBUS x16+x15+x2+1
 * Poly:0x8005
 * Init:0xFFFF
 * Refin: True
 * Refout:True
 * Xorout:0x0000
 * Note:
 *****************************************************************************/
uint16_t crc16_modbus(INT8U *data, INT16U length)
{
	INT8U i;
	uint16_t crc = 0xffff;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for (i = 0; i < 8; ++i)
		{
			if (crc & 1)
#ifndef POLY
				crc = (crc >> 1) ^ 0xA001;// 0xA001 = reverse 0x8005
#else
				crc = (crc >> 1) ^ POLY;//0xA001;// 0xA001 = reverse 0x8005
#endif
			else
				crc = (crc >> 1);
		}
	}
	return crc;
}

/******************************************************************************
 * Name:CRC-16/CCITTx16+x12+x5+1
 * Poly:0x1021
 * Init:0x0000
 * Refin: True
 * Refout:True
 * Xorout:0x0000
 * Alias: CRC-CCITT,CRC-16/CCITT-TRUE,CRC-16/KERMIT
 *****************************************************************************/
uint16_t crc16_ccitt(INT8U *data, INT16U length)
{
	INT8U i;
	uint16_t crc = 0;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for (i = 0; i < 8; ++i)
		{
			if (crc & 1)
#ifndef POLY
				crc = (crc >> 1) ^ 0x8408;// 0x8408 = reverse 0x1021
#else
				crc = (crc >> 1) ^ POLY;//0x8408;// 0x8408 = reverse 0x1021
#endif
			else
				crc = (crc >> 1);
		}
	}
	return crc;
}

/******************************************************************************
 * Name:CRC-16/CCITT-FALSE x16+x12+x5+1
 * Poly:0x1021
 * Init:0xFFFF
 * Refin: False
 * Refout:False
 * Xorout:0x0000
 * Note:
 *****************************************************************************/
uint16_t crc16_ccitt_false(INT8U *data, INT16U length)
{
	INT8U i;
	uint16_t crc = 0xffff;//Initial value
	while(length--)
	{
		crc ^= (uint16_t)(*data++) << 8; // crc ^= (uint6_t)(*data)<<8; data++;
		for (i = 0; i < 8; ++i)
		{
			if ( crc & 0x8000 )
#ifndef POLY
				crc = (crc << 1) ^ 0x1021;
#else
				crc = (crc << 1) ^ POLY;//0x1021;
#endif
			else
				crc <<= 1;
		}
	}
	return crc;
}

/******************************************************************************
 * Name:CRC-16/X25x16+x12+x5+1
 * Poly:0x1021
 * Init:0xFFFF
 * Refin: True
 * Refout:True
 * Xorout:0XFFFF
 * Note:
 *****************************************************************************/
uint16_t crc16_x25(INT8U *data, INT16U length)
{
	INT8U i;
	uint16_t crc = 0xffff;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for (i = 0; i < 8; ++i)
		{
			if (crc & 1)
#ifndef POLY
				crc = (crc >> 1) ^ 0x8408;// 0x8408 = reverse 0x1021
#else
				crc = (crc >> 1) ^ POLY;//0x8408;// 0x8408 = reverse 0x1021
#endif
			else
				crc = (crc >> 1);
		}
	}
	return ~crc;// crc^Xorout
}

/******************************************************************************
 * Name:CRC-16/XMODEM x16+x12+x5+1
 * Poly:0x1021
 * Init:0x0000
 * Refin: False
 * Refout:False
 * Xorout:0x0000
 * Alias: CRC-16/ZMODEM,CRC-16/ACORN
 *****************************************************************************/
uint16_t crc16_xmodem(INT8U *data, INT16U length)
{
	INT8U i;
	uint16_t crc = 0;// Initial value
	while(length--)
	{
		crc ^= (uint16_t)(*data++) << 8; // crc ^= (uint16_t)(*data)<<8; data++;
		for (i = 0; i < 8; ++i)
		{
			if ( crc & 0x8000 )
#ifndef POLY
				crc = (crc << 1) ^ 0x1021;
#else
				crc = (crc << 1) ^ POLY;//0x1021;
#endif
			else
				crc <<= 1;
		}
	}
	return crc;
}

/******************************************************************************
 * Name:CRC-16/DNPx16+x13+x12+x11+x10+x8+x6+x5+x2+1
 * Poly:0x3D65
 * Init:0x0000
 * Refin: True
 * Refout:True
 * Xorout:0xFFFF
 * Use: M-Bus,ect.
 *****************************************************************************/
uint16_t crc16_dnp(INT8U *data, INT16U length)
{
	INT8U i;
	uint16_t crc = 0;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for (i = 0; i < 8; ++i)
		{
			if (crc & 1)
#ifndef POLY
				crc = (crc >> 1) ^ 0xA6BC;// 0xA6BC = reverse 0x3D65
#else
				crc = (crc >> 1) ^ POLY;//0xA6BC;// 0xA6BC = reverse 0x3D65
#endif
			else
				crc = (crc >> 1);
		}
	}
	return ~crc;// crc^Xorout
}

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
INT32U crc32(INT8U *data, INT16U length)
{
	INT8U i;
	INT32U crc = 0xffffffff;// Initial value
	while(length--)
	{
		crc ^= *data++;// crc ^= *data; data++;
		for (i = 0; i < 8; ++i)
		{
			if (crc & 1)
#ifndef POLY
				crc = (crc >> 1) ^ 0xEDB88320;// 0xEDB88320= reverse 0x04C11DB7
#else
				crc = (crc >> 1) ^ POLY;//0xEDB88320;// 0xEDB88320= reverse 0x04C11DB7
#endif
			else
				crc = (crc >> 1);
		}
	}
	return ~crc;
}

/******************************************************************************
 * Name:CRC-32/MPEG-2x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
 * Poly:0x4C11DB7
 * Init:0xFFFFFFF
 * Refin: False
 * Refout:False
 * Xorout:0x0000000
 * Note:
 *****************************************************************************/
INT32U crc32_mpeg_2(INT8U *data, INT16U length)
{
	INT8U i;
	INT32U crc = 0xffffffff;// Initial value
	while(length--)
	{
		crc ^= (INT32U)(*data++) << 24;// crc ^=(INT32U)(*data)<<24; data++;
		for (i = 0; i < 8; ++i)
		{
			if ( crc & 0x80000000 )
#ifndef POLY
				crc = (crc << 1) ^ 0x04C11DB7;
#else
				crc = (crc << 1) ^ POLY;//0x04C11DB7;
#endif
			else
				crc <<= 1;
		}
	}
	return crc;
}

