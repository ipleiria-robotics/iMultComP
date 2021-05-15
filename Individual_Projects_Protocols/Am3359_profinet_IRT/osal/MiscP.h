/*
 * Copyright (c) 2017, Texas Instruments Incorporated
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
 */
/** ============================================================================
 *  @file       MiscP.h
 *
 *  @brief      Miscellaneous functions for the RTOS Porting Interface
 *
 *
 *  ============================================================================
 */

#ifndef ti_osal_MiscP__include
#define ti_osal_MiscP__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define Uint32      uint32_t
#define Uint16      uint16_t
#define Uint8       uint8_t


/*!
 *  @brief    64-bit Frequency value structure
 *
 */
typedef struct TypesP_FreqHz_s
{
    uint32_t hi;
    // most significant 32-bits of frequency
    uint32_t lo;
    // least significant 32-bits of frequency
} TypesP_FreqHz;


/*!
 *  @brief  Function to get system timestamp
 *
 *  @return 32-bit system cycle counter value
 */
extern uint32_t OSAL_Timestamp_get32(void);

/*!
 *  @brief  Function to get CPU clock frequency
 *
 *  @param  freq  Pointer to the struct where frequency is to be stored
 */
extern void OSAL_getCpuFreq(TypesP_FreqHz *freq);

#ifdef __cplusplus
}
#endif

#endif /* ti_osal_MiscP__include */
