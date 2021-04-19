/**
 *   @file  icss_emac_osal.c
 *
 *   @brief
 *      This is the OS abstraction layer and is used by the Uart, The example application
 *      works on multiple cores.
 *
 *
 * Copyright (c) 2015, Texas Instruments Incorporated
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
 **/
#ifndef COMMON_OSAL_PROTOTYPE
#define COMMON_OSAL_PROTOTYPE

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/timers/dmtimer/Timer.h>


#include <Include/Board_am335x/test_common_utils.h>
#include <ti/drv/icss_emac/icss_emac_ver.h>
/**********************************************************************
 ************************** Local Definitions *************************
 **********************************************************************/


/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/

/**********************************************************************
 **************************** OSAL Functions **************************
 **********************************************************************/
#if ICSS_EMAC_DRV_VERSION_ID ==(0x01000002)
void TimerP_params_init(TimerP_Params *params)
{
    params->freqHi = 0;
    params->freqLo = ICSS_EMAC_TEST_TIMER_FREQUENCY;
    params->name = 0;
    params->runMode = Timer_RunMode_CONTINUOUS;
    params->startMode = Timer_StartMode_USER;
    params->periodType = Timer_PeriodType_MICROSECS;
    params->period = ICSS_EMAC_TEST_TIMER_PERIOD;
}





void *TimerP_create(int32_t id,
                    TimerP_Fxn tickFxn,
                    TimerP_Params *params)
{
    Error_Block eb;
    Timer_Handle timerHandle;
    Timer_Params timerParams;

    Error_init(&eb);
    Timer_Params_init(&timerParams);

    timerParams.arg = (xdc_UArg) params->arg;
    timerParams.extFreq.hi  =     params->freqHi;
    timerParams.extFreq.lo   =    params->freqLo;
    timerParams.runMode = (ti_sysbios_interfaces_ITimer_RunMode)params->runMode;
    timerParams.periodType = (ti_sysbios_interfaces_ITimer_PeriodType)
                             params->periodType;
    timerParams.startMode = (ti_sysbios_interfaces_ITimer_StartMode)
                            params->startMode;
    timerParams.period = params->period;

    timerHandle = Timer_create(id, tickFxn, &timerParams, &eb);
    return (Timer_Handle)timerHandle;
}


void TimerP_start(TimerP_Handle timerHandle)
{
    Timer_start(timerHandle);
}



#endif
#endif
