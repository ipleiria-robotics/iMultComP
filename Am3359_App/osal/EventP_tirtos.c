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
/*
 *  ======== EventP_tirtos.c ========
 */
#include <ti/sysbios/knl/Event.h>
#include "EventP.h"
#include <xdc/runtime/Memory.h>

/*
 *  ======== EventP_create ========
 */
EventP_Handle EventP_create(EventP_Params *params)
{
    Event_Handle eventHandle;
    Event_Params eventParams;

    Event_Params_init(&eventParams);

    eventHandle = Event_create(&eventParams, NULL);

    return ((EventP_Handle)eventHandle);
}

/*
 *  ======== EventP_delete ========
 */
void EventP_delete(EventP_Handle *handle)
{
    Event_delete((Event_Handle *)handle);
}

/*
 *  ======== EventP_Params_init ========
 */
void EventP_Params_init(EventP_Params *params)
{
    Event_Params eventParams;
    Event_Params_init(&eventParams);
}
/*
 *  ======== EventP_pend ========
 */
uint32_t EventP_pend(EventP_Handle handle, uint32_t andMask, uint32_t orMask,
                     uint32_t timeout)
{
    return (Event_pend(handle, andMask, orMask, timeout));
}
/*
 *  ======== EventP_post ========
 */
void EventP_post(EventP_Handle handle, uint32_t eventMask)
{
    Event_post(handle, eventMask);
}
/*
 *  ======== EventP_getPostedEvents ========
 */
uint32_t EventP_getPostedEvents(EventP_Handle handle)
{
    return Event_getPostedEvents(handle);
}
