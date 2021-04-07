#ifndef __FSM_QUEUE_H_
#define __FSM_QUEUE_H_

#include "FsmPort.h"

typedef void *FSMQueueHandle_t;

FSMQueueHandle_t FSMQueueCreate(INT32U ItemNbr, INT32U ItemSize);
void FSMQueueDelete(FSMQueueHandle_t xQueue);
INT8U FSMQueueSend(FSMQueueHandle_t xQueue, const void *const pItemToQueue, INT32U xTimeout);
INT8U FSMQueueReceive(FSMQueueHandle_t xQueue, void *const pBuffer, INT32U xTimeout);

#endif /* __FSM_QUEUE_H_ */