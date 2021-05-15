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
/*
 *  ======== ClockP_tirtos.c ========
 */

#include <ti/sysbios/knl/Clock.h>
#include "ClockP.h"
#include <xdc/runtime/Memory.h>

/*
 *  ======== ClockP_create ========
 */
ClockP_Handle ClockP_create(void *clkfxn, uint32_t timeout,
                            ClockP_Params *params)
{
    Clock_Handle clkHandle;
    Clock_Params clkParams;

    Clock_Params_init(&clkParams);

    if(params != NULL)
    {
        clkParams.startFlag = params->startflag;
        clkParams.period = params->period;
        clkParams.arg = (UArg)(params->arg);
        clkParams.instance->name = (xdc_String)params->name;
    }

    clkHandle = Clock_create((Clock_FuncPtr)clkfxn, timeout,
                             (Clock_Params *)&clkParams, NULL);

    return ((ClockP_Handle)clkHandle);
}

/*
 *  ======== ClockP_delete ========
 */
ClockP_Status ClockP_delete(ClockP_Handle *handle)
{
    Clock_delete((Clock_Handle *)handle);
    return ClockP_OK;
}

/*
 *  ======== ClockP_Params_init ========
 */
void ClockP_Params_init(ClockP_Params *params)
{
    Clock_Params clkParams;
    Clock_Params_init(&clkParams);
    params->period = clkParams.period;
    params->startflag = clkParams.startFlag;
}
/*
 *  ======== ClockP_getTickPeriod ========
 */
uint32_t ClockP_getTickPeriod()
{
    return Clock_tickPeriod;
}

/*
 *  ======== ClockP_start ========
 */
void ClockP_start(ClockP_Handle handle)
{
    Clock_start(handle);
}

/*
 *  ======== ClockP_stop ========
 */
void ClockP_stop(ClockP_Handle handle)
{
    Clock_stop(handle);
}

/*
 *  ======== ClockP_stop ========
 */
uint32_t ClockP_getTicks()
{
    return Clock_getTicks();
}
