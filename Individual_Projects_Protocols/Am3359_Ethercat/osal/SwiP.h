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
 *  @file       SwiP.h
 *
 *  @brief      Software interrupt module for the RTOS Porting Interface
 *
 *
 *  ============================================================================
 */

#ifndef ti_osal_SwiP__include
#define ti_osal_SwiP__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*!
 *  @brief    Status codes for SwiP APIs
 */
typedef enum SwiP_Status_e
{
    /*! API completed successfully */
    SwiP_OK         = 0,
    /*! API failed */
    SwiP_FAILURE    = (-(int32_t)1)
} SwiP_Status;


/*!
 *  @brief    Opaque client reference to an instance of a SwiP
 *
 *  A SwiP_Handle returned from the SwiP_create
 */
typedef  void *SwiP_Handle;


/*!
 *  @brief    Swi Parameters
 *
 */
typedef struct SwiP_Params_s
{
    uint8_t *name;           /*!< Name of the Swi instance */
    uint8_t priority;        /*!< Priority of Swi */
    uint32_t trigger;        /*!< Trigger type of Swi */
    void *arg0;               /*!< arg0 */
    void *arg1;               /*!< arg1 */
} SwiP_Params;

/*!
 *  @brief  Function to create a SWI.
 *
 *  @param  swifxn  Swi function
 *
 *  @param  params  Pointer to the instance configuration parameters.
 *
 *  @return A SwiP_Handle on success or a NULL on an error
 */
extern SwiP_Handle SwiP_create(void *swifxn, SwiP_Params *params);

/*!
 *  @brief  Function to delete a SWI.
 *
 *  @param  handle  SwiP_Handle
 *
 *  @return Status of the functions
 *    - SwiP_OK: Deleted the swi instance
 *    - SwiP_FAILURE: Failed to delete the swi instance
 */
extern SwiP_Status SwiP_delete(SwiP_Handle *handle);

/*!
 *  @brief  Initialize params structure to default values.
 *
 *
 *  @param params  Pointer to the instance configuration parameters.
 */
extern void SwiP_Params_init(SwiP_Params *params);

/*!
 *  @brief  Function to post SWI.
 *
 *
 *  @param handle  SwiP_Handle
 */
extern void SwiP_post(SwiP_Handle handle);


#ifdef __cplusplus
}
#endif

#endif /* ti_osal_SwiP__include */
