/**
 *  \file   pn_soc.h
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
#ifndef _PN_SOC_H_
#define _PN_SOC_H_

#include <stdint.h>
#include <Profinet/Include_profinet/PN_Handle.h>
#include <Profinet/Include_profinet/PN_HandleDef.h>


#include <ti/starterware/include/board.h>
#include <ti/starterware/include/hw/soc_am335x.h>
#include <ti/starterware/include/device.h>
#include "chipdb.h"
#include "soc_control.h"



#include <ti/csl/src/ip/mdio/V2/cslr_mdio.h>
#include <ti/csl/src/ip/mdio/V2/csl_mdio.h>
#include <ti/csl/src/ip/mdio/V2/csl_mdioAux.h>


#define PRUICSS_INSTANCE_ONE   1
#define PRUICSS_INSTANCE_TWO   2

#define DEFAULT_PRUICSS_INSTANCE    PRUICSS_INSTANCE_ONE


#define PRUICSS_INSTANCE        DEFAULT_PRUICSS_INSTANCE

#define PRUICSS_BASEADDRESS_INSTANCE     PRUICSS_INSTANCE


#define PN_RX_INT_NUM          20
#define PN_LINK_INT_NUM        26

#define PN_CSL_ECAP_REG        SOC_ECAP_0_REGS;

#define PN_PTCP_TIMERID         3  /*DMTIMER 5. SYSBIOS Timer Id is 3*/
#define PN_PTCP_TIMER_FREQ      24000000



void PN_socinitIRTHandle(PN_Handle pnHandle);

void PN_socgetMACAddress(uint8_t index, uint8_t *lclMac);

void PN_socHwinit(uint8_t instance);

void PN_socInit();

void PN_configurePHYLeds();

#endif /* _PN_SOC_H_*/
