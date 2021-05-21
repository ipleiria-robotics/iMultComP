/**
 * \file eip_ndk.h
 * \brief Header file for ndk related functionalities
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
#ifndef EIP_NDK_H_
#define EIP_NDK_H_

#include <ti/ndk/inc/netmain.h>
#include <ti/ndk/inc/stkmain.h>
#include <ti/ndk/inc/usertype.h>
#include <Include/protocol/eip_types.h>


#define SIZE_OF_EIP_FLASH_DATA   256
/**
 *  @brief  Led Task Definition
 *
 *  @param  arg0
 *  @param  arg1
 *
 *  @retval none
 *
 */
void EIPUTILS_ledFxn(UArg arg0, UArg arg1);

/**
 *  @brief  The function changes the endianness of the IP Address
 *
 *  @param   dwIPAddress [in] IP address pointer
 *
 *  @retval  none
 *
 */
EIP_VOID EIPUTILS_changeIPEndianness(EIP_DWORD *dwIPAddress);

/**
 *  @brief  The function receives IP Address from User and assign it to the device.
 *
 *  @param   userIPAddress [in] IP address pointer
 *
 *  @retval  1 - If success
 *           0 - If failed
 *
 */
EIP_BOOL EIPUTILS_assignUserIP();
/**
 *  @brief  The function returns the IP addreses assigned
 *
 *  @param   ipAddr [out] IP address pointer
 *
 *  @retval EIP_TRUE  - If success
 *          EIP_FALSE - If failed
 *
 */
EIP_BOOL EIPUTILS_getCurrentIP(EIP_CHAR *ipAddr);
/**
 *  @brief  The function is used to validate an IP adddress.
 *
 *  @param   ip_str [in] IP address pointer
 *
 *  @retval EIP_TRUE  - If valid IP address
 *          EIP_FALSE - If invalid IP address
 *
 */
int EIPUTILS_isValidIP(const char *ip_str);
/**
 *  @brief  Function displays Assigned macid in serial console
 *
 *  @param  none
 *
 *  @retval none
 *
 */
void EIPUTILS_displayMacid();
/**
 *  @brief  Function displays Application status and details on startup
 *
 *  @param  none
 *
 *  @retval none
 *
 */
void EIPUTILS_displayApplicationDetails();
/**
 *  @brief  Function displays Assigned IP address in serial console
 *
 *  @param  none
 *
 *  @retval none
 *
 */
void EIPUTILS_displayIPDetails();

#endif /* EIP_NDK_H_ */
