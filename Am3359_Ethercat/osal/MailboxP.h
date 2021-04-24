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
 *  @file       MailboxP.h
 *
 *  @brief      Mailbox module for the RTOS Porting Interface
 *
 *
 *  ============================================================================
 */

#ifndef ti_osal_MailboxP__include
#define ti_osal_MailboxP__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*!
 *  @brief    Status codes for SemaphoreP APIs
 */
typedef enum MailboxP_Status_e
{
    /*! API completed successfully */
    MailboxP_OK         = 0,
    /*! API failed */
    MailboxP_FAILURE    = (-(int32_t)1)
} MailboxP_Status;



/*!
 *  @brief    Mailbox Parameters
 *
 */
typedef struct MailboxP_Params_s
{
    uint8_t *name;           /*!< Name of the mailbox instance */
} MailboxP_Params;

/*!
 *  @brief    Wait forever define
 */
#define MailboxP_WAIT_FOREVER (~((uint32_t)0U))

/*!
 *  @brief    No wait define
 */
#define MailboxP_NO_WAIT       ((uint32_t)0U)
/*!
 *  @brief    Opaque client reference to an instance of a MailboxP
 *
 *   MailboxP_Handle
 */
typedef  void *MailboxP_Handle;


/*!
 *  @brief  Function to create a Mailbox.
 *
 *  @param  msgsize  Size of the mailbox message.
 *
 *  @param  num  number of messages.
 *
 *  @param  params  Pointer to the instance configuration parameters.
 *
 *  @return A MailboxP_Handle on success or a NULL on an error
 */
extern MailboxP_Handle MailboxP_create(uint32_t msgsize, uint32_t num,
                                       MailboxP_Params *params);

/*!
 *  @brief  Function to delete a Mailbox.
 *
 *  @param  handle  MailboxP_Handle
 *
 *  @return Status of the functions
 *    - MailboxP_OK: Deleted the Mailbox instance
 *    - MailboxP_FAILURE: Failed to delete the Mailbox instance
 */
extern MailboxP_Status MailboxP_delete(MailboxP_Handle *handle);

/*!
 *  @brief  Initialize params structure to default values.
 *
 *  @param params  Pointer to the instance configuration parameters.
 */
extern void MailboxP_Params_init(MailboxP_Params *params);

/*!
 *  @brief  Function to pend (wait) on a Mailbox.
 *
 *  @param  handle  MailboxP_Handle
 *  @param  msg     message buffer
 *  @param  timeout Timeout (in milliseconds) to wait for the mailbox to
 *                  be posted (signalled).
 *
 *  @return Status of the functions
 *    - MailboxP_OK: Obtain the mailbox
 *    - MailboxP_FAILURE: Failed to obtain mailbox.
 */
extern MailboxP_Status MailboxP_pend(MailboxP_Handle handle, void *msg,
                                     uint32_t timeout);

/*!
 *  @brief  Function to post (signal) a mailbox.
 *
 *  @param  handle  MailboxP_Handle
 *  @param  msg     message buffer
 *  @param  timeout Timeout (in milliseconds)
 *
 *  @return Status of the functions
 *    - MailboxP_OK: Released the mailbox
 *    - MailboxP_FAILURE: Failed to post the mailbox
 */
extern MailboxP_Status MailboxP_post(MailboxP_Handle handle, void *msg,
                                     uint32_t timeout);


#ifdef __cplusplus
}
#endif

#endif /* ti_osal_MailboxP__include */
