/*
 * @file    profi_timer.c
 * @brief   Implements the timer function used for timertick which is used
 *          by the internal counters used by profibus stack
 *          default implementation is using TimerTask in main.c, however DMTimer3
 *          can also be used by uncommenting the appropriate sections.
 */
/*
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
/* Application header files */
#include "profi_timer.h"

/* Stack header files */
#include "profibus_timer.h"

/* SysBios header files */
#include <ti/sysbios/knl/Clock.h>

/****************************************************************************
 *                      FUNCTION DEFINITIONS
 ****************************************************************************/
void profi_timer_isr(void);
volatile profibus_hwutils_function_handler callback_timerfn = 0;

/****************************************************************************/
/*                      GLOBAL VARIABLES                                    */
/****************************************************************************/
/* Variable to hold millisecond count since start of system */
static volatile unsigned int long tickCounter = 0;

/**
 *  @brief This timer isr to be used if hardware timer is used
 *                  for tickcounter. Default is Timer_task in main.c which gives
 *                  5ms tick using Task_sleep()
 *                  Uncomment DMTimerIntStatusClear if hardware timer is used for tickcounter.
 *                  Comment the Timer_task in main.c
 */
void profi_timer_isr(void)
{
    if(callback_timerfn)
    {
        callback_timerfn(0);
    }
}

/**
 *  @brief   registers the timer function callback
 *  @param   ms    Timer count value in ms
 *  @param   timerfn    Timer ISR function
 */
void profi_timer_register_call_back(unsigned int ms,
                                    profibus_hwutils_function_handler timerfn)
{
    callback_timerfn = timerfn;
}
