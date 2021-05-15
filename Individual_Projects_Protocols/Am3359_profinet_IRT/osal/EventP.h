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
 *  @file       EventP.h
 *
 *  @brief      Event module for the RTOS Porting Interface
 *
 *
 *  ============================================================================
 */

#ifndef ti_osal_EventP__include
#define ti_osal_EventP__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
/*
 * Pre-defined Event Ids. Event_Ids are provided to simplify the specification
 * of andMasks and orMasks arguments to Event_pend().
 * Since each Event_Id is a bitmask composed of only a single bit,
 * a group of Event_Ids within an andMask or orMask can be indicated by simply adding them together.
 */

/*!
*  @brief    Event ID 0
*/
#define EventP_ID_NONE     0
/*!
 *  @brief    Event ID 0
 */
#define EventP_ID_00        0x00000001
/*!
 *  @brief    Event ID 1
 */
#define EventP_ID_01        0x00000002
/*!
 *  @brief    Event ID 2
 */
#define EventP_ID_02        0x00000004
/*!
 *  @brief    Event ID 3
 */
#define EventP_ID_03        0x00000008
/*!
 *  @brief    Event ID 4
 */
#define EventP_ID_04        0x00000010
/*!
 *  @brief    Event ID 5
 */
#define EventP_ID_05        0x00000020
/*!
 *  @brief    Event ID 6
 */
#define EventP_ID_06        0x00000040
/*!
 *  @brief    Event ID 7
 */
#define EventP_ID_07        0x00000080
/*!
 *  @brief    Event ID 8
 */
#define EventP_ID_08        0x00000100
/*!
 *  @brief    Event ID 9
 */
#define EventP_ID_09        0x00000200
/*!
 *  @brief    Event ID 10
 */
#define EventP_ID_10        0x00000400

/*!
 *  @brief    Status codes for EventP APIs
 */
typedef enum EventP_Status_e
{
    /*! API completed successfully */
    EventP_OK         = 0,
    /*! API failed */
    EventP_FAILURE    = (-(int32_t)1)
} EventP_Status;

/*!
 *  @brief    Wait forever define
 */
#define EventP_WAIT_FOREVER (~((uint32_t)0U))

/*!
 *  @brief    No wait define
 */
#define EventP_NO_WAIT       ((uint32_t)0U)


/*!
 *  @brief    Opaque client reference to an instance of a EventP
 *
 *  A EventP_Handle returned from the ::EventP_create represents that
 *  instance and  is used in the other instance based functions
 */
typedef  void *EventP_Handle;

/*!
 *  @brief    Basic EventP Parameters
 *
 *  Structure that contains the parameters are passed into ::EventP_create
 *  when creating a EventP instance.
 */
typedef struct EventP_Params_s
{
    void *instance;          /*!< Name of the task instance.                  */
} EventP_Params;

/*!
 *  @brief  Function to create an event.
 *
 *  @param  params  Pointer to the instance configuration parameters.
 *
 *  @return A EventP_Handle on success or a NULL on an error
 */
extern EventP_Handle EventP_create(EventP_Params *params);

/*!
 *  @brief  Function to delete an event.
 *
 *  @param  handle  A EventP_Handle returned from EventP_create
 */
extern void EventP_delete(EventP_Handle *handle);

/*!
 *  @brief  Initialize params structure to default values.
 *
 *  @param params  Pointer to the instance configuration parameters.
 */
extern void EventP_Params_init(EventP_Params *params);

/*!
 *  @brief  Function for Event Pend
 *
 *  @param params  Pointer to the instance configuration parameters.
 *  @param andMask return from pend() when ALL of these events have occurred
 *  @param orMask  return from pend() when ANY of these events have occurred
 *  @param timeout return from pend() after this many system time units
 *  @return All consumed events or zero if timeout
 */
extern uint32_t EventP_pend(EventP_Handle handle, uint32_t andMask,
                            uint32_t orMask, uint32_t timeout);

/*!
 *  @brief  Function for Event Post.
 *
 *  @param handle  A EventP_Handle returned from EventP_create.
 *  @param eventMask  mask of eventIds to post (must be non-zero)
 */
extern void EventP_post(EventP_Handle handle, uint32_t eventMask);

/*!
 *  @brief  Function for Event Post.
 *
 *  @param handle  A EventP_Handle returned from EventP_create.
 *  @return All un-consumed events posted to the event
 */
extern uint32_t EventP_getPostedEvents(EventP_Handle handle);

#ifdef __cplusplus
}
#endif

#endif /* ti_osal_EventP__include */
