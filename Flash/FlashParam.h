#ifndef __FLASH_PARAMETER_H_
#define __FLASH_PARAMETER_H_

#ifdef __cplusplus
 extern "C" {
#endif
#include "FsmPort.h"

#define PARAMETER_BASE_ADDRESS  (0x0800FC00)
#define PARAMETER_OFFSET        (0)

extern const uint16_t TemperatureStep[2][11];
extern uint16_t TemperatureStep_Param[2][11];

void FlashStoreParam(void);
void FlashLoadParam(void);
#ifdef __cplusplus
 }
#endif

#endif /* __FLASH_PARAMETER_H_ */