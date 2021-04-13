/**
 * board_platform.h
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
#ifndef BOARD_PLATFORM_H_
#define BOARD_PLATFORM_H_
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdio.h>
//#include <ti/starterware/include/interrupt.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 * \brief    Initializes the platform and return platform specific values from Starterware board data
 *
 * \details  Detects if the DevID-instance number exists in the devices list of board
 *           Gets relevant data(Eg: GPIO Instance no, pin no., base address etc) and returns the same
 *           PRCM of the module enabled
 *
 * \param    moduleId[IN]       Module ID of the device to add  Eg: CHIPDB_MOD_ID_I2C
 * \param    devId[IN]          Device ID of the device         Eg: DEVICE_ID_I2C_LED
 * \param    instNum[IN]        Instance number of the device
 * \param    *devInstNum[OUT]   Instance number of the SoC module to which the device is connected for controlling it.
 * \param    *devCtrlInfo[OUT]  Control information
 * \param    *instAddr[OUT]     Base address of the device
 *
 * \retVal   S_PASS        initialization successful.
 * \retVal   E_FAIL        initialization failed.
 */
uint32_t Board_initPlatform(uint32_t moduleId, uint32_t devId,
                            uint32_t instNum, uint32_t *devInstNum, uint32_t *devCtrlInfo,
                            uint32_t *instAddr);
#endif /* BOARD_PLATFORM_H_ */
