/**
 * @file delay_us.h
 *
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
#ifndef _DELAY_US_H_
#define _DELAY_US_H_

/*-----------------------------------------------------------------------------------------
------
------  Includes
------
-----------------------------------------------------------------------------------------*/
#include <stdint.h>

/*-----------------------------------------------------------------------------------------
------
------  Defines and Types
------
-----------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------
------
------  variables
------
-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------  function prototypes
------
-----------------------------------------------------------------------------------------*/

/**
*  @brief  Delay in micro-seconds using SYSBIOS timers and events
*  @warning This function is currently not thread safe
*  @warning The Maximum delay value should be less than 7 seconds at 600MHz frequency
*
*  @param delay_in_microseconds The delay value in microseconds
*
*/
void delay_us(uint32_t delay_in_microseconds);

#endif /*_DELAY_US_H_*/
