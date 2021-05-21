/**
 * @file  icss_ptp_driver.h
 *
 * @brief Include file for icss_ptp_driver.c
 *  
 *\par
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 *\par
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
#ifndef ICSS_PTP_DRIVER_H_
#define ICSS_PTP_DRIVER_H_

#include <ti/drv/icss_emac/icss_emacDrv.h>

#include <Include/protocol/timeSync/icss_timeSyncApi.h>

/**
 *  @brief De-Initialize the PTPd stack
 *
 *  @param args ptpHandle Pointer to PTP Handle
 *
 *  @retval None
 *
 */
void ptpDrvStackDeInit(TimeSync_ParamsHandle_t ptpHandle);
/**
 *  @brief Initialize the PTPd stack
 *
 *  @param args ptpHandle Pointer to PTP Handle
 *
 *  @retval None
 *
 */
void ptpDrvStackInit(TimeSync_ParamsHandle_t ptpHandle);
/**
 *  @brief Disables PTP Firmware/Driver
 *
 *  @param None
 *
 *  @retval None
 *
 */
void ptpdDrvDisablePTP();
/**
 *  @brief Enables PTP Firmware/Driver
 *
 *  @param None ptpHandle Pointer to PTP Handle
 *
 *  @retval None
 *
 */
void ptpdDrvEnablePTP();
/**
 *  @brief Initializes the PTP Master ID. Call during a reset.
 *
 *  @param args ptpMac Pointer to Master MAC ID
 *
 *  @retval None
 *
 */
void ptpdDrvRemoveMACID(uint8_t *ptpMac);
/**
 *  @brief Returns the previous Master's MAC ID
 *
 *  @param ptpMac Pointer to previous Master's MAC ID
 *
 *  @retval None
 *
 */
void ptpdDrvGetPrevMACID(uint8_t *ptpMac);
/**
 *  @brief Updates Parent MAC ID with new value
 *
 *  @param ptpMac Pointer to MAC ID of Master. BMC Algorithm calls this
 *
 *  @retval None
 *
 */
void ptpdDrvUpdateParentMACID(uint8_t *ptpMac);
/**
 *  @brief Updates the current time (Seconds and nanoseconds) and distance from PTP Master
 *
 *  @param clockVar Pointer to PTP Stack clock structure
 *
 *  @retval None
 *
 */
void updateCalculatedVals(void *clockVar);
#endif /* ICSS_PTP_DRIVER_H_ */
