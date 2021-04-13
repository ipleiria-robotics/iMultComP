/*
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** ============================================================================
 *  @file       TaskP.h
 *
 *  @brief      Task module for the RTOS Porting Interface
 *
 *
 *  ============================================================================
 */

#ifndef ti_osal_TaskP__include
#define ti_osal_TaskP__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*!
 *  @brief    Status codes for TaskP APIs
 */
typedef enum TaskP_Status_e
{
    /*! API completed successfully */
    TaskP_OK         = 0,
    /*! API failed */
    TaskP_FAILURE    = (-(int32_t)1)
} TaskP_Status;

/*!
 *  @brief    Number Ticks by OS in 1 millisecond
 */
#define OS_TICKS_IN_MILLI_SEC   1 /* 1000us tick */

/*!
 *  @brief    Opaque client reference to an instance of a TaskP
 *
 *  A TaskP_Handle returned from the ::TaskP_create represents that
 *  instance and  is used in the other instance based functions
 */
typedef  void *TaskP_Handle;

/*!
 *  @brief    Basic SemaphoreP Parameters
 *
 *  Structure that contains the parameters are passed into ::TaskP_create
 *  when creating a TaskP instance.
 */
typedef struct TaskP_Params_s
{
    uint8_t *name;           /*!< Name of the task instance.                  */
    void *pErrBlk; /*!< Pointer to the error block for task Create */
    int8_t priority;     /*!< The priority of the task                    */
    uint32_t stacksize;   /*!< The stack size of the task                  */
    void *arg0;           /*!< arg0                                        */
    void *arg1;           /*!< arg1                                        */
    void *stack;       /*!< pointer to stack memory, if NULL OS will allocate internally */
} TaskP_Params;

/*!
 *  @brief  Function to create a task.
 *
 *  @param  taskfxn  Function pointer of the task.
 *
 *  @param  params  Pointer to the instance configuration parameters.
 *
 *  @return A TaskP_Handle on success or a NULL on an error
 */
extern TaskP_Handle TaskP_create(void *taskfxn,
                                 const TaskP_Params *params);

/*!
 *  @brief  Function to delete a task.
 *
 *  @param  handle  A TaskP_Handle returned from TaskP_create
 *
 *  @return Status of the functions
 *    - TaskP__OK: Deleted the semaphore instance
 *    - TaskP_FAILURE: Failed to delete the semaphore instance
 */
extern TaskP_Status TaskP_delete(TaskP_Handle *handle);

/*!
 *  @brief  Initialize params structure to default values.
 *
 *  @param params  Pointer to the instance configuration parameters.
 */
extern void TaskP_Params_init(TaskP_Params *params);

/*!
 *  @brief  Function for Task sleep in units of OS tick
 *
 *  @param timeout  sleep ticks.
 */
extern void TaskP_sleep(uint32_t timeout);


/*!
 *  @brief  Function for Task sleep in units of msecs
 *
 *  @param timeoutInMsecs  sleep in units of msecs.
 */
extern void TaskP_sleepInMsecs(uint32_t timeoutInMsecs);

/*!
 *  @brief  Function for Task sleep.
 *
 *  @param handle  A TaskP_Handle returned from TaskP_create.
 *  @param priority  New priority to be set.
 */
extern void TaskP_setPrio(TaskP_Handle handle, uint32_t priority);

/*!
 *  @brief  Function returns the Task handle of current task
 *
 */
extern TaskP_Handle TaskP_self(void);

/*!
 *  @brief  Function returns the Task handle of current task.This is an inline function
 *
 */
extern TaskP_Handle TaskP_selfmacro(void);

/*!
 *  @brief  Function Yield processor to equal priority task
 *
 */
extern void TaskP_yield(void);

/*!
 *  @brief  Check if task is terminated.
 *
 * Typically a task MUST be terminated before it can be deleted.
 *
 *  @return 0: task is not terminated, 1: task is terminated
 */
extern uint32_t TaskP_isTerminated(TaskP_Handle handle);

#ifdef __cplusplus
}
#endif

#endif /* ti_osal_TaskP__include */
