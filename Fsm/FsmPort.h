#ifndef __FSM_PORT_H_
#define __FSM_PORT_H_

#include <stddef.h>
#include "stm32f1xx_hal.h"
#include "Legacy/stm32_hal_legacy.h"

#ifndef FIRMWARE_VERSION
 #define FIRMWARE_MAJOR_VERSION		(1)
 #define FIRMWARE_MINOR_VERSION		(4)
 #define FIRMWARE_PATCH_VERION		(346)

 #define FIRMWARE_VERSION			((FIRMWARE_MAJOR_VERSION) << 24 | (FIRMWARE_MINOR_VERSION) << 16 | (FIRMWARE_PATCH_VERION))
#endif /* DEBUG_MODE_VERSION */

typedef unsigned char		INT8U;
typedef unsigned short		INT16U;
typedef unsigned int		INT32U;
typedef signed char			INT8S;
typedef signed short		INT16S;
typedef signed int			INT32S;

typedef unsigned char		BOOL;

#ifndef TRUE
 #define TRUE  1
#endif

#ifndef FALSE
 #define FALSE 0
#endif

#define nOSTimeDly HAL_Delay
#define osTimeDly osDelay
#define OS_TICKS_PER_SEC configTICK_RATE_HZ

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(*arr))

#ifndef ENTER_CRITICAL_SECTION
 #define ENTER_CRITICAL_SECTION()       if(!IsWithinException()){__set_BASEPRI(0);}
#endif

#ifndef EXIT_CRITICAL_SECTION
 #define EXIT_CRITICAL_SECTION()        if(!IsWithinException()){__set_BASEPRI(1);}
#endif

#ifndef ASSERT
 #define ASSERT(x)  if((x) == 0) {ENTER_CRITICAL_SECTION(); for(;;){}}
#endif

/**
 * @brief Char array convert to unsigned int used big ending
 * @param pstr char array
 * @retval unsigned int 
 */
static inline uint32_t stohl(uint8_t *pstr)
{
	return (uint32_t)(pstr[0] << 24 | pstr[1] << 16 | pstr[2] << 8 | pstr[3]);
}

/**
 * @brief Char array convert to unsigned short used big ending
 * @param pstr char array
 * @retval unsigned short 
 */
static inline uint16_t stohs(uint8_t *pstr)
{
	return (uint16_t)(pstr[0] << 8 | pstr[1]);
}

/**
 * @brief Comfirm 
 */
BOOL IsWithinException(void);

#endif