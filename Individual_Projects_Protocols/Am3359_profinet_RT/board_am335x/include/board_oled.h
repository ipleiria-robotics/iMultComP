/*
 * Copyright (c) 2016, Texas Instruments Incorporated
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
 *
 */

/**
*
* \file    board_oled.h
*
* \brief This contains oled specific structure, typedefs, function prototypes
*
******************************************************************************/
#ifndef _BOARD_OLED_H_
#define _BOARD_OLED_H_

/************************
 * Include Files
 ************************/
#include "oled_drv.h"
#if defined(SOC_K2G)
#include <ti/board/src/iceK2G/include/board_cfg.h>
#elif defined(SOC_AM335x)
#include <ti/board/src/icev2AM335x/include/board_cfg.h>
#endif

/************************
 * Defines and Macros
 ************************/
/** Macros for I2C instance */
#ifdef SOC_AM335x
#define BOARD_OLED_DISPLAY_INSTANCE     (1U)
#endif

/** Macros for I2C slave address*/
#define OLED_SLAVE_ADDR                 (0x3C)

/************************
 * Function declarations
 ************************/
OLED_RET Board_oledGPIOInit(void);


#endif /* _BOARD_OLED_H_ */
