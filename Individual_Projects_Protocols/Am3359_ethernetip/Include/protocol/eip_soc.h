/**
 *  \file   eip_soc.h
 *
 */

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
 **/
#ifndef _EIP_SOC_H_
#define _EIP_SOC_H_

#include <stdint.h>

#ifdef SOC_AM335x
#include <ti/starterware/include/board.h>
#include <ti/starterware/include/hw/soc_am335x.h>
#include <ti/starterware/include/device.h>
#endif

#ifdef SOC_AM437x
#include <ti/starterware/include/hw/am437x.h>
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

#include <ti/drv/icss_emac/icss_emacFwInit.h>
#include <ti/drv/icss_emac/icss_emacLearning.h>
#include <ti/drv/icss_emac/icss_emacStatistics.h>
#include <ti/drv/icss_emac/icss_emacDrv_Def.h>
#include <ti/drv/icss_emac/icss_emacStormControl.h>
#include <ti/transport/ndk/nimu_icss/example/src/osdrv_ndkdeviceconfig.h>
#include <ti/drv/icss_emac/icss_emacDrv.h>

#include <Include/protocol/timeSync/icss_timeSyncApi.h>
#include <Include/protocol/timeSync/icss_timeSync_osal.h>
#include <Include/protocol/icss_dlr.h>
#include <Include/protocol/icss_dlrDef.h>

#include <Include/board/board_phy.h>
#include <Include/board/board_gpioLed.h>
#include <Include/board/board_spi.h>


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

#if defined(SOC_AM437x)
#define PRUICSS_BASEADDRESS_INSTANCE     PRUICSS_INSTANCE-1
#else
#define PRUICSS_BASEADDRESS_INSTANCE     PRUICSS_INSTANCE
#endif

#if defined(SOC_K2G)
#define EIP_CSL_ECAP_REG        CSL_ECAP_1_CFG_REGS;
#define PRU_ICSS_VERSION        V2

#elif defined(SOC_AM571x) || defined(SOC_AM572x)
#define EIP_RX_INT_NUM        120 + 32  /*ARM interrupt number 20 on AM3x, 20+32 on AM4x, 20+100+32 on AM5x;*/
#define EIP_LINK_INT_NUM      126 + 32; /*ARM interrupt number 26 on AM3x, 26+32 on AM4x, 26+100+32 on AM5x;*/
#define EIP_PTP_INT_NUM       123 + 32; /*ARM interrupt number 23 on AM3x, 23+32 on AM4x, 23+100+32 on AM5x;*/
#define EIP_DLR_P0_INT_NUM    121 + 32; /*ARM interrupt number 21 on AM3x, 21+32 on AM4x, 21+100+32 on AM5x;*/
#define EIP_DLR_P1_INT_NUM    122 + 32; /*ARM interrupt number 22 on AM3x, 22+32 on AM4x, 22+100+32 on AM5x;*/
#define EIP_BEACON_TIMEOUT_INT_NUM_P0    124 + 32; /*ARM interrupt number 27 on AM3x, 27+32 on AM4x, 27+100+32 on AM5x;*/
#define EIP_BEACON_TIMEOUT_INT_NUM_P1    127 + 32; /*ARM interrupt number 27 on AM3x, 27+32 on AM4x, 27+100+32 on AM5x;*/
#define PRU_ICSS_VERSION        V2

#define EIP_CSL_ECAP_REG        CSL_MPU_PWMSS1_IECAP_REGS;
#elif defined(SOC_AM335x)
#define EIP_RX_INT_NUM                  20
#define EIP_LINK_INT_NUM                26
#define EIP_PTP_INT_NUM                 23
#define EIP_DLR_P0_INT_NUM              21
#define EIP_DLR_P1_INT_NUM              22
#define EIP_BEACON_TIMEOUT_INT_NUM_P0   24
#define EIP_BEACON_TIMEOUT_INT_NUM_P1   27
#define PRU_ICSS_VERSION        V1

#define EIP_CSL_ECAP_REG        SOC_ECAP_0_REGS;
#elif defined(SOC_AM437x)
#define EIP_RX_INT_NUM                  52
#define EIP_LINK_INT_NUM                58
#define EIP_PTP_INT_NUM                 55
#define EIP_DLR_P0_INT_NUM              53
#define EIP_DLR_P1_INT_NUM              54
#define EIP_BEACON_TIMEOUT_INT_NUM_P0   56
#define EIP_BEACON_TIMEOUT_INT_NUM_P1   59
#define PRU_ICSS_VERSION        V1

#define EIP_CSL_ECAP_REG        SOC_PWMSS0_ECAP_REG;
#endif

#if defined(SOC_AM335x) || defined(SOC_AM437x)
#define EIP_TIMER_ID    DMTIMER_4_ID
#else
#define EIP_TIMER_ID    TimerP_ANY
#endif

void EIP_configureInterrupts(ICSSEMAC_InitConfig *switchEmacCfg);

void EIP_socgetMACAddress(uint8_t *lclMac);

void EIP_socHwinit(uint8_t instance);

void EIP_socInit();

void EIP_crossbarConfig(uint8_t instance);

void EIP_configurePHYLeds();

#endif /* _EIP_SOC_H_*/
