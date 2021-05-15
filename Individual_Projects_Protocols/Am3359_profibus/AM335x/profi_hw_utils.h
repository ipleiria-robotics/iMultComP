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
 * @file    profi_hw_utils.h
 * @brief   Initialization and usage of peripherals used
 *          for profibus like UART, ECAP, and Timer
 *
 */
#ifndef PROFI_HW_UTILS_H
#define PROFI_HW_UTILS_H
#include <ti/drv/pruss/pruicss.h>
#include <ti/csl/hw_types.h>
#include <ti/drv/pruss/pruicss.h>
#include <ti/drv/pruss/soc/pruicss_v1.h>

typedef void *(*profibus_hwutils_function_handler)(void *);

/** @addtogroup PB_ARM_APPLICATION_UTILITY
 @{ */


/**
 * @brief calls functions that registers the timer callback function.
 *
 *  @param   period    Time period of timer
 *
 *  @param   timerfn    Call back function
 *  @retval  0 on success.
 */
int profibus_hwutils_systemtimer_init(unsigned int period,
                                      profibus_hwutils_function_handler timerfn);

/**
 * @brief wrapper function that calls ecap init.
 *
 * @param   pruIcssHandle    PRUICSS handle
 * @retval  0 on success.
 */
int profibus_hwutils_timer_init(PRUICSS_Handle pruIcssHandle);

/**
 * @brief wrapper function for Clock_getTicks which is a Sysbios function
 *                  that gives the tickcount in millisecond.
 *
 * @retval  tick count.
 *
 */
unsigned int long  GetTickCount(void);


/**
 * @brief sets the baudrate on UART.
 *
 * @param   baudrate  Required baud rate to be configured.
 * @retval  0 on success,-1 on failure.
 *
 */
int bsp_uart_init(unsigned int baudrate);


/**
 *
 * @brief Cross bar interrupt configuration
 *        TX and RX pin muxing
 *        TX enable pin muxing.
 *
 *
 */
void Board_Profibus_hw_init(void);

/**
 *
 * @brief I2c creation with frequency of 400kHz
 *
 * @retval  I2c handle on success,-1 on failure.
 *
 */
void *Board_I2c_Init(void);
/**
@}
*/
#endif

