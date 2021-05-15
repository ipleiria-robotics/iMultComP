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
 *  @file       ClockP.h
 *
 *  @brief      Clock module for the RTOS Porting Interface
 *
 *
 *  ============================================================================
 */

#ifndef ti_osal_ClockP__include
#define ti_osal_ClockP__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
/*!
 *  @brief    Status codes for ClockP APIs
 */
typedef enum ClockP_Status_e
{
    /*! API completed successfully */
    ClockP_OK         = 0,
    /*! API failed */
    ClockP_FAILURE    = (-(int32_t)1)
} ClockP_Status;


/*!
 *  @brief    Opaque client reference to an instance of a SemaphoreP
 *
 *   ClockP_Handle
 */
typedef  void *ClockP_Handle;


/*!
 *  @brief    Basic ClockP Parameters
 *
 */
typedef struct ClockP_Params_s
{
    uint8_t *name;
    void *arg;
    uint32_t period;
    uint8_t startflag;
} ClockP_Params;

/*!
 *  @brief  Function to create a Clock.
 *
 *  @param  clkfxn  Clock function
 *  @param  timeout  Clock time out,
 *  @param   params  Pointer to the instance configuration parameters.
 *
 *  @return A ClockP_Handle on success or a NULL on an error
 */
extern ClockP_Handle ClockP_create(void *clkfxn, uint32_t timeout,
                                   ClockP_Params *params);

/*!
 *  @brief  Function to delete a Clock.
 *
 *  @param  handle  Clock handle
 *
 *  @return Status of the functions
 *    - ClockP_OK: Deleted the Clock instance
 *    - ClockP_FAILURE: Failed to delete the Clock instance
 */
extern ClockP_Status ClockP_delete(ClockP_Handle *handle);

/*!
 *  @brief  Initialize params structure to default values.
 *
 *  @param params  Pointer to the instance configuration parameters.
 */
extern void ClockP_Params_init(ClockP_Params *params);

/*!
 *  @brief  Function to start a Clock.
 *
 *  @param  handle  Clock handle
 */
extern void ClockP_start(ClockP_Handle handle);

/*!
 *  @brief  Function to stop a Clock.
 *
 *  @param  handle  Clock handle
 */
extern void ClockP_stop(ClockP_Handle);


/*!
 *  @brief  Get the configure tick period.
 *
 *  @param none
 */
extern uint32_t ClockP_getTickPeriod();

extern uint32_t ClockP_getTicks();

#ifdef __cplusplus
}
#endif

#endif /* ti_osal_ClockP__include */
