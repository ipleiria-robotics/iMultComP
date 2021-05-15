/**
 * board_platform.c
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
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <ti/starterware/include/board.h>
#include <ti/starterware/include/prcm.h>
#include <ti/starterware/include/error.h>
#include <examples/board/include/board_platform.h>
#include <ti/starterware/include/device.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
/**
 * \brief
 *
 * \param
 *
 *
 * \return
 *
 **/
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

uint32_t Board_initPlatform(uint32_t moduleId, uint32_t devId,
                            uint32_t instNum, uint32_t *devInstNum, uint32_t *devCtrlInfo,
                            uint32_t *instAddr)
{
    int32_t status = S_PASS;
    chipdbModuleID_t modId = CHIPDB_MOD_ID_INVALID;

    //    modId = BOARDGetDeviceCtrlModId(devId, instNum);
    if(devId == DEVICE_ID_W25Q64BV)
    {
        modId = CHIPDB_MOD_ID_MCSPI; /* Directly from the starterware file to remove lib usage */
        *devInstNum = 0U; /* Directly from the starterware file to remove lib usage */
        *devCtrlInfo = 0U; /* Directly from the starterware file to remove lib usage */
    }

    else if(devId == DEVICE_ID_HVS882 && instNum == 0)
    {
        modId = CHIPDB_MOD_ID_MCSPI; /* Directly from the starterware file to remove lib usage */
        *devInstNum = 0U; /* Directly from the starterware file to remove lib usage */
        *devCtrlInfo = 1U; /* Directly from the starterware file to remove lib usage */
    }

    else if(devId == DEVICE_ID_HVS882 && instNum == 1)
    {
        modId = CHIPDB_MOD_ID_GPIO; /* Directly from the starterware file to remove lib usage */
        *devInstNum = 3U; /* Directly from the starterware file to remove lib usage */
        *devCtrlInfo = 18U; /* Directly from the starterware file to remove lib usage */
    }

    if(CHIPDB_MOD_ID_INVALID == modId)
    {
        status = E_FAIL;
        return status;
    }

    else if(moduleId == modId)
    {

        if((INVALID_INST_NUM == *devInstNum) || (INVALID_INFO == *devCtrlInfo))
        {
            status = E_FAIL;
            return status;
        }

    }

    else
    {
        status = E_FAIL;
        return status;
    }

    /* End Board info */

    /* Start SOC info */
    if(true == CHIPDBIsResourcePresent(moduleId, *devInstNum))
    {
        *instAddr = CHIPDBBaseAddress(moduleId,    *devInstNum);
    }

    else
    {
        status = E_FAIL;
        return status;
    }

    /* End SOC info */

    status = PRCMModuleEnable(moduleId, *devInstNum, 0u);

    return status;
}


/* -------------------------------------------------------------------------- */
/*                 Internal Function Definitions                              */
/* -------------------------------------------------------------------------- */
