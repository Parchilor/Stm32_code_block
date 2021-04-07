#include "FsmPort.h"

extern void App_5Ms_Process(void);
extern void App_10Ms_Process(void);
extern void App_50Ms_Process(void);
extern void App_100Ms_Process(void);
extern void App_500Ms_Process(void);
extern void App_1S_Process(void);
extern void App_5S_Process(void);
extern void App_1M_Process(void);

static __IO INT32U TimeBaseCount    = 0;
static __IO INT8U TimeBase_5MS      = 0;
static __IO INT8U TimeBase_10MS     = 0;
static __IO INT8U TimeBase_50MS     = 0;
static __IO INT8U TimeBase_100MS    = 0;
static __IO INT8U TimeBase_500MS    = 0;
static __IO INT8U TimeBase_1S       = 0;
static __IO INT8U TimeBase_5S       = 0;
static __IO INT8U TimeBase_1M       = 0;

void TimeBase_Init(void)
{
    TimeBaseCount     = 0;
    TimeBase_10MS     = 0;
    TimeBase_50MS     = 0;
    TimeBase_100MS    = 0;
    TimeBase_500MS    = 0;
    TimeBase_1S       = 0;
    TimeBase_5S       = 0;
    TimeBase_1M       = 0;
    TimeBaseCount = HAL_GetTick();
}

static void TimeProcess_1MIN(void)
{
    App_1M_Process();
    TimeBase_1M++;
    do
    {
        if(TimeBase_1M < 1) {break;}
        TimeBase_1M -= 1;
        // 1min process
    } while (0UL);
}
static void TimeProcess_5S(void)
{
    App_5S_Process();
    TimeBase_5S++;
    do
    {
        if(TimeBase_5S < 12) {break;}
        TimeBase_5S -= 12;
        // 
        TimeProcess_1MIN();
    } while (0UL);
}
static void TimeProcess_1S(void)
{
    App_1S_Process();
    TimeBase_1S++;
    do
    {
        if(TimeBase_1S < 5) {break;}
        TimeBase_1S -= 5;
        // 
        TimeProcess_5S();
    } while (0UL);
}
static void TimeProcess_500MS(void)
{
    App_500Ms_Process();
    TimeBase_500MS++;
    do
    {
        if(TimeBase_500MS < 2) {break;}
        TimeBase_500MS -= 2;
        // 
        TimeProcess_1S();
    } while (0UL);
}
static void TimeProcess_100MS(void)
{
    App_100Ms_Process();
    TimeBase_100MS++;
    do
    {
        if(TimeBase_100MS < 5) {break;}
        TimeBase_100MS -= 5;
        // 
        TimeProcess_500MS();
    } while (0UL);
}
static void TimeProcess_50MS(void)
{
    App_50Ms_Process();
    TimeBase_50MS++;
    do
    {
        if(TimeBase_50MS < 2) {break;}
        TimeBase_50MS -= 2;
        // 
        TimeProcess_100MS();
    } while (0UL);
}
static void TimeProcess_10MS(void)
{
    App_10Ms_Process();
    TimeBase_10MS++;
    do
    {
        if(TimeBase_10MS < 5) {break;}
        TimeBase_10MS -= 5;
        // 
        TimeProcess_50MS();
    } while (0UL);
}
static void TimeProcess_5MS(void)
{
    App_5Ms_Process();
    TimeBase_5MS++;
    do
    {
        if(TimeBase_5MS < 2) {break;}
        TimeBase_5MS -= 2;
        // 
        TimeProcess_10MS();
    } while (0UL);
}

void TimeBase_Process(void)
{
    INT32U tCnt;
    do{
        tCnt = HAL_GetTick();
        if(tCnt - TimeBaseCount < 5) {break;}
        TimeBaseCount += 5;
        TimeProcess_5MS();
    }while(0UL);
}