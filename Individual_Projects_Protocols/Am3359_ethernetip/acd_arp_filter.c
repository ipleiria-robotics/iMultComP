/**
 * \file acd_arp_filter.c
 * \brief
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
#include "eip_main.h"
#include <stdio.h>

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

unsigned int countACD = 0;
extern unsigned int acdARPFlag;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
/**
 *  @brief  Function checks whether the ARP  ( Address Resolution Protocol) which Tx by the driver is Gratituous ARP or not
 *
 *          Used in ACD mode to implement the workaroumd where NDK the Gratitous ARP send by NDK stack
 *          can be blocked
 *
 *  @param  none
 *
 *  @retval  The packet is Gratitous ARP or not
 *
 */
int32_t EIPACD_checkGratArp(ICSS_EmacTxArgument *txArg)
{

    int32_t retVal = 0;
    Uint16 *typeProt;
    Uint16  typeProt1;
    Uint16  typeProt2;
    Uint16  *isGrac;
    Uint16  isGracIP16;
    Uint32  isGracIPSender;
    Uint32  isGracIPTarget;

    typeProt = (Uint16 *)(txArg->srcAddress) + 6;
    typeProt1 = (*typeProt << 8);
    typeProt2 = (*typeProt >> 8);
    typeProt1 = typeProt1 | typeProt2;

    if(typeProt1 == 0x0806)   /* Check for ARP packets*/
    {
        if(!countACD)
        {
            if(acdARPFlag)   /* check who should sent ARPs*/
            {
                return (1); /* tell driver this packet shoouldnot be sent*/
            }

            else
            {
                isGrac = (Uint16 *)(txArg->srcAddress) + 19;
                isGracIP16 = *isGrac;
                isGracIPTarget = isGracIP16 << 8;
                isGracIPTarget |= isGracIP16 >> 8;
                isGracIPTarget = isGracIPTarget << 16;
                isGrac++;
                isGracIP16 = *isGrac;


                isGracIPTarget |=  isGracIPTarget | (isGracIP16 << 8 & 0x0000FF00);
                isGracIPTarget |=  isGracIPTarget | (isGracIP16 >> 8 & 0x000000FF);


                isGrac = (Uint16 *)(txArg->srcAddress) + 14;
                isGracIP16 = *isGrac;
                isGracIPSender = isGracIP16 << 8;
                isGracIPSender |= isGracIP16 >> 8;
                isGracIPSender = isGracIPSender << 16;
                isGrac++;
                isGracIP16 = *isGrac;
                isGracIPSender |=  isGracIPSender | (isGracIP16 << 8 & 0x0000FF00);
                isGracIPSender |=  isGracIPSender | (isGracIP16 >> 8 & 0x000000FF);

                if(isGracIPTarget == isGracIPSender)
                {
                    return (1);
                }
            }
        }

        else
        {
            countACD--;
        }

    }

    ICSS_EmacTxPacket(txArg, NULL);
    return(retVal);
}
