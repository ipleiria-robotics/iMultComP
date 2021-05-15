/*---------------------------------------------------------------------------*/

/*  Copyright (C) 2006 Woodhead Software & Electonics. All rights reserved.  */
/*---------------------------------------------------------------------------*/
/*  This program is protected by international copyright.                    */
/*                                                                           */
/*  The use of this software including but not limited to its Source Code    */
/*  is subject to restrictions as agreed in the license agreement between    */
/*  you and Woodhead.                                                        */
/*  Copying or distribution is not allowed unless expressly permitted        */
/*  according to your license agreement with Woodhead.                       */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  Project             : STACK PROFINET Controller                          */
/*  Component           : Hardware Interface                                 */
/* $Workfile:: os.h                                                         $*/
/* $Revision:: 1                                                            $*/
/* $Date:: 5/01/16 9:55                                                     $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*  Operating system Abstraction Layer                                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#ifndef _OS_H
#define _OS_H

/* Add your os include file here */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//#include <xdc/std.h>
//#include <xdc/cfg/global.h>
//#include <xdc/runtime/Error.h>
//#include <xdc/runtime/System.h>

//#include <ti/sysbios/BIOS.h>
//#include <ti/sysbios/knl/Clock.h>
//#include <ti/sysbios/knl/Task.h>
//#include <ti/sysbios/knl/Event.h>
//#include <ti/sysbios/knl/Semaphore.h>
//#include <ti/sysbios/knl/Mailbox.h>

#include <ti/ndk/inc/netmain.h>
//#include <ti/sysbios/timers/dmtimer/Timer.h>

#include <Profinet/include/RT/platform_pn.h>


//#include "plat_utils.h"
#include <ti/csl/hw_types.h>

#include <ti/drv/icss_emac/icss_emacFwInit.h>


/*...*/


/* Stack include */
#include <Profinet/include/types_pn.h>




/*define OS_DEBUG*/

#define STACK_SIZE 8192 

#define INFINITE   -1
#define ERROR      -1
#define NOERROR     0

#define THREAD_PRIORITY_NORMAL        5
#define THREAD_PRIORITY_HIGHEST       6
#define THREAD_PRIORITY_TIME_CRITICAL 7

  #define TASK_RPC_RUNTIME_PRIO       THREAD_PRIORITY_NORMAL
  #define TASK_MSG_ASYNC_PRIO         THREAD_PRIORITY_NORMAL
  #define TASK_ERROR_PRIO             THREAD_PRIORITY_NORMAL
  #define TASK_CPM_RUNTIME_PRIO       THREAD_PRIORITY_HIGHEST
  #define TASK_LMPM_RUNTIME_PRIO      THREAD_PRIORITY_TIME_CRITICAL
  #define TASK_PPM_RUNTIME_PRIO       THREAD_PRIORITY_NORMAL
  #define TASK_ASE_RUNTIME_PRIO       THREAD_PRIORITY_NORMAL
  #define TASK_ALM_RUNTIME_PRIO       THREAD_PRIORITY_HIGHEST
  #define TASK_RPC_MSSER_PRIO         THREAD_PRIORITY_NORMAL
  #define TASK_FSPM_RUNTIME_PRIO      THREAD_PRIORITY_NORMAL
  #define TASK_SCHEDULE_RUNTIME_PRIO  THREAD_PRIORITY_TIME_CRITICAL
  #define TASK_ADDIN_RUNTIME_PRIO     THREAD_PRIORITY_TIME_CRITICAL
  #define TASK_MSGPROXY_RUNTIME_PRIO  TASK_CPM_RUNTIME_PRIO
  #define TASK_PSM_RUNTIME_PRIO       TASK_CPM_RUNTIME_PRIO
  #define TASK_PTCP_RUNTIME_PRIO      THREAD_PRIORITY_NORMAL
  #define TASK_LLDP_RUNTIME_PRIO      THREAD_PRIORITY_NORMAL

#define OS_MEMCOPY   memcpy
#define OS_MEMSET    memset
#define OS_MEMCMP    memcmp
#define OS_SPRINTF   sprintf
#define OS_SNPRINTF  snprintf
#define OS_SSCANF    sscanf
#define OS_STRCPY    strcpy
#define OS_STRCAT    strcat
#define OS_STRNCAT   strncat
#define OS_FOPEN     fopen
#define OS_FCLOSE    fclose
#define OS_FREAD     fread
#define OS_FWRITE    fwrite

/**
* @def PN_TIMER_ID
*        Timer instance used by Profinet Stack
*/
#if defined(SOC_AM335x) || defined(SOC_AM437x)
#define PN_TIMER_ID	4     /*DMTIMER 6 in case of AM335x.SYSBIOS Timer Id is 4. DMTIMER 4 for AM437x*/
#define PN_TIMER_PERIOD 500
#define PN_TIMER_EXT_FREQ 24000000
#elif defined(SOC_K2G)
#define PN_TIMER_ID 4      /*DMTIMER 4 */
#define PN_TIMER_PERIOD 2500 /*TODO -Fix this. OSAL/SYSBIOS API issue?*/
#define PN_TIMER_EXT_FREQ 0
#else
#define PN_TIMER_ID TimerP_ANY
#define PN_TIMER_PERIOD 500
#define PN_TIMER_EXT_FREQ 20000000
#endif


#ifdef ACTIVATE_LOG_EVENT
  APP_VOID logAssert(APP_DWORD dwDetails);
  #ifdef OS_DEBUG
    extern APP_WORD gwStackDeviceInit;
    extern char byOsBuff[500];
    #define OS_ASSERT(a)  if(!(a)){OS_SPRINTF(byOsBuff,"ASSERT -> %s - %d \n\r",__FILE__,__LINE__);UARTPutString(uartInstance,byOsBuff); if(0 != gwStackDeviceInit){logAssert(__LINE__);}}
  #else
    #define OS_ASSERT(a)  if(!(a)){logAssert(__LINE__);}
  #endif
#endif
#ifndef ACTIVATE_LOG_EVENT
  #ifdef OS_DEBUG
    #define OS_ASSERT(a) if(!(a)) printf("ASSERT -> %s Line %d\n",__FILE__ , __LINE__)
  #else
    #define OS_ASSERT(a)
  #endif
#endif

/* NETWORK CONVERSION MACROs*/
#define HTONS NDK_htons
#define HTONL NDK_htonl

/* STRING MACROs*/
#define OS_STRNCPY strncpy
#define OS_STRLEN  strlen
#define OS_STRCMP  strcmp

/*Use buzzer for user alarm*/
#ifdef __NO_BEEP
  #define OS_BEEP(x,y)
#else
  #define OS_BEEP(x,y) Beep((x),(y));
#endif

/* GENERAL MACROs */
#ifdef OS_DEBUG

APP_VOID OsDebugPrint (char * lpszFormat,...);
APP_VOID OsPndTestHeader(char * lpszFormat);

#define PN_TEST_FAILED    OsDebugPrint ("------- ######################################## --------\n");OsDebugPrint ("####### -------------- TEST FAILED -------------- #######\n");OsDebugPrint ("\n");OsDebugPrint
#define PN_TEST_SUCCEED   OsDebugPrint
#define osPndTestFinish(testName)   OsDebugPrint("\n \n \n");\
                                    OsDebugPrint("------------------------------------------------------------\n"); \
                                    OsDebugPrint("- *** Test Finish :%s ***\n",testName); \
                                    OsDebugPrint("------------------------------------------------------------\n \n \n");
#endif

/*---------------------------- Init function -----------------------*/
APP_WORD     OsInit(void);
APP_WORD     OsExit(APP_VOID );
/*---------------------------- memory management -------------------*/
APP_WORD    _OsAlloc      (APP_LPVOID* ppMem,APP_BYTE  byValue,APP_DWORD  dwLength);
APP_WORD    _OsFree       (APP_LPVOID* ppMem);
APP_WORD    _OsAllocDebug (APP_LPVOID* ppMem,APP_BYTE byValue,APP_DWORD dwLength,APP_LPCHAR szFile,APP_DWORD dwLine);
APP_WORD    _OsFreeDebug  (APP_LPVOID *pMemchar,APP_LPCHAR szFile,APP_DWORD dwLine);
/*---------------------------- time management ---------------------*/
APP_VOID    OsWait_ms     (APP_DWORD PauseTime_ms);
APP_DWORD   OsGetTime_us  (APP_VOID);
APP_DWORD   OsGetTime_ms  (APP_VOID);
APP_DWORD   OsGetUnixTime (APP_VOID); /* Return a date from ....*/
/*---------------------------- timer management --------------------*/
APP_WORD    OsAllocTimer  (APP_VOID);
APP_WORD    OsStartTimer  (APP_WORD wDelay);
APP_WORD    OsStopTimer   (APP_WORD wTimerId);
APP_WORD    OsWaitForTimer(APP_VOID);
/*---------------------------- thread handling ---------------------*/
APP_DWORD   OsGetCurrentThreadId    (void);
APP_HANDLE  OsGetCurrentThreadHandle(void);
/*---------------------------- mutex -------------------------------*/
APP_HANDLE  OsCreateMutex   (APP_LPCHAR szName);
APP_VOID    OsCloseMutex    (APP_HANDLE hMutext);
APP_WORD    OsReleaseMutex  (APP_HANDLE hMutext);

APP_HANDLE  OsCreateEvent              (APP_BOOL ManualReset,APP_BOOL InitialState,APP_LPCHAR szName);
APP_VOID    OsDestroyCriticalSection   (APP_LPHANDLE lphSection);
APP_WORD    OsCloseEvent               (APP_HANDLE hEvent);
APP_WORD    OsSetEvent                 (APP_HANDLE hEvent);
APP_WORD    OsResetEvent               (APP_HANDLE hEvent);
APP_DWORD   OsWaitForEvent             (APP_HANDLE hEvent,APP_DWORD dwDelay);
APP_WORD    OsWaitForMutex             (APP_HANDLE hEvent,APP_DWORD dwDelay);
APP_VOID    OsLeaveCriticalSection     (APP_LPHANDLE lphSection);
APP_VOID    OsInitializeCriticalSection(APP_LPHANDLE lphSection);
APP_VOID    OsEnterCriticalSection     (APP_LPHANDLE lphSection);

void * OsCreateThread (APP_WORD (*TaskEntry)(APP_LPVOID pArg), APP_DWORD  TaskPrio, APP_LPVOID pArg, APP_DWORD  * pThreadId, APP_LPCHAR szTaskName );

APP_WORD OsResumeThread (APP_HANDLE hThread);
APP_VOID OsDestroyThread(APP_HANDLE hThread);

typedef struct
{
  APP_VOID      (*callback_timeout) (APP_VOID);
  APP_HANDLE    hTimer;
  APP_DWORD     dwThread;
  APP_DWORD     dwTimerID;
  APP_BOOL      bRunning;
} ST_TIMER_PROPERTIES,APP_FAR *LPST_TIMER_PROPERTIES;

#define _DEBUG_MAX_ALLOC_ 1

#endif
