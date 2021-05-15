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
#include <include/PN_Handle.h>
#include <include/PN_HandleDef.h>

#ifdef SOC_AM335x
#include <ti/starterware/include/board.h>
#include <ti/starterware/include/hw/soc_am335x.h>
#include <ti/starterware/include/device.h>
#include "chipdb.h"
#include "soc_control.h"
#endif

#ifdef SOC_AM437x
#include <ti/starterware/include/hw/am437x.h>
#include "chipdb.h"
#include "soc_control.h"
#endif

#if defined(SOC_AM571x)
#include <ti/csl/soc/am571x/src/cslr_soc_mpu_baseaddress.h>
#include <ti/csl/soc/am571x/src/cslr_control_core.h>
#ifdef C66X
#include <ti/sysbios/family/c66/Cache.h>
#include <ti/csl/soc/am571x/src/csl_device_xbar.h>
#endif
#elif defined(SOC_AM572x)
#include <ti/csl/soc/am572x/src/cslr_soc_mpu_baseaddress.h>
#include <ti/csl/soc/am572x/src/cslr_control_core.h>
#ifdef C66X
#include <ti/sysbios/family/c66/Cache.h>
#include <ti/csl/soc/am572x/src/csl_device_xbar.h>
#endif
#endif

#if !defined (SOC_K2G)
#include <ti/csl/src/ip/mdio/V2/cslr_mdio.h>
#include <ti/csl/src/ip/mdio/V2/csl_mdio.h>
#include <ti/csl/src/ip/mdio/V2/csl_mdioAux.h>
#endif

#define PRUICSS_INSTANCE_ONE   1
#define PRUICSS_INSTANCE_TWO   2


#if defined(SOC_AM571x)
#define DEFAULT_PRUICSS_INSTANCE    PRUICSS_INSTANCE_TWO
#elif defined(SOC_AM572x)
#define DEFAULT_PRUICSS_INSTANCE    PRUICSS_INSTANCE_TWO
#elif defined(SOC_K2G)
#define DEFAULT_PRUICSS_INSTANCE    PRUICSS_INSTANCE_TWO
#elif defined(SOC_AM335x)
#define DEFAULT_PRUICSS_INSTANCE    PRUICSS_INSTANCE_ONE
#elif defined(SOC_AM437x)
#define DEFAULT_PRUICSS_INSTANCE    PRUICSS_INSTANCE_TWO
#else
#define DEFAULT_PRUICSS_INSTANCE    PRUICSS_INSTANCE_TWO
#endif



/* Change this define to switch between PRUICSS for AM571x */
#define PRUICSS_INSTANCE        DEFAULT_PRUICSS_INSTANCE

#if defined(SOC_K2G)

#if PRUICSS_INSTANCE == PRUICSS_INSTANCE_ONE
#define PN_RX_INT_NUM     CSL_ARM_GIC_ICSSM0_HOST_INT0 + 32 /*ARM interrupt number 20 on AM3x, 20+32 on AM4x, 20+100+32 on AM5x;*/
#define PN_LINK_INT_NUM   CSL_ARM_GIC_ICSSM0_HOST_INT6 + 32
#else
#define PN_RX_INT_NUM     CSL_ARM_GIC_ICSSM1_HOST_INT0 + 32 /*ARM interrupt number 20 on AM3x, 20+32 on AM4x, 20+100+32 on AM5x;*/
#define PN_LINK_INT_NUM   CSL_ARM_GIC_ICSSM1_HOST_INT6 + 32
#endif

#define PN_CSL_ECAP_REG       CSL_ECAP_1_CFG_REGS

#endif

#if defined(SOC_AM437x)
#define PRUICSS_BASEADDRESS_INSTANCE     PRUICSS_INSTANCE-1
#else
#define PRUICSS_BASEADDRESS_INSTANCE     PRUICSS_INSTANCE
#endif

#if defined(SOC_AM571x) || defined(SOC_AM572x)

#ifdef C66X
#define PN_RX_INT_NUM     75
#define PN_LINK_INT_NUM   81
#else
#define PN_RX_INT_NUM     120 +32 /*ARM interrupt number 20 on AM3x, 20+32 on AM4x, 20+100+32 on AM5x;*/
#define PN_LINK_INT_NUM   126 +  32 /*ARM interrupt number 26 on AM3x, 26+32 on AM4x, 26+100+32 on AM5x;*/
#endif

#define PN_CSL_ECAP_REG        CSL_MPU_PWMSS1_IECAP_REGS;
#elif defined(SOC_AM335x)
#define PN_RX_INT_NUM          20
#define PN_LINK_INT_NUM        26

#define PN_CSL_ECAP_REG        SOC_ECAP_0_REGS;
#elif defined(SOC_AM437x)
#define PN_RX_INT_NUM          52
#define PN_LINK_INT_NUM        58

#define PN_CSL_ECAP_REG        SOC_PWMSS0_ECAP_REG;
#endif


#if defined(SOC_K2G)
#define PN_PTCP_TIMERID         3 /*DMTIMER 3*/
#define PN_PTCP_TIMER_FREQ      0
#elif defined (SOC_AM335x)
#define PN_PTCP_TIMERID         3  /*DMTIMER 5. SYSBIOS Timer Id is 3*/
#define PN_PTCP_TIMER_FREQ      24000000
#elif defined (SOC_AM437x)
#define PN_PTCP_TIMERID         3  /*DMTIMER 3*/
#define PN_PTCP_TIMER_FREQ      24000000
#elif defined (SOC_AM571x)
#define PN_PTCP_TIMERID         TimerP_ANY
#define PN_PTCP_TIMER_FREQ      24000000
#elif defined (SOC_AM572x)
#define PN_PTCP_TIMERID         TimerP_ANY
#define PN_PTCP_TIMER_FREQ      24000000
#endif


void PN_socinitIRTHandle(PN_Handle pnHandle);

void PN_socgetMACAddress(uint8_t index, uint8_t *lclMac);

void PN_socHwinit(uint8_t instance);

void PN_socInit();

void PN_configurePHYLeds();

#endif /* _PN_SOC_H_*/
