#ifndef __NET_H_
#define __NET_H_

#include "FSM/FsmPort.h"

#define NET_SELECT_FUNCTION_MODE_ENABLED

#define TCP_DATA_DLC  (1024)       /*!< 协议最多只让传255个字节 */

typedef enum
{
    eNET_PHY_STATUS_UP = 0, 
    eNET_PHY_STATUS_DOWN,
} eNet_PHY_Status_t;

typedef enum
{
    eNET_NETIF_STATUS_UP = 0, 
    eNET_NETIF_STATUS_DOWN, 
}eNet_Netif_Status_t;

typedef enum
{
    eNET_STATUS_INIT = 0, 
    eNET_STATUS_SELECT , 
    eNET_STATUS_ACCEPT, 
    eNET_STATUS_NOCONNECT, 
    eNET_STATUS_CONNECTED, 
    eNET_STATUS_DISCONNECTED, 
    eNET_STATUS_ERROR, 
} eNet_Status_t;

extern INT8S TempSock;
extern INT8U TCP_Link_State;

extern eNet_Status_t Net_LinkStatus;
extern eNet_PHY_Status_t Net_PhyStatus;
extern eNet_Netif_Status_t Net_NetifStatus;
/**
 * @brief TCP任务
 * @param arg 保留
 * @retval 无
 */
void StartLwipTask(void *arg);
void Net_LinkupCheck(void);
void Net_NetifupCheck(void);

#endif