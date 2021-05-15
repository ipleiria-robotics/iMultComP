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
/**
* @file    profi_timer.h
* @brief   TIMER support functions for profibus
*
*/
#ifndef PROFI_TIMER_H
#define PROFI_TIMER_H

#include "profi_hw_utils.h"

/*!
 *  @brief  This timer isr to be used if hardware timer is used
 *           for tick counter
 *
 */
void profi_timer_isr(void);

/*!
 *  @brief  This timer ISR to be used if hardware timer is used
 *           for tick counter
 *
 */

/*!
*  @brief  create the Timer
*
*/
void profi_timer_init(void);

/*!
 *  @brief  enables the timer
 *
 *  @param  periodinMS periods in mili second.
 *
 */
void profi_timer_enable(unsigned int periodinMS);

/*!
 *  @brief  stop the timer
 *
 */
void profi_timer_stop(void);

/*!
 *  @brief  gives the times tick count value.
 *
 *  @return Tick count value
 */
unsigned int long prof_get_ms_tick();

/*!
 *  @brief  registers the timer function callback.
 *
 *  @param  ms - timer period value in milli sec
 *
  *  @param  timerfn  call back function
 */
void profi_timer_register_call_back(unsigned int ms,
                                    profibus_hwutils_function_handler timerfn);

#endif

