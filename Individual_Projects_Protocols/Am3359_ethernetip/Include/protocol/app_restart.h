/**
 * app_restart.h
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
#ifndef ETHERNETIP_ADAPTER_APP_RESTART_H_
#define ETHERNETIP_ADAPTER_APP_RESTART_H_

#define INTERFACE_RESTART_CHECK_ADDRESS  0x4030FFF4
#define INTERFACE_CONFIG_ADDRESS  0x4030FFEC
/**
 *  @brief  The function restarts the application
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
void EIPAPP_restartApplication();
/**
 *  @brief  Exit handler for the application.
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
void EIPAPP_exitHandler(int var);

#if defined(SOC_AM572x) || defined(SOC_AM571x)

/**
 *  @brief  Save IO delay to DDR. This is done to restore it before restart
 *
 *          The application during startup expects the IO delay data and code section to be placed
 *          in the begining of OCMC ram. During ICSS Switch operation this will be overwritten
 *          So this is stored atr a safe place
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
void EIPAPP_saveIODelaySection();
/**
 *  @brief  Restores the IO delay section saved.This is done before applciation restart
 *
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
void EIPAPP_restoreIODelaySection();
#endif /*defined(SOC_AM572x) || defined(SOC_AM571x)*/

#endif /* ETHERNETIP_ADAPTER_APP_RESTART_H_ */
