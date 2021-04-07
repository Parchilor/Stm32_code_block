#include "net.h"
#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "ethernetif.h"

#include "lwip/sockets.h"
#include "lwip/err.h"
#include "lwip/sys.h"

/**
 * @brief 网络状态机测试
 * 
 */
extern eNet_Status_t Net_LinkStatus = eNET_STATUS_INIT;
extern eNet_PHY_Status_t Net_PhyStatus = eNET_PHY_STATUS_DOWN;
extern eNet_Netif_Status_t Net_NetifStatus = eNET_NETIF_STATUS_DOWN;

INT8S TempSock;
INT8U TCP_Link_State;

INT8U tcp_recv_cache[TCP_DATA_DLC];
INT8U eth_to_uart_cache[TCP_DATA_DLC];
INT16U e2u_dlc = 0;

#define TCP_DEFAULT_Ip          "192.168.1.212"
#define TCP_DEFAULT_Gateway     "192.168.1.1"
#define TCP_DEFAULT_Mark        "255.255.255.0"
#define TCP_DEFAULT_Dns         "192.168.1.1"
#define	TCP_DEFAULT_Port        2020

#define MAX_ALIVE_COUNT	(4)		// 1.0s * MAX_ALIVE_COUNT(4) = 4s

INT32S LocalSock = -1;
INT16S aliveCnt = 0;

INT32S Net_Init(void)
{
    INT32S tSocket = -1, ret;
    struct sockaddr_in serveraddr;
    serveraddr.sin_family  = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port    = htons(TCP_DEFAULT_Port);
    memset(&(serveraddr.sin_zero), 0, sizeof(serveraddr.sin_zero));
    while (1) {
        tSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (tSocket >= 0) {
            break;
        }
        osTimeDly(OS_TICKS_PER_SEC / 10);
    }
    while (1) {
        ret = bind(tSocket,(struct sockaddr *)&serveraddr,sizeof(serveraddr)); 
        if (ret >= 0) {    
            break;
        }
        osTimeDly(OS_TICKS_PER_SEC / 10);
    }
    while (1) {
        ret = listen(tSocket, 2);
        if (ret >= 0)  {
            break;
        }
    }
    return tSocket;
}

INT8S Net_Accept(INT32S sock)
{
    INT8S retSock;
    struct sockaddr_in clientaddr;
    INT32S socklen;
    socklen = sizeof(clientaddr);
    retSock = accept(sock, (struct sockaddr *)&clientaddr, (socklen_t *)&socklen);
    return retSock;
}

eNet_Status_t Net_Disconnect(INT8S sock)
{
    INT32S ret;
    ret = close(sock);
    return ret != 0 ? eNET_STATUS_ERROR : eNET_STATUS_SELECT;
}

void Net_LinkupCheck(void)
{
    if(netif_is_link_up(&gnetif))
    {
        Net_PhyStatus = eNET_PHY_STATUS_UP;
    }
    else
    {
        Net_PhyStatus = eNET_PHY_STATUS_DOWN;
    }
}

void Net_NetifupCheck(void)
{
    if(netif_is_up(&gnetif))
    {
        Net_NetifStatus = eNET_NETIF_STATUS_UP;
    }
    else
    {
        Net_NetifStatus = eNET_NETIF_STATUS_DOWN;
    }
}

/**
 * @brief TCP任务
 * @param arg 保留
 * @retval 无
 */
void StartLwipTask(void *arg) /*!< TCP Communication and Frame Decode */
{
    INT32S rxlen = 0;
  
    INT8U *pHead = tcp_recv_cache;

    fd_set rxSet;
    INT32S ret = -1;
    struct timeval RxTimeout = {1, 0}; // 1.0s
    struct timeval SelectTimeout = {0, 144}; // 

    while(1)
    {
        osTimeDly(OS_TICKS_PER_SEC / 100); // 10ms

        switch(Net_PhyStatus)
        {
            case eNET_PHY_STATUS_UP:
                if(!netif_is_up(&gnetif))
                {
                    netif_set_up(&gnetif);
                }
                break;
            case eNET_PHY_STATUS_DOWN:
                if(netif_is_up(&gnetif))
                {
                    netif_set_down(&gnetif);
                }
                break;
        }

        switch(Net_LinkStatus)
        {
            case eNET_STATUS_INIT:
                if(Net_NetifStatus == eNET_NETIF_STATUS_UP)
                {
                    LocalSock = Net_Init();
                    
                    Net_LinkStatus = eNET_STATUS_SELECT;
                }
                break;
            case eNET_STATUS_SELECT:
                FD_ZERO(&rxSet);
                FD_SET(LocalSock, &rxSet);
                ret = select(LocalSock + 1, &rxSet, NULL, NULL, &SelectTimeout);
                if(ret > 0)
                {
                    Net_LinkStatus = eNET_STATUS_ACCEPT;
                }
                break;
            case eNET_STATUS_ACCEPT:
                if(Net_NetifStatus == eNET_NETIF_STATUS_UP)
                {
                    TempSock = Net_Accept(LocalSock);
                    if(TempSock >= 0)
                    {
                        Net_LinkStatus = eNET_STATUS_CONNECTED;
                        aliveCnt = 0;
                        if(setsockopt(TempSock, SOL_SOCKET, SO_RCVTIMEO, &RxTimeout, sizeof(struct timeval)) < 0)
                        {
                            Net_LinkStatus = eNET_STATUS_ERROR;
                        }
                    }
                }
                break;
            case eNET_STATUS_CONNECTED:
                if(Net_NetifStatus == eNET_NETIF_STATUS_UP)
                {
                    rxlen = recv(TempSock, tcp_recv_cache, 1000, 0);
                    if(rxlen < 0)
                    {
                        if(errno == EWOULDBLOCK)
                        {
                            // Timeout
                            ++aliveCnt;
                            if(aliveCnt >= MAX_ALIVE_COUNT)
                            {
                                Net_LinkStatus = eNET_STATUS_DISCONNECTED;
                                // continue;
                            }	
                        }
                        else if(errno == ENOTCONN | errno == ECONNABORTED | errno == ECONNRESET)
                        {
                            Net_LinkStatus = eNET_STATUS_DISCONNECTED;
                        }
                        else
                        {
                            Net_LinkStatus = eNET_STATUS_DISCONNECTED;
                        }
                    }
                    else
                    {
                        aliveCnt = 0;
                        memcpy(eth_to_uart_cache, tcp_recv_cache, rxlen);
                        e2u_dlc = rxlen;
                        
                        memset(pHead, 0, rxlen);
                        pHead = tcp_recv_cache;
                    }
                }
                else
                {
                    Net_LinkStatus = eNET_STATUS_DISCONNECTED;
                }
                break;
            case eNET_STATUS_DISCONNECTED:
                Net_LinkStatus = Net_Disconnect(TempSock);
                break;
            case eNET_STATUS_ERROR:
                // lwip API执行出错
                // TODO
                break;
        }
    }
}
