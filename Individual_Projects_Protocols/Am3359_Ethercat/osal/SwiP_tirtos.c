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
 *  ======== SwiP_tirtos.c ========
 */
#include <ti/sysbios/knl/Swi.h>
#include "SwiP.h"
#include <xdc/runtime/Memory.h>
/*
 *  ======== SwiP_create ========
 */
SwiP_Handle SwiP_create(void *swifxn, SwiP_Params *params)
{
    Swi_Handle swiHandle;
    Swi_Params swiParams;

    Swi_Params_init(&swiParams);

    if(params != NULL)
    {
        swiParams.arg0 = (UArg)(params->arg0);
        swiParams.arg1 = (UArg)(params->arg1);
        swiParams.priority = params->priority;
        swiParams.trigger = params->trigger;
        swiParams.instance->name = (xdc_String)params->name;
    }

    swiHandle = Swi_create((Swi_FuncPtr)swifxn, &swiParams, NULL);

    return ((SwiP_Handle)swiHandle);
}

/*
 *  ======== SwiP_delete ========
 */
SwiP_Status SwiP_delete(SwiP_Handle *handle)
{
    Swi_delete((Swi_Handle *)handle);
    return SwiP_OK;
}

/*
 *  ======== SwiP_Params_init ========
 */
void SwiP_Params_init(SwiP_Params *params)
{
    Swi_Params swiParams;
    Swi_Params_init(&swiParams);
    params->priority = swiParams.priority;
    params->trigger = swiParams.trigger;
}

/*
 *  ======== SwiP_post ========
 */
void SwiP_post(SwiP_Handle handle)
{
    Swi_post((Swi_Handle)handle);
}
