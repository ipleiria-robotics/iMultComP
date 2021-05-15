/*
 * @file    profi_edma.c
 * @brief   EDMA support functions for profibus
 *          EDMA functions implementation for allocate and free channels, memcopy,
 *          memset, set parameters, handle completion and error interrupts.
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
/* Application header files */
#include <stdlib.h>


/* Stack header files */

#include <AM335x/L2.h>

#include <ti/sdo/edma3/drv/edma3_drv.h>
#include <ti/sdo/edma3/drv/sample/bios6_edma3_drv_sample.h>

#include <AM335x/profi_edma.h>
#include <AM335x/profi_soc.h>

/****************************************************************************
 *                      Defines
 ****************************************************************************/


/****************************************************************************
 *                      FUNCTION DEFINITIONS
 ****************************************************************************/

/****************************************************************************/
/*                      GLOBAL VARIABLES                                    */
/****************************************************************************/

volatile unsigned int clBackFlag = 0;


EDMA3_RM_Handle gEdmaHandle = NULL;
uint32_t chId = 0;
uint32_t tcc = 0;
uint32_t chId1 = 0;
uint32_t tcc1 = 0;

/****************************************************************************
 *                      Defines
 ****************************************************************************/
#define OPT_SYNCDIM_SHIFT                   (0x00000002u)
#define OPT_TCC_MASK                        (0x0003F000u)
#define OPT_TCC_SHIFT                       (0x0000000Cu)
#define OPT_ITCINTEN_SHIFT                  (0x00000015u)
#define OPT_TCINTEN_SHIFT                   (0x00000014u)

/****************************************************************************
 *                      FUNCTION PROTOTYPES
 ****************************************************************************/
void callback1(uint32_t tcc, EDMA3_RM_TccStatus status, void *appData);

int32_t setParamAndCopy(uint32_t srcBuffer, uint32_t destbuffer, uint32_t length);

/****************************************************************************
 *                      FUNCTION DEFINITIONS
 ****************************************************************************/


void profi_edma_init(void)
{
    EDMA3_DRV_Result edmaResult = 0;
    gEdmaHandle = (EDMA3_RM_Handle)edma3init(0, &edmaResult);

    if(edmaResult != EDMA3_DRV_SOK)
    {
        /* Report EDMA Error */

        return;
    }

    /*setup for channel*/
    tcc = EDMA3_DRV_TCC_ANY;
    chId = ARM_EDMA_CHANNEL;

    if(edmaResult == EDMA3_DRV_SOK)
    {
        edmaResult = EDMA3_DRV_requestChannel(gEdmaHandle, &chId, &tcc,
                                              (EDMA3_RM_EventQueue)0,
                                              &callback1, NULL);
    }

    profi_soc_edma_init();
}

int edma_memcpy(int channel, unsigned int dst, unsigned int src,
                unsigned int length)
{

    int result = -1;
    result = setParamAndCopy(src, dst, length);

    if(result == 0)
    {
        /* Wait for return from callback */
        while(0 == clBackFlag);

        clBackFlag = 0;
    }

    return result;



}


int32_t setParamAndCopy(uint32_t srcBuffer, uint32_t destbuffer, uint32_t length)
{

    int result = -1;
    EDMA3_DRV_PaRAMRegs paramSet   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


    uint32_t BRCnt = 0;
    int srcbidx = 0, desbidx = 0;
    int srccidx = 0, descidx = 0;
    uint32_t acnt;
    uint32_t bcnt;
    uint32_t ccnt;

    acnt = length;
    bcnt = 1;
    ccnt = 1;

    EDMA3_DRV_getPaRAM((EDMA3_DRV_Handle) gEdmaHandle,
                       (uint32_t)chId , &paramSet);
    srcbidx = (int)acnt;
    desbidx = (int)acnt;
    srccidx = (int)acnt;
    descidx = (int)acnt;
    paramSet.srcAddr    = (uint32_t)(srcBuffer);
    paramSet.destAddr   = (uint32_t)(destbuffer);



    paramSet.srcBIdx    = srcbidx;
    paramSet.destBIdx   = desbidx;
    paramSet.srcCIdx    = srccidx;
    paramSet.destCIdx   = descidx;

    paramSet.aCnt       = acnt;
    paramSet.bCnt       = bcnt;
    paramSet.cCnt       = ccnt;

    /* For AB-synchronized transfers, BCNTRLD is not used. */
    paramSet.bCntReload = BRCnt;

    paramSet.linkAddr   = 0xFFFFu;

    /* Src & Dest are in INCR modes */
    paramSet.opt &= 0xFFFFFFFCu;
    /* Program the TCC */
    paramSet.opt |= ((tcc << OPT_TCC_SHIFT) & OPT_TCC_MASK);

    /* Enable Intermediate & Final transfer completion interrupt */
    paramSet.opt |= (1 << OPT_ITCINTEN_SHIFT);
    paramSet.opt |= (1 << OPT_TCINTEN_SHIFT);


    {
        paramSet.opt &= 0xFFFFFFFBu;
    }

    /* Now, write the PaRAM Set. */
    result = EDMA3_DRV_setPaRAM(gEdmaHandle , chId, &paramSet);

    if(result == 0)
    {
        result = EDMA3_DRV_enableTransfer(gEdmaHandle , chId,
                                          EDMA3_DRV_TRIG_MODE_MANUAL);
    }

    return(result) ;

}

/****************************************************************************
 *  name        : callback
 *  description : This function is used as a callback from EDMA3 Completion Handler.
 *
 *
 *
 *
 ***************************************************************************/

void callback1(uint32_t tcc, EDMA3_RM_TccStatus status,
               void *appData)
{
    clBackFlag = 1;
}

