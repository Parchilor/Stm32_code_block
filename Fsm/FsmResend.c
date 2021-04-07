#include <string.h>
#include "FsmEvent.h"
#include "FsmResend.h"

#define RESEND_STATE_REGISTBIT  (0x01 << 0)
#define RESEND_STATE_STARTBIT   (0x01 << 1)
#define RESEND_STATE_ENDBIT     (0x01 << 2)

#define ReSend_Timer                TIM4
#define ReSend_Timer_Handle         ReSndTim
#define ReSend_Tim_IRQn             TIM4_IRQn
#define ReSend_Timer_IRQHandler     TIM4_IRQHandler

#if !defined (configRESND_MAX_QUEUE)
 #define configRESND_MAX_QUEUE      (16)
#endif
#if !defined (configRESND_MAX_DATA)
 #define configRESND_MAX_DATA       (64)
#endif
#if !defined (configRESND_EVENT_NBR)
 #define configRESND_EVENT_NBR      (0)
#endif

typedef struct
{
    INT8U Register; /**
                     * Bit0: ReSend Regist Bit
                     * Bit1: ReSend Start Bit
                     */
    INT8U Event;
    INT8U Times;
    INT16U Timeout;
    INT16U Counter;

    INT8U SndPort;
    void *pParam;
    INT16U DLC;
    pReSndFunc_t pReSndFunc;
} sFSMReSndCB_t;    // 重发控制块

static INT8U ReSndRegistryCnt = 0;
static sFSMReSndCB_t ReSndCtrl[configRESND_MAX_QUEUE] = {0};
static INT8U ReSndData[configRESND_MAX_QUEUE][configRESND_MAX_DATA];

TIM_HandleTypeDef ReSndTim;

void FSM_ReSndTimerStart(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    __HAL_RCC_TIM4_CLK_ENABLE();

    HAL_NVIC_SetPriority(ReSend_Tim_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ReSend_Tim_IRQn);

    /**
     * @details
     * Prescaler = 7199; Period = 9999; ====> 1s
     * Prescaler = 7199; Period = 999;  ====> 100ms
     * Prescaler = 7199; Period = 499;  ====> 20ms
     */
    ReSend_Timer_Handle.Instance = ReSend_Timer;
    ReSend_Timer_Handle.Init.Prescaler = 7199;
    ReSend_Timer_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    ReSend_Timer_Handle.Init.Period = 499;
    ReSend_Timer_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    ReSend_Timer_Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&ReSend_Timer_Handle) != HAL_OK)
    {
        // Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&ReSend_Timer_Handle, &sClockSourceConfig) != HAL_OK)
    {
        // Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&ReSend_Timer_Handle, &sMasterConfig) != HAL_OK)
    {
        // Error_Handler();
    }

    __HAL_TIM_ENABLE_IT(&ReSend_Timer_Handle, TIM_IT_UPDATE);
    HAL_TIM_Base_Start_IT(&ReSend_Timer_Handle);
}

void FSM_ReSndTimerStop(void)
{
    HAL_TIM_Base_Stop(&ReSend_Timer_Handle);
    __HAL_TIM_DISABLE_IT(&ReSend_Timer_Handle, TIM_IT_UPDATE);

    __HAL_RCC_TIM4_CLK_DISABLE();
    HAL_NVIC_DisableIRQ(ReSend_Tim_IRQn);
}

/**
 * @brief 重发函数注册
 * @param port 指定重发端口
 * @param pSndFunc 重发函数
 * @retval 重发控制块指针
 */
FSMReSndCBHandle_t FSM_ReSndCreate(INT8U port, pReSndFunc_t pSndFunc, INT8U event)
{
    sFSMReSndCB_t *pHandler = NULL;
    INT8U *ptr = NULL;
    INT8U nRegistry;

    if(ReSndRegistryCnt < configRESND_MAX_QUEUE)
    {
        for(nRegistry = 0; nRegistry < configRESND_MAX_QUEUE; nRegistry++)
        {
            if(READ_BIT(ReSndCtrl[nRegistry].Register, RESEND_STATE_REGISTBIT))
            {
                // 已被注册
            }
            else
            {
                // 未被注册
                pHandler = &ReSndCtrl[nRegistry];
                ptr = ReSndData[nRegistry];
                break;
            }
        }
        ENTER_CRITICAL_SECTION();
        pHandler->Event = event;
        pHandler->SndPort = port;
        pHandler->pParam = ptr;
        if(pSndFunc != NULL)
        {
            pHandler->pReSndFunc = pSndFunc;
        }
        SET_BIT(pHandler->Register, RESEND_STATE_REGISTBIT);
        ++ReSndRegistryCnt;
        EXIT_CRITICAL_SECTION();
        return pHandler;
    }
    else
    {
        // 超出重发控制块可分配最大数量
        return NULL;
    }
}

/**
 * @brief 删除已注册的重发控制块并重置数据
 * @param xReSndCB 重发控制块指针
 */
void FSM_ReSndDelete(FSMReSndCBHandle_t xReSndCB)
{
    sFSMReSndCB_t *const pReSndCb = (sFSMReSndCB_t *) xReSndCB;
    if(READ_BIT(pReSndCb->Register, RESEND_STATE_REGISTBIT))
    {
        ENTER_CRITICAL_SECTION();
        pReSndCb->Register = 0;
        pReSndCb->Times = 0;
        pReSndCb->Timeout = 0;
        pReSndCb->Counter = 0;
        pReSndCb->Event = NULL;

        pReSndCb->SndPort = 0;
        memset(pReSndCb->pParam, 0, pReSndCb->DLC);
        pReSndCb->pParam = NULL;
        pReSndCb->DLC = 0;
        pReSndCb->pReSndFunc = NULL;
        EXIT_CRITICAL_SECTION();
    }
}

/**
 * @brief 启动重发
 * @param xReSndCB 重发控制块指针
 * @param pData 重发的数据
 * @param dlc 重发数据的长度
 * @param times 重发次数
 * @param timeout 重发间隔（以ReSend_Timer的设定为时基；计算公式：ReSend_Timer定时时间 * timeout）
 * @retval 启动成功: TRUE;
 *         重发已经被启动: FALSE;
 */
INT8U FSM_ReSndStart(FSMReSndCBHandle_t xReSndCB, INT8U *pData, INT16U dlc, INT8U times, INT16U timeout)
{
    INT8U result;
    sFSMReSndCB_t *const pReSndCb = (sFSMReSndCB_t *)xReSndCB;
    ASSERT(pReSndCb);

    if(READ_BIT(pReSndCb->Register, RESEND_STATE_REGISTBIT))
    {
        if(READ_BIT(pReSndCb->Register, RESEND_STATE_STARTBIT))
        {
            // 重发已启动
            result = FALSE;
        }
        else
        {
            ENTER_CRITICAL_SECTION();
            pReSndCb->Times = times;
            pReSndCb->Timeout = timeout;
            pReSndCb->Counter = 0;
            memcpy(pReSndCb->pParam, pData, dlc);
            pReSndCb->DLC = dlc;

            SET_BIT(pReSndCb->Register, RESEND_STATE_STARTBIT);
            result = TRUE;
            EXIT_CRITICAL_SECTION();
        }
    }
    return result;
}

/**
 * @brief 停止重发并清除重发数据
 * @param xReSndCB 重发控制块指针
 */
void FSM_ReSndStop(FSMReSndCBHandle_t xReSndCB)
{
    sFSMReSndCB_t *const pReSndCb = (sFSMReSndCB_t *)xReSndCB;
    ASSERT(pReSndCb);
    if(READ_BIT(pReSndCb->Register, RESEND_STATE_REGISTBIT))
    {
        if(READ_BIT(pReSndCb->Register, RESEND_STATE_STARTBIT))
        {
            ENTER_CRITICAL_SECTION();
            pReSndCb->Times = 0;
            pReSndCb->Timeout = 0;
            pReSndCb->Counter = 0;

            memset(pReSndCb->pParam, 0, pReSndCb->DLC);
            pReSndCb->DLC = 0;
            CLEAR_BIT(pReSndCb->Register, RESEND_STATE_STARTBIT);
            EXIT_CRITICAL_SECTION();
        }
    }
}

void FSM_ReSndSelect(sFSMReSndCB_t * const pReSndCb)
{
    // sFSMReSndCB_t *const pReSndCb = (sFSMReSndCB_t *)xReSndCB;
    ASSERT(pReSndCb);
    if(READ_BIT(pReSndCb->Register, RESEND_STATE_REGISTBIT))
    {
        if(READ_BIT(pReSndCb->Register, RESEND_STATE_STARTBIT))
        {
            ENTER_CRITICAL_SECTION();
            // 重发已启动
            if(pReSndCb->Times > 0)
            {
                ++pReSndCb->Counter;
                if(pReSndCb->Counter >= pReSndCb->Timeout)
                {
                    pReSndCb->Counter = 0;
                    // 重发次数为 0xFF 时代表一直重发，直到手动停止
                    if(pReSndCb->Times != 0xFF)
                    {
                        --pReSndCb->Times;
                    }

//                    FSM_EventPost(configRESND_EVENT_NBR, &pReSndCb->Event);
                    FSM_EventPost(configRESND_EVENT_NBR, pReSndCb->Event);
                }
            }
            else
            {
                // 已达重发最大次数
                pReSndCb->Times = 0;
                pReSndCb->Timeout = 0;
                pReSndCb->Counter = 0;

                memset(pReSndCb->pParam, 0, pReSndCb->DLC);
                pReSndCb->DLC = 0;
                CLEAR_BIT(pReSndCb->Register, RESEND_STATE_STARTBIT);
            }
            EXIT_CRITICAL_SECTION();
        }
    }
}

void FSM_ReSndPoll(void)
{
    for(int i = 0; i < ReSndRegistryCnt; ++i)
    {
        FSM_ReSndSelect(&ReSndCtrl[i]);
    }
}

void FSM_ReSndHandler(void)
{
    INT8U uEvent = 0;
    if(FSM_EventAccept(configRESND_EVENT_NBR, &uEvent))
    {
        for(INT8U i = 0; i < ReSndRegistryCnt; ++i)
        {
            if((uEvent == ReSndCtrl[i].Event) && (ReSndCtrl[i].pReSndFunc != NULL))
            {
                ReSndCtrl[i].pReSndFunc(ReSndCtrl[i].SndPort, ReSndCtrl[i].pParam, ReSndCtrl[i].DLC);
            }
        }
    }
}

void ReSend_Timer_IRQHandler(void)
{
    if(__HAL_TIM_GET_FLAG(&ReSend_Timer_Handle, TIM_FLAG_UPDATE))
    {
        FSM_ReSndPoll();
        __HAL_TIM_CLEAR_FLAG(&ReSend_Timer_Handle, TIM_FLAG_UPDATE);
    }
}
