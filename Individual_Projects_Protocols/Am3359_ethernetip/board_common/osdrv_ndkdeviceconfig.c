/**
 * @file   osdrv_ndkdeviceconfig.c
 *
 * @brief
 *
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

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <Include/board/osdrv_ndkdeviceconfig.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
static uint32_t nimuDeviceIndex = 0;
static void * nimuEmacHandle[MAX_ICSSNIMU_TBL_ENTRIES];
NIMU_DEVICE_TABLE_ENTRY NIMUDeviceTable[MAX_ICSSNIMU_TBL_ENTRIES];

/*
 * Desc
 *       This API adds initialization function to NDK device table,
 *        that will be executed by NDK on startup. This needs to be called before BIOS_Start
 * Params
 * Return
 *      0 - Failure
 *      1 - Success
 *
 */
int32_t OSDRV_addNetifEntry(NIMUInitFn initFct, void * emacHandle)
{

    if(initFct == NULL)
    {
        return -1;
    }

    if(nimuDeviceIndex < MAX_ICSSNIMU_TBL_ENTRIES)
    {
        NIMUDeviceTable[nimuDeviceIndex].init =  initFct ;
        nimuEmacHandle[nimuDeviceIndex++] = emacHandle;

        if(nimuDeviceIndex < MAX_ICSSNIMU_TBL_ENTRIES)
        {
            NIMUDeviceTable[nimuDeviceIndex].init =  NULL ;
        }
    }

    else
    {
        return -1;
    }

    return 1;
}

void * OSDRV_getNetifHandle(uint8_t inst)
{
    return nimuEmacHandle[inst];
}
