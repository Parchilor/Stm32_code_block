#ifndef __FSM_RESNED_H_
#define __FSM_RESNED_H_

#include "FsmPort.h"

typedef void *FSMReSndCBHandle_t;

typedef void (*pReSndFunc_t)(INT8U , const void * const, INT16U);

void FSM_ReSndTimerStart(void);
void FSM_ReSndTimerStop(void);
FSMReSndCBHandle_t FSM_ReSndCreate(INT8U port, pReSndFunc_t pSndFunc, INT8U event);
void FSM_ReSndDelete(FSMReSndCBHandle_t xReSndCB);
INT8U FSM_ReSndStart(FSMReSndCBHandle_t xReSndCB, INT8U *pData, INT16U dlc, INT8U times, INT16U timeout);
void FSM_ReSndStop(FSMReSndCBHandle_t xReSndCB);
void FSM_ReSndPoll(void);
void FSM_ReSndHandler(void);
#endif /* __FSM_RESNED_H_ */
