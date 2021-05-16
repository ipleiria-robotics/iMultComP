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
/*  Component           : Thread manager                                     */
/* $Workfile:: stk_thd.h                                                    $*/
/* $Revision:: 8                                                            $*/
/* $Date:: 12/09/14 16:53                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*     abstraction layer for thread                                          */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef __STK_THD_H__
#define __STK_THD_H__

#include "pool.h"

/*-----------------------------------------------------------------------*/
/* system                                                                */
/*-----------------------------------------------------------------------*/

#define MAXNUM_OF_TASKS_SYSTEM       14               /* Number of task or thread (Thread 0 / Queue 0 = Error Queue)*/
#define MAXNUM_OF_QUEUE_SYSTEM       ID_QUEUE_MAX     /* Normaly 1 Queue = 1 Task or Thread but to be able in the future to cut some Task/Thread. Some QueueID could redirect to the same Thread */
#define MAXNUM_OF_TASK_APPLICATE    (5+MAX_DEVICE)    /* Normaly 1 Queue = 1 Task or Thread but to be able in the future to cut some Task/Thread. Some QueueID could redirect to the same Thread */

#define MAXNUM_OF_QUEUE (MAXNUM_OF_QUEUE_SYSTEM + MAXNUM_OF_TASK_APPLICATE)
#define MAXNUM_OF_TASKS (MAXNUM_OF_TASKS_SYSTEM + MAXNUM_OF_TASK_APPLICATE)

#define TASK_BLOCKED    0
#define TASK_ENABLED    1
#define TASK_OPERATING  2

#define INVALID_TASK_ID 0

#define MAX_TASK_NAME_SIZE 32
typedef struct _task_prop
{
  APP_DWORD  volatile dwState;                         /* Task state (BLOCKED/ENABLED/OPERATING)*/
  ST_POOL             stPool;
  APP_HANDLE          hSection;                        /* Use to protect queue*/
  APP_HANDLE          hEvent;                          /* Use to  block read in queue if no message*/
  APP_DWORD           dwIdThread;
  APP_HANDLE          hThread;
  APP_CHAR            szTaskName[MAX_TASK_NAME_SIZE];  /* Nom du thread*/
  APP_DWORD           dwLastTime;
  APP_DWORD           dwCycle;
  APP_DWORD           dwCycleMin;
  APP_DWORD           dwCycleMax;
  APP_DWORD           dwNbMsg;
  APP_BOOL            bOccupied;                       /* valid, if value = 1, 0: invalid*/
}ST_TASK_PROPERTIES;

APP_DWORD   StackGetThreadId                    (APP_VOID);
APP_WORD    StackDestroyThread                  (APP_DWORD TaskId);
APP_VOID    StackInitializeCriticalSectionThread(APP_LONG lTaskId);
APP_VOID    StackDestroyCriticalSectionThread   (APP_LONG lTaskId);
APP_VOID    StackEnterCriticalSectionThread     (APP_LONG lTaskId);
APP_VOID    StackLeaveCriticalSectionThread     (APP_LONG lTaskId);
APP_BOOL    CreateEventThread                   (APP_LONG lTaskId,APP_BOOL ManualReset,APP_BOOL InitialState,APP_LPCHAR szName);
APP_DWORD   CloseEventThread                    (APP_LONG lTaskId);
APP_BOOL    SetEventThread                      (APP_LONG lTaskId);
APP_BOOL    ResetEventThread                    (APP_LONG lTaskId);
APP_DWORD   StackGetTaskState                   (APP_LONG lTaskId);
APP_VOID    StackSetTaskState                   (APP_LONG lTaskId, APP_DWORD dwState);
LPST_POOL   GetQueueThread                      (APP_LONG lTaskId);
APP_HANDLE  GetEventThread                      (APP_LONG lTaskId);
APP_DWORD   WaitForEventThread                  (APP_LONG lTaskId,APP_DWORD dwDelay);

#ifdef OS_DEBUG_MSG
APP_VOID StackIncNbMsgThread  (APP_DWORD dwThreadId);
APP_VOID StackDecNbMsgThread  (APP_DWORD dwThreadId);
APP_VOID StackResetNbMsgThread(APP_DWORD dwThreadId);
#endif

APP_VOID Monitor_Init_Task_cycle (APP_LONG lTaskId);
APP_VOID Monitor_Set_Task_cycle  (APP_LONG lTaskId);
APP_BOOL StackIsFreeThread       (APP_LONG lTaskId);
APP_BOOL StackThreadIsDisable    (APP_LONG lTaskId);
APP_VOID StackSetOccupiedThread  (APP_LONG lTaskId,APP_BOOL bState);
APP_WORD StackStartThread        (APP_DWORD dwThreadId);
APP_WORD StackCreateThread       (APP_WORD (*TaskEntry)(APP_LPVOID pArg), APP_DWORD TaskPrio, APP_LPVOID pArg, APP_DWORD* pTaskId, APP_LPCHAR szTaskName);
APP_WORD StackWaitOnEnableThread (APP_VOID);
APP_WORD StackWaitOnDisableThread(APP_VOID);
APP_VOID StackInitManagmentThread(APP_VOID);
#endif
