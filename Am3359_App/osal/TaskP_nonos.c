/**
 * \file   TaskP_nonos.c
 *
 * \brief  This file implements key TaskP APIs and a round-robin
 *         scheduler for nonos (bare metal) applications.
 */

/*
 * Copyright (C) 2018 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifdef BARE_METAL

#include <stdio.h>
#include <string.h>

#include <ti/osal/osal.h>
#include <ti/osal/src/nonos/Nonos_config.h>
#include <osal/TaskP.h>

#define TASK_STATE_READY     (1U)
#define TASK_STATE_WAITING   (2U)
#define TASK_MAX_NUM         (16U)

/* nonos task structure used to keep track of task state and params */
typedef struct TaskP_task_s
{
    void *taskfxn;
    TaskP_Params *params;
    uint8_t state;
} TaskP_task;

/* List of all tasks created by TaskP_create. */
static TaskP_task task_list[TASK_MAX_NUM];
static int task_create_idx = -1;
static bool run_scheduler;

TaskP_Handle TaskP_create(void *taskfxn, const TaskP_Params *params)
{
    task_create_idx++;

    if(task_create_idx >= TASK_MAX_NUM)
    {
        /* Can't create any more tasks */
        return NULL;
    }

    TaskP_task new_task;

    new_task.taskfxn = taskfxn;
    new_task.params = params;
    new_task.state = TASK_STATE_READY;

    task_list[task_create_idx] = new_task;

    /* Handle for nonos tasks the task structure itself */
    return (TaskP_Handle)&task_list[task_create_idx];
}

void TaskP_Params_init(TaskP_Params *params)
{
    params->priority = 0;   /* round robin scheduling so no priority */
    params->stacksize = 0;  /* stacksize param not used for nonos tasks */
    params->arg0 = NULL;
    params->arg1 = NULL;
    params->name = NULL;
}

void TaskP_sleep(uint32_t timeout)
{
    /* TaskP_sleep is not currently implemented for nonos
     * (bare metal) applications. Simply return back to the
     * task when sleep is called.
     */
    return;
}

void run_task(void (*f)(void *, void *), void *arg1, void *arg2)
{
    (*f)(arg1, arg2);
}

void scheduler_nonos_start()
{
    int cur_task_idx = 0;
    run_scheduler = TRUE;
    TaskP_task cur_task;

    /* Round-robin loop through all tasks created by TaskP_create */
    while(run_scheduler)
    {
        cur_task = task_list[cur_task_idx];

        if(cur_task.state == TASK_STATE_READY)
        {
            run_task(cur_task.taskfxn, cur_task.params->arg0, cur_task.params->arg1);
        }

        cur_task_idx++;

        if(cur_task_idx > task_create_idx)
        {
            cur_task_idx = 0;
        }
    }
}

void scheduler_nonos_exit()
{
    run_scheduler = FALSE;
}

#endif //BARE_METAL
