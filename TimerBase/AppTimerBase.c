#include "FsmEvent.h"
#include "FsmResend.h"

// __root const INT8U Copyright[]@0x0800A000 = {"*** Copyright DAYX ENGINEER XuDong, (c) "__DATE__" ***"};
const INT8U Copyright[] = "Copyright (c) 2021 By Parchilor, All rights reserved";

void App_5Ms_Process(void)
{
    // Uart_Printf("App_5Ms_Process\r\n");
}

void App_10Ms_Process(void)
{
    // Uart_Printf("App_10Ms_Process\r\n");
    FSM_ReSndHandler();
}

void App_50Ms_Process(void)
{
    // Uart_Printf("App_50Ms_Process\r\n");
}

void App_100Ms_Process(void)
{
    // Uart_Printf("App_100Ms_Process\r\n");
}

void App_500Ms_Process(void)
{
    // Uart_Printf("App_500Ms_Process\r\n");
}

void App_1S_Process(void)
{
    // Uart_Printf("App_1S_Process\r\n");
}

void App_5S_Process(void)
{
    // Uart_Printf("App_5S_Process\r\n");
}

void App_1M_Process(void)
{
    // Uart_Printf("%s\r\n", Copyright);
}