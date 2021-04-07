#include <string.h>
#include "FsmQueue.h"

typedef struct
{
  INT8U qReg;
  INT32U qItemSize;
  void *qParam;
} FSMQueue_t;

/**
 * @brief Signal Queue. Haven't RTOS
 * **************************************************************************************************
 * @attention
 *    Maximent Queue size is default to configFSM_MAX_QUEUE_SIZE(8)
 *    Maximent Parameter size is default to configFSM_MAX_PARAM_SIZE_PER_QUEUE(64) per Queue.
 */
#define QUEUE_STATE_REGISTBIT   (0x01 << 0)
#define QUEUE_STATE_SIGBIT      (0x01 << 1)

#if !defined (configFSM_MAX_QUEUE_SIZE)
 #define configFSM_MAX_QUEUE_SIZE            (8)
#endif
#if !defined (configFSM_MAX_PARAM_SIZE_PER_QUEUE)
 #define configFSM_MAX_PARAM_SIZE_PER_QUEUE  (64)
#endif

static INT8U qRegistryCnt = 0;

static FSMQueue_t qList[configFSM_MAX_QUEUE_SIZE] = {0};

static INT8U qData[configFSM_MAX_QUEUE_SIZE][configFSM_MAX_PARAM_SIZE_PER_QUEUE] = {0};

FSMQueueHandle_t FSMQueueCreate(INT32U ItemNbr, INT32U ItemSize)
{
//   UNUSED(ItemNbr);
  FSMQueue_t *pHandler = NULL;
  INT8U *ptr = NULL;
  INT32U nRegistry;

  if((qRegistryCnt >= configFSM_MAX_QUEUE_SIZE) || (ItemSize > configFSM_MAX_QUEUE_SIZE * configFSM_MAX_PARAM_SIZE_PER_QUEUE))
  {
    // 注册数量大于最大队列数或成员大小大于定义的缓存大小
    return NULL;
  }

  for(nRegistry = 0; nRegistry < configFSM_MAX_QUEUE_SIZE; nRegistry++)
  {
    if(READ_BIT(qList[nRegistry].qReg, QUEUE_STATE_REGISTBIT))
    {
      // 已被注册
    }
    else
    {
      // 未被注册
      pHandler = &qList[nRegistry];
      ptr = qData[nRegistry];
      break;
    }
  }

  ENTER_CRITICAL_SECTION();
  pHandler->qItemSize = ItemSize;
  pHandler->qParam = (void *)ptr;
  SET_BIT(pHandler->qReg, QUEUE_STATE_REGISTBIT);
  ++qRegistryCnt;
  EXIT_CRITICAL_SECTION();
  return pHandler;
}

void FSMQueueDelete(FSMQueueHandle_t xQueue)
{
  FSMQueue_t * const pQueue = (FSMQueue_t *)xQueue;
  if(READ_BIT(pQueue->qReg, QUEUE_STATE_REGISTBIT))
  {
    ENTER_CRITICAL_SECTION();
    memset(pQueue->qParam, 0, pQueue->qItemSize);
    pQueue->qParam = NULL;
    pQueue->qItemSize = 0;
    CLEAR_REG(pQueue->qReg);
    EXIT_CRITICAL_SECTION();
  }
}

INT8U FSMQueueSend(FSMQueueHandle_t xQueue, const void *const pItemToQueue, INT32U xTimeout)
{
//   UNUSED(xTimeout);
  INT8U ret = TRUE;
  FSMQueue_t * const pQueue = (FSMQueue_t *)xQueue;

  ASSERT(pQueue);
  ASSERT(!((pItemToQueue == NULL) && (pQueue->qItemSize != 0)));

  if(READ_BIT(pQueue->qReg, QUEUE_STATE_REGISTBIT))
  {
    if(READ_BIT(pQueue->qReg, QUEUE_STATE_SIGBIT))
    {
      // 上一信号未处理，当前信号忽略
      ret = FALSE;
    }
    else
    {
      ENTER_CRITICAL_SECTION();
      memcpy(pQueue->qParam, pItemToQueue, pQueue->qItemSize);
      SET_BIT(pQueue->qReg, QUEUE_STATE_SIGBIT);
      EXIT_CRITICAL_SECTION();
    }
  }
  return ret;
}

INT8U FSMQueueReceive(FSMQueueHandle_t xQueue, void *const pBuffer, INT32U xTimeout)
{
  INT8U ret = FALSE;
  INT32U startTick;
  FSMQueue_t * const pQueue = (FSMQueue_t *)xQueue;

  ASSERT(pQueue);
  ASSERT(!((pBuffer == NULL) && (pQueue->qItemSize != 0)));

  if(READ_BIT(pQueue->qReg, QUEUE_STATE_REGISTBIT))
  {
    startTick = HAL_GetTick();
    do{
      if(READ_BIT(pQueue->qReg, QUEUE_STATE_SIGBIT))
      {
        ENTER_CRITICAL_SECTION();
        memcpy(pBuffer, pQueue->qParam, pQueue->qItemSize);
        CLEAR_BIT(pQueue->qReg, QUEUE_STATE_SIGBIT);
        EXIT_CRITICAL_SECTION();
        ret = TRUE;
        return ret;
      }
      else
      {
        // 上一信号未处理
      }
    }while(HAL_GetTick() - startTick < xTimeout);
  }
  return ret;
}
