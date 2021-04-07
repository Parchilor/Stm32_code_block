#include "FsmPort.h"
/**
 * @brief Comfirm 
 */
BOOL IsWithinException(void)
{
	BOOL rInISR = TRUE;
	volatile INT32U tCPSR;
	asm("MRS %0, IPSR":"=r"(tCPSR):);
	switch (tCPSR & 0x0000001F)
	{
		case 0x00000010U:
		case 0x0000001FU:
		rInISR = FALSE;
			break;
	}
	return rInISR;
}