#include "FsmQueue.h"

#if 0
 #define EventQueueHandle_t          QueueHandle_t
 #define EventQueueCreate            xQueueCreate
 #define EventQueueDelete            vQueueDelete
 #define EventQueueSendFromISR       xQueueSendFromISR
 #define EventQueueSend              xQueueSend
 #define EventQueueReceiveFromISR    xQueueReceiveFromISR
 #define EventQueueReceive           xQueueReceive
#else
 #define EventQueueHandle_t          FSMQueueHandle_t
 #define EventQueueCreate            FSMQueueCreate
 #define EventQueueDelete            FSMQueueDelete
 #define EventQueueSendFromISR       FSMQueueSend
 #define EventQueueSend              FSMQueueSend
 #define EventQueueReceiveFromISR    FSMQueueReceive
 #define EventQueueReceive           FSMQueueReceive
 #define BaseType_t                  INT8U
#ifndef pdFALSE
 #define pdFALSE                     FALSE
#endif
#ifndef pdTRUE
 #define pdTRUE                      TRUE
#endif
#endif

static EventQueueHandle_t queueList[8];

INT8U FSM_EventInit(INT8U idx, INT32U ItemSize)
{
  INT8U fOkay = pdFALSE;
  EventQueueHandle_t EventQueue;

  ENTER_CRITICAL_SECTION();
  EventQueue = EventQueueCreate(1, ItemSize);
  if(EventQueue != 0)
  {
    queueList[idx] = EventQueue;
    fOkay = pdTRUE;
  }
  EXIT_CRITICAL_SECTION();
  return fOkay;
}

void FSM_EventClose(INT8U idx)
{
  ENTER_CRITICAL_SECTION();
  if(queueList[idx] != 0)
  {
    EventQueueDelete(queueList[idx]);
    queueList[idx] = 0;
  }
  EXIT_CRITICAL_SECTION();
}

INT8U FSM_EventPost(INT8U idx, INT8U event)
{
  BaseType_t eSent = pdFALSE;
  do
  {
    ENTER_CRITICAL_SECTION();
    if(IsWithinException())
    {
      eSent = EventQueueSendFromISR(queueList[idx], (void *)&event, 0);
    }
    else
    {
      eSent = EventQueueSend(queueList[idx], (void *)&event, 0);
    }
    EXIT_CRITICAL_SECTION();
  } while (0UL);
  
  return eSent;
}

INT8U FSM_EventAccept(INT8U idx, INT8U *event)
{
  BaseType_t eRecv = pdFALSE;
  ENTER_CRITICAL_SECTION();
  if(IsWithinException())
  {
    eRecv = EventQueueReceiveFromISR(queueList[idx], event, 0);
  }
  else
  {
    eRecv = EventQueueReceive(queueList[idx], event, 50);
  }
  EXIT_CRITICAL_SECTION();
  return eRecv;
}