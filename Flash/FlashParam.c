#include "FlashParam.h"
#include "InnerFlash.h"

__root const uint16_t TemperatureStep[2][11]@(PARAMETER_BASE_ADDRESS + PARAMETER_OFFSET) = 
{
	{1339, 1222, 1081, 924, 755, 596, 452, 326, 225, 139, 72},
	{1339, 1222, 1081, 924, 755, 596, 452, 326, 225, 139, 72},
};

void FlashStoreParam(void)
{
    INT16U tRout;
    ENTER_CRITICAL_SECTION();
    FLASH_LoadHalfWord(&tRout, PARAMETER_BASE_ADDRESS);
    if(tRout != 0xFFFF)
    {
        FLASH_Erase(PARAMETER_BASE_ADDRESS);
    }
    FLASH_StoreMultiHalfWord((uint16_t *)TemperatureStep_Param, PARAMETER_BASE_ADDRESS, 2 * 11);
    EXIT_CRITICAL_SECTION();
}

void FlashLoadParam(void)
{
    FLASH_LoadMultiHalfWord((uint16_t *)TemperatureStep_Param, PARAMETER_BASE_ADDRESS, 2 * 11);
}