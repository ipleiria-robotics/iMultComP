/**
 *  \file   board_misc.h
 *
 *  \brief:
 *  Board Misc APIs
 *
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
#ifndef _BOARD_MISC_H_
#define _BOARD_MISC_H_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#if defined(SOC_AM571x) || defined(SOC_AM572x)
#include <ti/csl/soc/am571x/src/csl_device_xbar.h>
#endif
/***********************************************************************/
/* Functions                                                           */
/***********************************************************************/
#if defined(SOC_AM571x) || defined(SOC_AM572x)
void Board_xbarMpuIrqConfigure(Uint32 irqNumIdx, CSL_XbarIrq xbarIrq);
#endif
void Board_delay(uint32_t delay);

void Board_getDigInput(uint8_t *switchs);
/**
* @brief Function to Enable PWMSS1Module
*
*
* @param none
*
* @retval none
*/
void Board_enablePWMSS1Module();

/**
* @brief Function to introduce delay of microseconds resolution using HW timer
*
*
* @param delay in micro seconds
*
* @retval none
*/
void delay_us(uint32_t delay_in_microseconds);

#ifdef SOC_AM65XX
void InitMmu(void);
#endif

#endif /* _BOARD_MISC_H_*/
