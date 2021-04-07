#ifndef __FSM_EVENT_H_
#define __FSM_EVENT_H_

#include "FsmPort.h"

INT8U FSM_EventInit(INT8U idx, INT32U ItemSize);
void FSM_EventClose(INT8U idx);
INT8U FSM_EventPost(INT8U idx, INT8U event);
INT8U FSM_EventAccept(INT8U idx, INT8U *event);

#endif /* __FSM_EVENT_H_ */