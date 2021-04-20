/**
 * @file    iPtcpDrv.c
 *
 * @brief   Driver for PTCP protocol
 *      It includes cable/line delay calculation, sync handling and clock synchronization
 *
 * \par
 * * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
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
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 * \par
 *
 */


/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
/*
 * pnDrvConfig.h needs to be first first file included in driver!
 * It is application dependent and as such part of the application code.
 * It defines all basic feature options in driver (compile time!)
 */
#include <stdint.h>
#include <xdc/runtime/Error.h>
#include <xdc/cfg/global.h>
#include <ti/sysbios/knl/Task.h>
#include <Profinet/include/protocol_slave/RT_MRP/pnDrvConfig.h>

#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/Types.h>


#include <Profinet/include/protocol_slave/PN_Handle.h>
#include <Profinet/include/protocol_slave/PN_HandleDef.h>

#include <Profinet/include/protocol_slave/iRtcDrv2.h>
#include <Profinet/include/protocol_slave/iPtcpDrv.h>
#include <Profinet/include/protocol_slave/iPtcpUtils.h>
#include <Profinet/include/protocol_slave/iPNLegacy.h>
#include <Profinet/include/protocol_slave/iPnOs.h>

#include <ti/drv/icss_emac/icss_emacDrv_Def.h>
#include <ti/drv/gpio/GPIO.h>

#include <xdc/runtime/System.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Clock.h>

#ifdef PTCP_SUPPORT
#include <ti/drv/icss_emac/icss_emac_ver.h>
#include <ti/drv/icss_emac/src/icss_emacLoc.h>

#include <ti/drv/icss_emac/icss_emacCommon.h>

#include <ti/csl/cslr_ecap.h>
#include <ti/sdo/edma3/drv/edma3_drv.h>

#include <string.h>

#include <ti/csl/hw_types.h>
#include <ti/osal/osal.h>
#include <ti/drv/icss_emac/icss_emacFwInit.h>
#include <ti/drv/uart/UART_stdio.h>
#include <Include/Board_am335x/soc_icss_header.h>

#include <osal/TaskP.h>
#include <Profinet/pn_soc.h>

/*enable RED sync LED on ICE*/
#define PTCP_SYNC_SIGNAL

#define DEBUG_SYNC_EVENTS
#ifdef DEBUG_SYNC_EVENTS
uint32_t syncmissCounter = 0;

#endif

/* OPT Field specific defines */
#define OPT_SYNCDIM_SHIFT                   (0x00000002u)
#define OPT_TCC_MASK                        (0x0003F000u)
#define OPT_TCC_SHIFT                       (0x0000000Cu)
#define OPT_ITCINTEN_SHIFT                  (0x00000015u)
#define OPT_TCINTEN_SHIFT                   (0x00000014u)
#define OPT_STATIC_SHIFT                    (0x00000003u)

/* Macros to define DELAY REQ and RES packets */
#define DA_ID1 0x8001
#define DA_ID2 0x00c2
#define DA_ID3 0x0e00


#define SA_ID1 0x0000
#define SA_ID2 0x0000
#define SA_ID3 0x0000

#define ETHER_TYPE 0x9288
#define DELAY_REQ_FRAME_ID   0x40ff
#define DELAY_RES_FRAME_ID   0x43ff

#define DELAY_RES_TYPE_LENGTH1 0x0a0a
#define DELAY_RES_TYPE_LENGTH2 0x0000

#define DELAY_RES_PORT_RX_DELAY1 0x0000
#define DELAY_RES_PORT_RX_DELAY2 0xb800

#define DELAY_RES_PORT_TX_DELAY1 0x0000
#define DELAY_RES_PORT_TX_DELAY2 0x5600

#define DELAY_RES_TYPE_LENGTH 0x060e

#define ZERO_PADDING 0x0000

#define TLV_DELAY 0x060c

/* ========================================================================== */
/*                             Static variables                               */
/* ========================================================================== */


/**Syslog sync packet*/
uint8_t syncSysLogFrame[] = \

{
    0x00, 0x1b, 0x1b, 0x35, 0x4e, 0xcf, 0xc4, 0xed, 0xba, 0x86, 0xfe, 0x39,
    0x08, 0x00, 0x45, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x80, 0x11,
    0x00, 0x00, 0xC0, 0xA8, 0x01, 0x32, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02,
    0x02, 0x02, 0x00, 0x2A, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20
};
#ifdef LATCH_DEBUG
#define LDBG_SZ 40

LatchVars       g_dbg_latch[LDBG_SZ];
PNIO_TimeStamp  g_dbg_T1[LDBG_SZ];
uint32_t        td1[LDBG_SZ];
uint64_t        tickdiff[LDBG_SZ];
int32_t         index_dbg_api = 0;
#endif

/* ========================================================================== */
/*                             Function Definitions                           */
/* ========================================================================== */
/* External Declaration */
extern EDMA3_DRV_Handle edma3init(unsigned int edma3Id,
                                  EDMA3_DRV_Result *errorCode);

void PN_PTCP_timerHandler(void* arg);

void PN_PTCP_registerSyncStatusCall(PN_Handle pnHandle, ptcpCallBack_t callBack)
{
    (pnHandle->pnPtcpConfig)->ptcpSyncStatusCall = callBack;
}

void PN_PTCP_registerDelayUpdateCall(PN_Handle pnHandle,
                                     ptcpCallBack_t callBack)
{
    (pnHandle->pnPtcpConfig)->ptcpDelayUpdateCall = callBack;
}

void PN_PTCP_start(PN_Handle pnHandle)
{
    SemaphoreP_post((pnHandle->pnPtcpConfig)->ptcpStartSem);
}

void PN_PTCP_triggerMeasurement(PN_Handle pnHandle)
{
    SemaphoreP_post((pnHandle->pnPtcpConfig)->ptcpTriggerMeasurementSem);
}

void PN_PTCP_ClockChange(PN_Handle pnHandle, uint32_t cycleTime)
{
    if((pnHandle->pnPtcpConfig)->pnCyclePeriod == cycleTime)
    {
        return;
    }

    (pnHandle->pnPtcpConfig)->pnCyclePeriod = cycleTime;
    (pnHandle->pnPtcpConfig)->clkChangeNotifyDelay = 1;

    /* reset ptcp sync : direct adjustment and reset the filter*/
    PN_PTCP_reset(pnHandle);
}

void PN_PTCP_init(PN_Handle pnHandle)
{
#ifdef SOC_K2G
    uint32_t ptcpTimerID;
#endif
    uint16_t *pTemp16;
    uint8_t *pTemp8;
    SemaphoreP_Params semParams;
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;
    PN_PtcpConfig *ptcpConfig = pnHandle->pnPtcpConfig;
    uint32_t ocmcBaseAddress = emacBaseAddr->l3OcmcBaseAddr;

    /*Write the Delay Req Frame for PORT1 in ICSS Shared RAM*/
    pTemp16 = (uint16_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                           PORT1_DELAY_REQ_FRAME_OFFSET);
    /*Write DA*/
    *pTemp16++ = DA_ID1;
    *pTemp16++ = DA_ID2;
    *pTemp16++ = DA_ID3;
    /*Write SA .. Port MAC Address*/
    *pTemp16++ = SA_ID1;
    *pTemp16++ = SA_ID2;
    *pTemp16++ = SA_ID3;
    /*Write Type and Frame ID*/
    *pTemp16++ = ETHER_TYPE;
    *pTemp16++ = DELAY_REQ_FRAME_ID;
    /*Write 0 padding*/
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    /* Write the Initial value of Seq ID*/
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    /* Write the TLV - Delay Parameter*/
    *pTemp16++ = TLV_DELAY;
    /*Write SA .. Port MAC Address*/
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    /*crypto: check reason for extra padding: min length ??*/
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;

    /* write Dst MAC addr at two locations*/
    pTemp8 = (uint8_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                         PORT1_DELAY_REQ_FRAME_OFFSET + 6);
    PN_EmacSocMACAddrGet(pnHandle, PORT1_MAC, pTemp8);

    pTemp8 = (uint8_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                         PORT1_DELAY_REQ_FRAME_OFFSET + 38);
    PN_EmacSocMACAddrGet(pnHandle, PORT1_MAC, pTemp8);

    /*Write the Delay Req Frame for PORT2 in ICSS Shared RAM*/
    pTemp16 = (uint16_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                           PORT2_DELAY_REQ_FRAME_OFFSET);
    /*Write DA*/
    *pTemp16++ = DA_ID1;
    *pTemp16++ = DA_ID2;
    *pTemp16++ = DA_ID3;
    /*Write SA .. Port MAC Address*/
    *pTemp16++ = SA_ID1;
    *pTemp16++ = SA_ID2;
    *pTemp16++ = SA_ID3;
    /*Write Type and Frame ID*/
    *pTemp16++ = ETHER_TYPE;
    *pTemp16++ = DELAY_REQ_FRAME_ID;
    /*Write 0 padding*/
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    /* Write the Initial value of Seq ID*/
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    /* Write the TLV - Delay Parameter*/
    *pTemp16++ = TLV_DELAY;
    /*Write SA .. Port MAC Address*/
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    /*crypto: check reason for extra padding: min length ??*/
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;

    /* write Src MAC addr at two locations*/
    pTemp8 = (uint8_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                         PORT2_DELAY_REQ_FRAME_OFFSET + 6);
    PN_EmacSocMACAddrGet(pnHandle, PORT2_MAC, pTemp8);

    pTemp8 = (uint8_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                         PORT2_DELAY_REQ_FRAME_OFFSET + 38);
    PN_EmacSocMACAddrGet(pnHandle, PORT2_MAC, pTemp8);

    /* Write the Delay_Response Frame for Port1 in ICSS Shared RAM*/
    pTemp16 = (uint16_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                           PORT1_DELAY_RESP_FRAME_OFFSET);
    /*Write DA*/
    *pTemp16++ = DA_ID1;
    *pTemp16++ = DA_ID2;
    *pTemp16++ = DA_ID3;
    /*Write SA .. Port MAC Address*/
    *pTemp16++ = SA_ID1;
    *pTemp16++ = SA_ID2;
    *pTemp16++ = SA_ID3;
    /*Write Type and Frame ID*/
    *pTemp16++ = ETHER_TYPE;
    *pTemp16++ = DELAY_RES_FRAME_ID;
    /* Write the Initial value of Seq ID / padding/delay*/
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    /* Write the TLV - Delay Parameter*/
    *pTemp16++ = TLV_DELAY;
    /*Write requestor Port MAC Address*/
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    /* Write TypeLength*/
    *pTemp16++ = DELAY_RES_TYPE_LENGTH1;
    *pTemp16++ = DELAY_RES_TYPE_LENGTH2;
    /* T2 PortRxDelay*/
    *pTemp16++ = DELAY_RES_PORT_RX_DELAY1;
    *pTemp16++ = DELAY_RES_PORT_RX_DELAY2;
    /* T3 PortTxDelay*/
    *pTemp16++ = DELAY_RES_PORT_TX_DELAY1;
    *pTemp16++ = DELAY_RES_PORT_TX_DELAY2;
    /* TypeLength*/
    *pTemp16++ = DELAY_RES_TYPE_LENGTH;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;

    /* write Src MAC addr*/
    pTemp8 = (uint8_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                         PORT1_DELAY_RESP_FRAME_OFFSET + 6);
    PN_EmacSocMACAddrGet(pnHandle, PORT1_MAC, pTemp8);

    /* Write the Delay_Response Frame for Port2 in ICSS Shared RAM*/
    pTemp16 = (uint16_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                           PORT2_DELAY_RESP_FRAME_OFFSET);
    /*Write DA*/
    *pTemp16++ = DA_ID1;
    *pTemp16++ = DA_ID2;
    *pTemp16++ = DA_ID3;
    /*Write SA .. Port MAC Address*/
    *pTemp16++ = SA_ID1;
    *pTemp16++ = SA_ID2;
    *pTemp16++ = SA_ID3;
    /*Write Type and Frame ID*/
    *pTemp16++ = ETHER_TYPE;
    *pTemp16++ = DELAY_RES_FRAME_ID;
    /* Write the Initial value of Seq ID / padding/delay*/
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    /* Write the TLV - Delay Parameter*/
    *pTemp16++ = TLV_DELAY;
    /*Write requestor Port MAC Address*/
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    /* Write TypeLength*/
    *pTemp16++ = DELAY_RES_TYPE_LENGTH1;
    *pTemp16++ = DELAY_RES_TYPE_LENGTH2;
    /* T2 PortRxDelay*/
    *pTemp16++ = DELAY_RES_PORT_RX_DELAY1;
    *pTemp16++ = DELAY_RES_PORT_RX_DELAY2;
    /* T3 PortTxDelay*/
    *pTemp16++ = DELAY_RES_PORT_TX_DELAY1;
    *pTemp16++ = DELAY_RES_PORT_TX_DELAY2;
    /* TypeLength*/
    *pTemp16++ = DELAY_RES_TYPE_LENGTH;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;
    *pTemp16++ = ZERO_PADDING;

    /* write Src MAC addr*/
    pTemp8 = (uint8_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                         PORT2_DELAY_RESP_FRAME_OFFSET + 6);
    PN_EmacSocMACAddrGet(pnHandle, PORT2_MAC, pTemp8);

    /* Write to PRU0 Handshake Reg*/
    pTemp16 = (uint16_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                           PRU0_HANDSHAKE_OFFSET);
    *pTemp16++ = 0x0000;
    *pTemp16++ = 0x0000;
    /* Write to PRU1 Handshake Reg*/
    pTemp16 = (uint16_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                           PRU1_HANDSHAKE_OFFSET);
    *pTemp16++ = 0x0000;
    *pTemp16++ = 0x0000;

    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_1 - 1].pT1TS = (uint32_t *)(
                emacBaseAddr->sharedDataRamBaseAddr + P1_T1_TIME_STAMP_OFFSET);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_1 - 1].pT4TS = (uint32_t *)(
                emacBaseAddr->sharedDataRamBaseAddr + P1_T4_TIME_STAMP_OFFSET);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_1 - 1].pT1CycleCtr = (uint32_t *)(
                emacBaseAddr->sharedDataRamBaseAddr + P1_T1_CYCLE_CTR_OFFSET);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_1 - 1].pT4CycleCtr = (uint32_t *)(
                emacBaseAddr->sharedDataRamBaseAddr + P1_T4_CYCLE_CTR_OFFSET);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_1 - 1].pDelayReqPacket =
        (uint8_t *)(emacBaseAddr->sharedDataRamBaseAddr + PORT1_DELAY_REQ_FRAME_OFFSET);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_1 - 1].pSeqIdInDelayPacket =
        (uint16_t *)(emacBaseAddr->sharedDataRamBaseAddr + PORT1_DELAY_REQ_FRAME_OFFSET
                     + 28);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_1 - 1].pInDelayResPacket =
        (uint8_t *)(ocmcBaseAddress + P1_DLY_RSP_PACKET_OFFSET);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_1 - 1].pInDelayFupResPacket =
        (uint8_t *)(ocmcBaseAddress + P1_DLY_FUP_PACKET_OFFSET);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_1 - 1].pInDelayResCtrl =
        (uint8_t *)(ocmcBaseAddress + P1_PTCP_CTRL_OFFSET);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_1 - 1].pSmaLineDelay =
        (uint32_t *)(emacBaseAddr->sharedDataRamBaseAddr + P1_SMA_LINE_DELAY_OFFSET);


    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_2 - 1].pT1TS = (uint32_t *)(
                emacBaseAddr->sharedDataRamBaseAddr + P2_T1_TIME_STAMP_OFFSET);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_2 - 1].pT4TS = (uint32_t *)(
                emacBaseAddr->sharedDataRamBaseAddr + P2_T4_TIME_STAMP_OFFSET);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_2 - 1].pT1CycleCtr = (uint32_t *)(
                emacBaseAddr->sharedDataRamBaseAddr + P2_T1_CYCLE_CTR_OFFSET);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_2 - 1].pT4CycleCtr = (uint32_t *)(
                emacBaseAddr->sharedDataRamBaseAddr + P2_T4_CYCLE_CTR_OFFSET);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_2 - 1].pDelayReqPacket =
        (uint8_t *)(emacBaseAddr->sharedDataRamBaseAddr + PORT2_DELAY_REQ_FRAME_OFFSET);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_2 - 1].pSeqIdInDelayPacket =
        (uint16_t *)(emacBaseAddr->sharedDataRamBaseAddr + PORT2_DELAY_REQ_FRAME_OFFSET
                     + 28);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_2 - 1].pInDelayResPacket =
        (uint8_t *)(ocmcBaseAddress + P2_DLY_RSP_PACKET_OFFSET);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_2 - 1].pInDelayFupResPacket =
        (uint8_t *)(ocmcBaseAddress + P2_DLY_FUP_PACKET_OFFSET);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_2 - 1].pInDelayResCtrl =
        (uint8_t *)(ocmcBaseAddress + P2_PTCP_CTRL_OFFSET);
    ptcpConfig->devicePortOffsets[ICSS_EMAC_PORT_2 - 1].pSmaLineDelay =
        (uint32_t *)(emacBaseAddr->sharedDataRamBaseAddr + P2_SMA_LINE_DELAY_OFFSET);

    PN_PTCP_resetDelayValues(pnHandle);
    ptcpConfig->seqId      = 0;

    /* reset the flag, as ptcp measurement hasn't started yet*/
    ptcpConfig->clkChangeNotifyDelay = 0;

    memset((void *) & (ptcpConfig->syncDeltaTs), 0,
           ptcpConfig->ptcpSyncFilterfactor);

    ptcpConfig->syncIndex = 0;

    ptcpConfig->pSyncTorgNs         = (uint32_t *)(
                                          emacBaseAddr->sharedDataRamBaseAddr + SYNC_TORG_TIME_OFFSET +
                                          4); /* 4 byte offset for ns field*/
    ptcpConfig->pSyncTorgSecs       = (uint32_t *)(
                                          emacBaseAddr->sharedDataRamBaseAddr + SYNC_TORG_TIME_OFFSET);
    ptcpConfig->pSyncInDelayPlusLD  = (uint32_t *)(
                                          emacBaseAddr->sharedDataRamBaseAddr + SYNC_INDELAY_PLUS_LD_OFFSET);
    ptcpConfig->pSyncRxSOF          = (uint32_t *)(
                                          emacBaseAddr->sharedDataRamBaseAddr + SYNC_RX_SOF_OFFSET);
    ptcpConfig->pPmCycleCounter     = (uint16_t *)(emacBaseAddr->dataRam0BaseAddr
                                      + PTCP_PM_CYCLE_COUNTER_OFFSET);
    ptcpConfig->pPmPhaseCounter     = (uint16_t *)(emacBaseAddr->dataRam0BaseAddr
                                      + PTCP_PM_PHASE_COUNTER_OFFSET);

    ptcpConfig->pSubDomainUUID      = (uint8_t *)(
                                          emacBaseAddr->sharedDataRamBaseAddr + SYNC_UUID_OFFSET);
    ptcpConfig->pSyncMasterMac      = (uint8_t *)(
                                          emacBaseAddr->sharedDataRamBaseAddr + SYNC_MASTER_MAC_OFFSET);
    ptcpConfig->pSyncInitFlag       = (uint8_t *)(
                                          emacBaseAddr->sharedDataRamBaseAddr + SYNC_INIT_FLAG_OFFSET);

    PN_PTCP_setupIsr(pnHandle);

    /*Use Slow compensation instead of ECAP/EDMA scheme for synchronization.
    In SOC's which support slow compensation, The smooth adjustment of time can be done using the slow
    compensation register in PRUSS.In other ECAP/EDMA scheme will be used.*/
    if(!ptcpConfig->ptcpEnableSlowCompensation)
    {
        PN_PTCP_initEcap(pnHandle);
        PN_PTCP_configEdma(pnHandle);
    }

    PN_PTCP_reset(pnHandle);
    PN_PTCP_configureSync0Pin(pnHandle);

#ifdef PTCP_DEBUG
    PN_PtcpDebug *pnPtcpDebugAttrs = pnHandle->pnPtcpConfig->pnPtcpDebugAttrs;
    pnPtcpDebugAttrs->debugDelayIndex = 1;
    pnPtcpDebugAttrs->debugSyncIndex    = 1;
#endif

#ifdef PTCP_SYNC_SIGNAL
    GPIO_write(0, 0);
#endif

    memset((void *)((ptcpConfig->deviceSyncInfo)->masterSA), 0, 6);
    memset((void *)((ptcpConfig->deviceSyncInfo)->subdomainUUID), 0, 16);
    memset((void *)((ptcpConfig->currentPtcpStatus)->masterSA), 0, 6);
    memset((void *)((ptcpConfig->currentPtcpStatus)->subdomainUUID), 0, 16);

    memset((void *)(ptcpConfig->pSubDomainUUID), 0, 16);
    memset((void *)(ptcpConfig->pSyncMasterMac), 0, 6);
    memset((void *)(ptcpConfig->pSyncInitFlag), 0, 1);


    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;

    ptcpConfig->ptcpStartSem = SemaphoreP_create(0, &semParams);

    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;

    ptcpConfig->ptcpTriggerMeasurementSem = SemaphoreP_create(0, &semParams);


    /*enable by default*/
    (ptcpConfig->currentPtcpStatus)->cDelayEnable[ICSS_EMAC_PORT_1 - 1] = enable;
    (ptcpConfig->currentPtcpStatus)->cDelayEnable[ICSS_EMAC_PORT_2 - 1] = enable;

    /* enabling Single shot mode for capture register 4/5, i.e. TX PORT1 and TX PORT2*/
    HWREG(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_CAP_CFG_REG) = 0x0001FC30;

    /* enable sync forwarding by default*/
    pTemp8 = (uint8_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                         SYNC_FWD_ENABLED_OFFSET);
    *pTemp8 = 1;

    /* enable resp for delay req in firmware by default*/
    pTemp8 = (uint8_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                         P1_DELAY_RESP_CTRL_OFFSET);
    *pTemp8 = 1;

    /* enable resp for delay req in firmware by default*/
    pTemp8 = (uint8_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                         P2_DELAY_RESP_CTRL_OFFSET);
    *pTemp8 = 1;
#ifdef SOC_K2G
    TimerP_Params pnTimParams;
    TimerP_Params_init(&pnTimParams);

    pnTimParams.runMode = TimerP_RunMode_ONESHOT;
    pnTimParams.startMode = TimerP_StartMode_USER;
    pnTimParams.arg = pnHandle;
    pnTimParams.period = 10;
    pnTimParams.periodType =TimerP_PeriodType_MICROSECS;
    pnTimParams.extfreqLo = PN_PTCP_TIMER_FREQ;
    pnTimParams.extfreqHi = 0;
    ptcpTimerID = ptcpConfig->ptcpTimer->ptcpTimerID;
    ptcpConfig->ptcpTimer->ptcpTimerHandle = TimerP_create(ptcpTimerID, (TimerP_Fxn)PN_PTCP_timerHandler, &pnTimParams);
#endif
}

void PN_PTCP_reset(PN_Handle pnHandle)
{
    PN_PtcpConfig *ptcpConfig = pnHandle->pnPtcpConfig;
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;
    ((pnHandle->pnPtcpConfig)->pnPtcpDebugAttrs)->debugSyncIndex = 0;

    /* Initialize current PTCP status of the device*/
    ptcpConfig->currentPtcpStatus->syncState = OUT_OF_SYNC;
    ptcpConfig->currentPtcpStatus->syncPllWnd =
        1000;           /* default value of 1 us*/
    ptcpConfig->currentPtcpStatus->syncTimeoutFactor =
        6;       /* default value of 6 sync interval*/
    ptcpConfig->currentPtcpStatus->takeoverTimeoutFactor =
        3;   /* default value of 3 sync interval*/
    ptcpConfig->currentPtcpStatus->firstSyncRcv = 0;
    ptcpConfig->currentPtcpStatus->syncRcv = 0;
    ptcpConfig->currentPtcpStatus->nSyncMissed = 0;

    if(ptcpConfig->pnCyclePeriod == 0)
    {
        ptcpConfig->pnCyclePeriod =
            250000;    /*default value, if it is not initialized yet!*/
    }

    ptcpConfig->deviceSyncInfo->syncState = OUT_OF_SYNC;
    ptcpConfig->numInSync = 0;
    ptcpConfig->syncIndex = 0;
    ptcpConfig->initPmCycleCtrDone = 0;

    HWREGB(emacBaseAddr->dataRam0BaseAddr
           + RTC_DEVICE_SYNC_STATUS_OFFSET) =  0;

    if((pnHandle->pnPtcpConfig)->ptcpSyncStatusCall != NULL)
    {
        (pnHandle->pnPtcpConfig)->ptcpSyncStatusCall((
                    pnHandle->pnPtcpConfig)->currentPtcpStatus->syncState, (uint32_t)NULL);
    }
#ifdef PTCP_SYNC_SIGNAL
    GPIO_write(0, 0);
#endif
#ifdef SYNC_ANALYSIS
    nResets++;
    nSyncTrans = 0;
#endif
}

void PN_PTCP_delayMeasurement(PN_Handle pnHandle)
{
    int32_t i = 0;
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;
    (pnHandle->pnPtcpConfig)->mandCtrAdj = MAX_CTR_VAL * (INTER_DEL_REQ_GAP / ((
            pnHandle->pnPtcpConfig)->pnCyclePeriod * MAX_CTR_VAL));

    for(i = 0; i < NUM_DELAYS_BURST; i++)
    {
        /* increment the seqId and delayIndex*/
        (pnHandle->pnPtcpConfig)->seqId = ((pnHandle->pnPtcpConfig)->seqId + 1) %
                                          MAX_SEQID;
        (pnHandle->pnPtcpConfig)->delayIndex = ((pnHandle->pnPtcpConfig)->delayIndex +
                                                1) % NUM_DELAYS_SMA;

        /* reset the values*/
        PN_PTCP_resetDelayTimings(pnHandle, ICSS_EMAC_PORT_1 - 1);
        PN_PTCP_resetDelayTimings(pnHandle, ICSS_EMAC_PORT_2 - 1);

        /* clear the flag if clk changed before delay measurement*/
        if((pnHandle->pnPtcpConfig)->clkChangeNotifyDelay == 1)
        {
            /* check whether PRU has programmed the IEP or not*/
            uint8_t *cycleChangeDone = (uint8_t *)(emacBaseAddr->dataRam0BaseAddr +
                                                   RTC_BASE_CLK_CHANGED_OFFSET);

            if((*cycleChangeDone) == 0)
            {
                (pnHandle->pnPtcpConfig)->clkChangeNotifyDelay = 0;
                /* recalculate if cycle period has changed*/
                (pnHandle->pnPtcpConfig)->mandCtrAdj = MAX_CTR_VAL * (INTER_DEL_REQ_GAP / ((
                        pnHandle->pnPtcpConfig)->pnCyclePeriod * MAX_CTR_VAL));
            }
        }

        /* fill queue1 with delay req*/
        if((pnHandle->pnPtcpConfig)->currentPtcpStatus->cDelayEnable[0] == enable)
            PN_OS_txPacket(pnHandle->emacHandle,
                           (pnHandle->pnPtcpConfig)->devicePortOffsets[0].pDelayReqPacket,
                           ICSS_EMAC_PORT_1, ICSS_EMAC_QUEUE1, PTCP_DELAY_REQ_LEN);

        if((pnHandle->pnPtcpConfig)->currentPtcpStatus->cDelayEnable[1] == enable)
            PN_OS_txPacket(pnHandle->emacHandle,
                           (pnHandle->pnPtcpConfig)->devicePortOffsets[1].pDelayReqPacket,
                           ICSS_EMAC_PORT_2, ICSS_EMAC_QUEUE1, PTCP_DELAY_REQ_LEN);

        PN_PTCP_taskSleep(INTER_DEL_REQ_GAP_MS);


        /* process the delay res*/
        if((pnHandle->pnPtcpConfig)->currentPtcpStatus->cDelayEnable[0] == enable)
        {
            PN_PTCP_processDelayResponse(pnHandle, ICSS_EMAC_PORT_1 - 1);
        }

        if((pnHandle->pnPtcpConfig)->currentPtcpStatus->cDelayEnable[1] == enable)
        {
            PN_PTCP_processDelayResponse(pnHandle, ICSS_EMAC_PORT_2 - 1);
        }

#ifdef PTCP_DEBUG

        /* for debug: capturing cable delay for 100 iterations*/
        if((pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugDelayIndex <
                DEBUG_DELAY_N_ITER - 1)
        {
            (pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugDelayIndex++;
        }

#endif
    }
}

void PN_PTCP_resetDelayValues(PN_Handle pnHandle)
{
    int32_t i, j;
    (pnHandle->pnPtcpConfig)->delayIndex        =
        -1;   /* FW setting index to -1 may cause array out of bounds!*/
    (pnHandle->pnPtcpConfig)->firstDelayBurst = 1;


    for(i = ICSS_EMAC_PORT_1 - 1; i < PTCP_NUM_PORTS; i++)
    {
        (pnHandle->pnPtcpConfig)->portTimes[i].cableDelay     = 0;
        (pnHandle->pnPtcpConfig)->portTimes[i].rxDelayLocal = PORT_RX_DELAY;
        (pnHandle->pnPtcpConfig)->portTimes[i].txDelayLocal = PORT_TX_DELAY;
        (pnHandle->pnPtcpConfig)->portTimes[i].rxDelayRemote = 0;
        (pnHandle->pnPtcpConfig)->portTimes[i].txDelayRemote = 0;

        (pnHandle->pnPtcpConfig)->T1_prev[i]        = 0;
        (pnHandle->pnPtcpConfig)->T2_prev[i]        = 0;
        (pnHandle->pnPtcpConfig)->T1_CTR_prev[i]    = 0;

        /*currentPtcpStatus.cDelay[i] = 0;*/

        for(j = 0; j < NUM_DELAYS_SMA; j++)
        {
            (pnHandle->pnPtcpConfig)->deviceDelays[i].lDelays[j] = 0;
            (pnHandle->pnPtcpConfig)->deviceDelays[i].cDelays[j] = 0;
        }
    }

}

void PN_PTCP_resetDelayTimings(PN_Handle pnHandle, uint8_t portNum)
{
    /* reset the timing values*/
    *((pnHandle->pnPtcpConfig)->devicePortOffsets[portNum].pT1TS)           =
        0x00000000;
    *((pnHandle->pnPtcpConfig)->devicePortOffsets[portNum].pT4TS)           =
        0x00000000;
    *((pnHandle->pnPtcpConfig)->devicePortOffsets[portNum].pInDelayResCtrl)     =
        0x00000000;
    /* set the seqId in delay req packet*/
    *((pnHandle->pnPtcpConfig)->devicePortOffsets[portNum].pSeqIdInDelayPacket) =
        (((pnHandle->pnPtcpConfig)->seqId << 8) & 0xFF00) | (((
                    pnHandle->pnPtcpConfig)->seqId >> 8) & 0x00FF);
}

void PN_PTCP_processDelayResponse(PN_Handle pnHandle, uint8_t portNum)
{
    ptcp_iDelayResp_struct_t    ptcp_iDelayResp_parsed;

    /* parse the delay resp and fup packet*/
    if(*((pnHandle->pnPtcpConfig)->devicePortOffsets[portNum].pInDelayResCtrl) ==
            1)/*resp without FUP*/
        PN_PTCP_parseInDelayResp(&ptcp_iDelayResp_parsed,
                                 ((pnHandle->pnPtcpConfig)->devicePortOffsets[portNum].pInDelayResPacket),
                                 ((pnHandle->pnPtcpConfig)->devicePortOffsets[portNum].pInDelayFupResPacket), 0);

    else if(*((pnHandle->pnPtcpConfig)->devicePortOffsets[portNum].pInDelayResCtrl)
            == 6)/*resp with FUP and also FUP received*/
        PN_PTCP_parseInDelayResp(&ptcp_iDelayResp_parsed,
                                 ((pnHandle->pnPtcpConfig)->devicePortOffsets[portNum].pInDelayResPacket),
                                 ((pnHandle->pnPtcpConfig)->devicePortOffsets[portNum].pInDelayFupResPacket), 1);

    else                            /* no response received or double resp recv or other error combination*/
    {
        (pnHandle->pnPtcpConfig)->deviceDelays[portNum].cDelays[(pnHandle->pnPtcpConfig)->delayIndex]
            = 0;
        (pnHandle->pnPtcpConfig)->T2_prev[portNum]                              =
            0;    /*discard the previous values for next cable delay measurement*/
#ifdef PTCP_DEBUG
        ((pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugDelay[portNum]).cDelay[(pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugDelayIndex]
            = -1;
#endif
        return;
    }

    /* update timestamps*/
    ptcp_iDelayResp_parsed.T1TimeStamp = *((
            pnHandle->pnPtcpConfig)->devicePortOffsets[portNum].pT1TS);
    ptcp_iDelayResp_parsed.T4TimeStamp = *((
            pnHandle->pnPtcpConfig)->devicePortOffsets[portNum].pT4TS);
    ptcp_iDelayResp_parsed.T1_cycle_ctr = *((
            pnHandle->pnPtcpConfig)->devicePortOffsets[portNum].pT1CycleCtr);
    ptcp_iDelayResp_parsed.T4_cycle_ctr = *((
            pnHandle->pnPtcpConfig)->devicePortOffsets[portNum].pT4CycleCtr);
    ptcp_iDelayResp_parsed.T1PortTXDelay = PORT_TX_DELAY;
    ptcp_iDelayResp_parsed.T4PortRXDelay = PORT_RX_DELAY;

    /* caculate line delay, rcf_peer and cable delay*/
    PN_PTCP_lineDelayCalc(pnHandle, &ptcp_iDelayResp_parsed);
    PN_PTCP_cableDelayCalc(pnHandle, &ptcp_iDelayResp_parsed, portNum);

    (pnHandle->pnPtcpConfig)->deviceDelays[portNum].cDelays[(pnHandle->pnPtcpConfig)->delayIndex]
        = ptcp_iDelayResp_parsed.cable_delay;
    (pnHandle->pnPtcpConfig)->deviceDelays[portNum].lDelays[(pnHandle->pnPtcpConfig)->delayIndex]
        = ptcp_iDelayResp_parsed.line_delay;
    (pnHandle->pnPtcpConfig)->portTimes[portNum].rxDelayRemote =
        ptcp_iDelayResp_parsed.T2PortRXDelay;
    (pnHandle->pnPtcpConfig)->portTimes[portNum].txDelayRemote =
        ptcp_iDelayResp_parsed.T3PortTXDelay;

#ifdef PTCP_DEBUG
    /*for debug purposes*/
    ((pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugDelay[portNum]).cDelay[(pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugDelayIndex]
        = ptcp_iDelayResp_parsed.cable_delay;
    ((pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugDelay[portNum]).lDelay[(pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugDelayIndex]
        = ptcp_iDelayResp_parsed.line_delay;
    ((pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugDelay[portNum]).rcfPeer[(pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugDelayIndex]
        = ptcp_iDelayResp_parsed.rcf_peer;
    ((pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugDelay[portNum]).reqDelay[(pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugDelayIndex]
        = ptcp_iDelayResp_parsed.reqDelay;
    ((pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugDelay[portNum]).resDelay[(pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugDelayIndex]
        = ptcp_iDelayResp_parsed.resDelay;
    ((pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugDelay[portNum]).seqId[(pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugDelayIndex]
        = ptcp_iDelayResp_parsed.seqId;
#endif

#ifdef PTCP_DEBUG_LI

    if((pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->debugSyncIndex > 50
            && portNum == 0)
    {
        if(ptcp_iDelayResp_parsed.cable_delay >
                (pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->maxDelay)
        {
            (pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->maxDelay =
                ptcp_iDelayResp_parsed.cable_delay;
        }

        if(ptcp_iDelayResp_parsed.cable_delay <
                (pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->minDelay)
        {
            (pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->minDelay =
                ptcp_iDelayResp_parsed.cable_delay;
        }

        if(ptcp_iDelayResp_parsed.cable_delay > 100)
        {
            (pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->delayOutOfRange++;
        }
    }

#endif
}
void PN_PTCP_smaDelayMeasurement(PN_Handle pnHandle)
{
    uint32_t  portNum;
    uint32_t  measurementTimeout = (((INTER_DEL_REQ_BURST_GAP_MS-INTER_DEL_REQ_GAP_MS) * 1000) /
                                    Clock_tickPeriod) + 1;
    uint8_t linkStatus = 0;
    PN_PTCP_enableIsr(pnHandle);
    PN_PTCP_start(pnHandle);

    SemaphoreP_pend((pnHandle->pnPtcpConfig)->ptcpStartSem,
                    SemaphoreP_WAIT_FOREVER); /* wait for trigger to start measurement*/

    do
    {
        PN_PTCP_delayMeasurement(pnHandle);


        for(portNum = ICSS_EMAC_PORT_1 - 1; portNum < PTCP_NUM_PORTS; portNum++)
        {
            linkStatus = ((ICSS_EmacObject *)
                          pnHandle->emacHandle->object)->linkStatus[portNum];

            if((linkStatus == 0)
                    || (pnHandle->pnPtcpConfig)->currentPtcpStatus->cDelayEnable[portNum] ==
                    disable)
            {
                (pnHandle->pnPtcpConfig)->portTimes[portNum].cableDelay = 0;
            }

            else
            {
                PN_PTCP_portDelaySmaCalc(pnHandle, portNum);
            }

            /* inform stack about new measurement update*/
            if((pnHandle->pnPtcpConfig)->currentPtcpStatus->cDelay[portNum] !=
                    (pnHandle->pnPtcpConfig)->portTimes[portNum].cableDelay)
            {
                (pnHandle->pnPtcpConfig)->currentPtcpStatus->cDelay[portNum] =
                    (pnHandle->pnPtcpConfig)->portTimes[portNum].cableDelay;

                if((pnHandle->pnPtcpConfig)->ptcpDelayUpdateCall != NULL)
                {
                    (pnHandle->pnPtcpConfig)->ptcpDelayUpdateCall(portNum,
                            (pnHandle->pnPtcpConfig)->portTimes[portNum].cableDelay);
                }
            }
        }

        if(SemaphoreP_pend((pnHandle->pnPtcpConfig)->ptcpTriggerMeasurementSem,
                           measurementTimeout))
        {
            PN_PTCP_resetDelayValues(pnHandle);
        }
    }
    while(1);
}

void PN_PTCP_portDelaySmaCalc(PN_Handle pnHandle, uint8_t portNum)
{
    uint32_t    lDelaySum = 0, cDelaySum = 0;
    uint32_t    i = 0;

    for(i = 0; i < NUM_DELAYS_SMA; i++)
    {
        lDelaySum = lDelaySum + ((
                                     pnHandle->pnPtcpConfig)->deviceDelays[portNum]).lDelays[i];
        cDelaySum = cDelaySum + ((
                                     pnHandle->pnPtcpConfig)->deviceDelays[portNum]).cDelays[i];
    }

    if((pnHandle->pnPtcpConfig)->firstDelayBurst == 1)
    {
        *((pnHandle->pnPtcpConfig)->devicePortOffsets[portNum].pSmaLineDelay) =
            lDelaySum / NUM_DELAYS_BURST;
        (pnHandle->pnPtcpConfig)->portTimes[portNum].cableDelay = cDelaySum /
                NUM_DELAYS_BURST;
    }

    else
    {
        *((pnHandle->pnPtcpConfig)->devicePortOffsets[portNum].pSmaLineDelay) =
            lDelaySum / NUM_DELAYS_SMA;
        (pnHandle->pnPtcpConfig)->portTimes[portNum].cableDelay = cDelaySum /
                NUM_DELAYS_SMA;
    }

    (pnHandle->pnPtcpConfig)->firstDelayBurst = 0;
}

int32_t PN_PTCP_lineDelayCalc(PN_Handle pnHandle,
                              ptcp_iDelayResp_struct_t *ptcp_port_desc)
{
    int32_t ctr_diff;
    ctr_diff = (ptcp_port_desc->T4_cycle_ctr) - (ptcp_port_desc->T1_cycle_ctr);

    if(ctr_diff < 0)
    {
        ctr_diff = ctr_diff + 512;
    }

    ptcp_port_desc->reqDelay = ctr_diff * (pnHandle->pnPtcpConfig)->pnCyclePeriod;
    ptcp_port_desc->reqDelay = (int32_t)(ptcp_port_desc->reqDelay) + (int32_t)(
                                   ptcp_port_desc->T4TimeStamp - ptcp_port_desc->T1TimeStamp);

    return 0;
}

int32_t PN_PTCP_parseInDelayResp(ptcp_iDelayResp_struct_t
                                 *ptcp_iDelayResp_parsed, uint8_t *ptcp_iDelayResp_packet,
                                 uint8_t *ptcp_iDelayFupResp_packet, int32_t w_FUP)
{
    ptcp_iDelayResp_parsed->seqId           =   PN_PTCP_rotUshort((uint16_t *)(
                ptcp_iDelayResp_packet + 28));
    ptcp_iDelayResp_parsed->resDelay        =   PN_PTCP_rotUint((uint32_t *)(
                ptcp_iDelayResp_packet + 32));
    ptcp_iDelayResp_parsed->T2PortRXDelay   =   PN_PTCP_rotUint((uint32_t *)(
                ptcp_iDelayResp_packet + 48));
    ptcp_iDelayResp_parsed->T3PortTXDelay   =   PN_PTCP_rotUint((uint32_t *)(
                ptcp_iDelayResp_packet + 52));
    ptcp_iDelayResp_parsed->T2TimeStamp     =   PN_PTCP_rotUint((uint32_t *)(
                ptcp_iDelayResp_packet + 60));

    if(w_FUP == 1)
    {
        if(ptcp_iDelayResp_parsed->seqId != PN_PTCP_rotUshort((uint16_t *)(
                    ptcp_iDelayFupResp_packet + 28)))
        {
            return -4;    /* incorrect FUP response*/
        }

        ptcp_iDelayResp_parsed->resDelay     = PN_PTCP_rotUint((uint32_t *)(
                ptcp_iDelayFupResp_packet + 32));
    }

    return 0;
}

int32_t PN_PTCP_cableDelayCalc(PN_Handle pnHandle,
                               ptcp_iDelayResp_struct_t *ptcpDelayRespParsed, uint8_t port)
{
    uint32_t temp;
    int32_t ctrDiff;
    int64_t numerator;
    numerator = ptcpDelayRespParsed->T2TimeStamp -
                (pnHandle->pnPtcpConfig)->T2_prev[port];

    if(numerator < 0)
    {
        numerator = numerator + 4294967296;    /*2^32*/
    }

    ctrDiff = (ptcpDelayRespParsed->T1_cycle_ctr -
               (pnHandle->pnPtcpConfig)->T1_CTR_prev[port]);

    ctrDiff = PN_PTCP_adjCtrDiff(pnHandle, ctrDiff);

    temp = ctrDiff * (pnHandle->pnPtcpConfig)->pnCyclePeriod;
    temp = temp + (int32_t)((int32_t)ptcpDelayRespParsed->T1TimeStamp - (int32_t)(
                                pnHandle->pnPtcpConfig)->T1_prev[port]);

    /* rcf_peer calculation */
    if((pnHandle->pnPtcpConfig)->T2_prev[port] ==
            0)                                /* for the first time*/
    {
        ptcpDelayRespParsed->rcf_peer = 1;
    }

    else if((((pnHandle->pnPtcpConfig)->seqId) % 5) == 1)
    {
        ptcpDelayRespParsed->rcf_peer = (pnHandle->pnPtcpConfig)->rcf_prev[port];
    }

    else
    {
        ptcpDelayRespParsed->rcf_peer = (float)(numerator) / (float)temp;
    }

    (pnHandle->pnPtcpConfig)->T2_prev[port] = ptcpDelayRespParsed->T2TimeStamp;
    (pnHandle->pnPtcpConfig)->T1_prev[port] = ptcpDelayRespParsed->T1TimeStamp;
    (pnHandle->pnPtcpConfig)->T1_CTR_prev[port] = ptcpDelayRespParsed->T1_cycle_ctr;
    (pnHandle->pnPtcpConfig)->rcf_prev[port] = ptcpDelayRespParsed->rcf_peer;

    /* cable_delay calculation */
    ptcpDelayRespParsed->resDelay_peer = (ptcpDelayRespParsed->resDelay) /
                                         (ptcpDelayRespParsed->rcf_peer);

    ptcpDelayRespParsed->cable_delay = (ptcpDelayRespParsed->reqDelay -
                                        ptcpDelayRespParsed->resDelay_peer - ptcpDelayRespParsed->T1PortTXDelay -
                                        ptcpDelayRespParsed->T2PortRXDelay - ptcpDelayRespParsed->T3PortTXDelay -
                                        ptcpDelayRespParsed->T4PortRXDelay) / 2;

    if(ptcpDelayRespParsed->reqDelay < ptcpDelayRespParsed->resDelay_peer +
            ptcpDelayRespParsed->T1PortTXDelay + ptcpDelayRespParsed->T2PortRXDelay +
            ptcpDelayRespParsed->T3PortTXDelay +
            ptcpDelayRespParsed->T4PortRXDelay)          /* Make sure that it should never occur*/
    {
        ptcpDelayRespParsed->cable_delay = 0;
    }

    if(ptcpDelayRespParsed->cable_delay >
            100000)    /*Discard the value completely(wire greater than 10 KM!!) Make sure that it should never occur*/
    {
#ifdef PTCP_DEBUG
        UART_printf("Wire greater that 10KM!! This should not happen");
#endif
        ptcpDelayRespParsed->cable_delay = 0;
    }

    ptcpDelayRespParsed->line_delay = (ptcpDelayRespParsed->cable_delay) +
                                      (ptcpDelayRespParsed->T3PortTXDelay) + (ptcpDelayRespParsed->T4PortRXDelay);

    /*ignore the measurement and use previous value, in case of clock change*/
    /* use the previous values even in case of phase counter reset*/
    if((pnHandle->pnPtcpConfig)->clkChangeNotifyDelay == 1
            || (pnHandle->pnPtcpConfig)->phaseCtrChange == 1)
    {
        (pnHandle->pnPtcpConfig)->T2_prev[port] =
            0;        /*don't use this data for next measurement of rcf*/
        /*completely discard this value*/
        ptcpDelayRespParsed->cable_delay = (pnHandle->pnPtcpConfig)->prev_cDelay[port];
        ptcpDelayRespParsed->line_delay = (pnHandle->pnPtcpConfig)->prev_lDelay[port];

        if((pnHandle->pnPtcpConfig)->phaseCtrChange == 1)
        {
            (pnHandle->pnPtcpConfig)->phaseCtrChange = 0;
        }
    }

    (pnHandle->pnPtcpConfig)->prev_cDelay[port] = ptcpDelayRespParsed->cable_delay;
    (pnHandle->pnPtcpConfig)->prev_lDelay[port] = ptcpDelayRespParsed->line_delay;
    return 0;
}

int32_t PN_PTCP_adjCtrDiff(PN_Handle pnHandle, int32_t ctrDiff)
{
    if(ctrDiff < 0)
    {
        return ctrDiff + MAX_CTR_VAL + (pnHandle->pnPtcpConfig)->mandCtrAdj;
    }

    else
    {
        return ctrDiff + (pnHandle->pnPtcpConfig)->mandCtrAdj;
    }
}

uint32_t PN_PTCP_rotUint(uint32_t *input)
{
    return ((*input & 0xFF000000) >> 24) | ((*input & 0x00FF0000) >> 8) | ((
                *input & 0x0000FF00) << 8) | ((*input & 0x000000FF) << 24);
}

uint16_t PN_PTCP_rotUshort(uint16_t *input)
{
    return ((*input & 0xFF00) >> 8) | ((*input & 0x00FF) << 8);
}


int32_t PN_PTCP_parseSyncFields(PN_Handle pnHandle,
                                volatile ptcpSyncInfo_t *ptcp_sync_parsed, uint8_t *sync_sblock)
{
    int32_t i;
    uint8_t syncInitFlag = *((pnHandle->pnPtcpConfig)->pSyncInitFlag);

    if((pnHandle->pnPtcpConfig)->currentPtcpStatus->firstSyncRcv == 0
            || (pnHandle->pnPtcpConfig)->masterChange == 1)
    {
        for(i = 0; i < 6; i++)
        {
            *(ptcp_sync_parsed->masterSA + i) = *(sync_sblock + 6 + i);
            (pnHandle->pnPtcpConfig)->currentPtcpStatus->masterSA[i] = *
                    (sync_sblock + 6 + i);

            *(Uint8 *)((pnHandle->pnPtcpConfig)->pSyncMasterMac + i) = *(Uint8 *)(
                        sync_sblock + 6 + i);
        }

        *((pnHandle->pnPtcpConfig)->pSyncInitFlag) = syncInitFlag | (1 << 1);
    }

    for(i = 0; i < 16; i++)
    {
        *(ptcp_sync_parsed->subdomainUUID + i) = *(sync_sblock + 12 + i);
    }

    return 0;
}


#ifdef ENABLE_LATCH_SUPPORT
void PN_PTCP_latchIsrHandler(UArg arg)
{
    uint64_t PTCP_time_tL;
    uint32_t tick_elapsed;
    int32_t time_elapsed ;
    PN_Handle pnHandle = (PN_Handle)arg;
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;

    PN_clearPruIRQ(emacBaseAddr, LATCH0_EVENT);


    /* Timing parameters of latch event and latch isr */
    HWREG(emacBaseAddr->prussIntcRegs + CSL_ICSSINTC_SICR) = LATCH0_EVENT;
    (pnHandle->pnPtcpConfig)->g_Latch.IEP_count_tL_R    = HWREG(
                emacBaseAddr->prussIepRegs + CSL_ICSSIEP_CAPR6_REG0);
    (pnHandle->pnPtcpConfig)->g_Latch.IEP_count_tL_F    = HWREG(
                emacBaseAddr->prussIepRegs + CSL_ICSSIEP_CAPF6_REG0);


    /*Redundant timer */
    Types_Timestamp64 result;
    Timestamp_get64(&result);
    (pnHandle->pnPtcpConfig)->g_Latch.tick_L = (uint64_t)result.lo;
    (pnHandle->pnPtcpConfig)->g_Latch.tick_L |= ((uint64_t)result.hi << 32);

    tick_elapsed = (((pnHandle->pnPtcpConfig)->g_Latch.tick_L -
                              (pnHandle->pnPtcpConfig)->g_Latch.tick_S) * 10) / 6;

    /* Absolute time calculation - start*/
    int32_t IEP_diff = (pnHandle->pnPtcpConfig)->g_Latch.IEP_count_tL_R -
                       (pnHandle->pnPtcpConfig)->g_Latch.IEP_count_tS;

    /* Find cycle counter difference so that calculated time_elapsed  matches with system clock difference*/
    int n_CycCntr, Curr_min_diff = SYNC_INTERVAL, CycCntr_elapsed = -1;
    int SCF = HWREGH(emacBaseAddr->dataRam0BaseAddr + RTC_SCF_OFFSET);

    for(n_CycCntr = 0; n_CycCntr <= 960; n_CycCntr = n_CycCntr + SCF)
    {
        int t_elap, diff;
        t_elap = IEP_diff + (n_CycCntr * 31250);

        if(t_elap < 0)
        {
            t_elap = 0;
        }

        if(t_elap > tick_elapsed)
        {
            diff = t_elap - tick_elapsed;
        }

        else
        {
            diff = tick_elapsed - t_elap;
        }

        if(diff <= Curr_min_diff)
        {
            CycCntr_elapsed = n_CycCntr;
            Curr_min_diff = diff;
        }
    }

    time_elapsed = IEP_diff + (CycCntr_elapsed * 31250);

    /* Add time_elapsed to PTCP time got from Sync frame */
    PTCP_time_tL = ((uint64_t)((pnHandle->pnPtcpConfig)->g_Latch.TorgSec) *
                             1000000000) + (pnHandle->pnPtcpConfig)->g_Latch.TorgNsec +
                            (pnHandle->pnPtcpConfig)->g_Latch.TDelay + time_elapsed;

    /* Copy to Timestamp structure */
    (pnHandle->pnPtcpConfig)->g_T1.Status       = 0;
    (pnHandle->pnPtcpConfig)->g_T1.SecondsHigh  = 0;
    (pnHandle->pnPtcpConfig)->g_T1.SecondsLow   = PTCP_time_tL / 1000000000;
    (pnHandle->pnPtcpConfig)->g_T1.Nanoseconds  = PTCP_time_tL % 1000000000;

#ifdef LATCH_DEBUG

    g_dbg_latch[index_dbg_api] = g_Latch;
    g_dbg_T1[index_dbg_api] = g_T1;

    if(index_dbg_api >= 1)
    {
        td1[index_dbg_api] = g_dbg_T1[index_dbg_api].Nanoseconds -
                             g_dbg_T1[index_dbg_api - 1].Nanoseconds;
        tickdiff[index_dbg_api] = ((g_dbg_latch[index_dbg_api].tick_L -
                                    g_dbg_latch[index_dbg_api - 1].tick_L) * 10) / 6;
    }

    index_dbg_api++;

    if(index_dbg_api >= LDBG_SZ)
    {
        index_dbg_api = 0;
    }
#endif

    return;
}
#endif
/* Absolute time API
 Params:
     LatchEn:
     1 - Triggered by latch input
     0 - Triggered by function call itself

     p_PNIO_TimeStamp: pointer to structure PNIO_TimeStamp

 Return val:
      0  - if device is in sync with PLC
      -1 - if out of sync
 */
int32_t PN_PTCP_getAbsoluteTime(PN_Handle pnHandle,
                                PNIO_TimeStamp *p_PNIO_TimeStamp, int32_t LatchEn)
{
    int32_t time_elapsed;
    uint32_t tick_elapsed;
    uint64_t PTCP_time_tL;
    int32_t IEP_diff;
    int SCF ;
    int t_elap, diff;
    int n_CycCntr, Curr_min_diff, CycCntr_elapsed;
    Types_Timestamp64 result;
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;

    if(0 == LatchEn)
    {

        (pnHandle->pnPtcpConfig)->g_Latch.IEP_count_fn  = HWREG(
                    emacBaseAddr->prussIepRegs + CSL_ICSSIEP_COUNT_REG0);

        /*Redundant timer */

        Timestamp_get64(&result);
        (pnHandle->pnPtcpConfig)->g_Latch.tick_fn = (uint64_t)result.lo;
        (pnHandle->pnPtcpConfig)->g_Latch.tick_fn |= ((uint64_t)result.hi << 32);

        tick_elapsed = (((pnHandle->pnPtcpConfig)->g_Latch.tick_fn -
                                  (pnHandle->pnPtcpConfig)->g_Latch.tick_S) * 10) / 6;

        /* Absolute time calculation - start*/
         IEP_diff = (pnHandle->pnPtcpConfig)->g_Latch.IEP_count_fn -
                           (pnHandle->pnPtcpConfig)->g_Latch.IEP_count_tS;

        /* Find cycle counter difference so that calculated time_elapsed matches with system clock difference*/
        Curr_min_diff = SYNC_INTERVAL;
        CycCntr_elapsed = -1;
        SCF = HWREGH(emacBaseAddr->dataRam0BaseAddr + RTC_SCF_OFFSET);

        for(n_CycCntr = 0; n_CycCntr <= 960; n_CycCntr = n_CycCntr + SCF)
        {
            t_elap = IEP_diff + (n_CycCntr * 31250);

            if(t_elap < 0)
            {
                t_elap = 0;
            }

            if(t_elap > tick_elapsed)
            {
                diff = t_elap - tick_elapsed;
            }

            else
            {
                diff = tick_elapsed - t_elap;
            }

            if(diff <= Curr_min_diff)
            {
                CycCntr_elapsed = n_CycCntr;
                Curr_min_diff = diff;
            }
        }

        time_elapsed = IEP_diff + (CycCntr_elapsed * 31250);

        /* Add time_elapsed to PTCP time got from Sync frame */
        PTCP_time_tL = ((uint64_t)((pnHandle->pnPtcpConfig)->g_Latch.TorgSec) *
                                 1000000000) + (pnHandle->pnPtcpConfig)->g_Latch.TorgNsec +
                                (pnHandle->pnPtcpConfig)->g_Latch.TDelay + time_elapsed;

        (pnHandle->pnPtcpConfig)->g_T1.Status = 0;
        (pnHandle->pnPtcpConfig)->g_T1.SecondsHigh = 0;
        (pnHandle->pnPtcpConfig)->g_T1.SecondsLow = PTCP_time_tL / 1000000000;
        (pnHandle->pnPtcpConfig)->g_T1.Nanoseconds = PTCP_time_tL % 1000000000;

#ifdef LATCH_DEBUG
        g_dbg_latch[index_dbg_api] = g_Latch;
        g_dbg_T1[index_dbg_api] = g_T1;

        if(index_dbg_api >= 1)
        {
            td1[index_dbg_api] = g_dbg_T1[index_dbg_api].Nanoseconds -
                                 g_dbg_T1[index_dbg_api - 1].Nanoseconds;
            tickdiff[index_dbg_api] = ((g_dbg_latch[index_dbg_api].tick_fn -
                                        g_dbg_latch[index_dbg_api - 1].tick_fn) * 10) / 6;
        }

        index_dbg_api++;

        if(index_dbg_api >= LDBG_SZ)
        {
            index_dbg_api = 0;
        }

#endif

    }

    /* Output time stamp and status */
    p_PNIO_TimeStamp->Nanoseconds = (pnHandle->pnPtcpConfig)->g_T1.Nanoseconds;
    p_PNIO_TimeStamp->SecondsHigh = (pnHandle->pnPtcpConfig)->g_T1.SecondsHigh;
    p_PNIO_TimeStamp->SecondsLow = (pnHandle->pnPtcpConfig)->g_T1.SecondsLow;
    p_PNIO_TimeStamp->Status = (pnHandle->pnPtcpConfig)->g_T1.Status;

    if((pnHandle->pnPtcpConfig)->currentPtcpStatus->syncState == IN_SYNC)
    {
        return 0;
    }

    else
    {
        return -1;
    }
}

#ifdef ENABLE_LATCH_SUPPORT
/* Setup latch ISR*/
int32_t PN_PTCP_latchSetupIsr(PN_Handle pnHandle)
{
    ICSSEMAC_Handle icssEmacHandle = pnHandle->emacHandle;
    ICSS_EmacBaseAddressHandle_T ICSS_EmacBaseAddressHandle = ((
                ICSSEMAC_HwAttrs *)icssEmacHandle->hwAttrs)->emacBaseAddrCfg;

    Hwi_Params hwiParams;
    Error_Block eb;

    Hwi_Params_init(&hwiParams);
    Error_init(&eb);
    /*Set capture to first event mode*/

    HWREG(ICSS_EmacBaseAddressHandle->prussIepRegs + CSL_ICSSIEP_CAP_CFG_REG) |=
        0x000000C0;

    hwiParams.arg = (UArg)pnHandle;
    hwiParams.eventId = ISR_LATCH_NUM;
    hwiParams.maskSetting = Hwi_MaskingOption_SELF;
    hwiParams.enableInt = FALSE;        /* enable later*/
    hwiParams.priority = ISR_LATCH_NUM;

    Hwi_create(ISR_LATCH_NUM_ARM, PN_PTCP_latchIsrHandler, &hwiParams, &eb);

    if(Error_check(&eb))
    {
        return -1;
    }

    return 0;
}

/* Setup latch ISR - needs emac handle*/
void PN_PTCP_latchInit(PN_Handle pnHandle)
{
    PN_PTCP_latchSetupIsr(pnHandle);
}
#endif
int32_t PN_PTCP_setupIsr(PN_Handle pnHandle)
{
    PN_IntAttrs *ptcpIntConfig = (pnHandle->pnIntConfig)->ptcpIntConfig;
    Hwi_Params hwiParams;
    Error_Block eb;
    Hwi_Params_init(&hwiParams);
    Error_init(&eb);

    /* setup Sync ISR*/
    hwiParams.arg = (UArg)(ptcpIntConfig->args);        /* EMAC LLD subsytem handle*/
    hwiParams.eventId = ptcpIntConfig->socEvId;
    hwiParams.maskSetting = Hwi_MaskingOption_SELF;
    hwiParams.enableInt = FALSE;        /* enable later*/
    hwiParams.priority = ptcpIntConfig->intPrio;

    Hwi_create(ptcpIntConfig->coreIntNum, ptcpIntConfig->isrFnPtr, &hwiParams, &eb);

    if(Error_check(&eb))
    {
        /* handle the error*/
        return -1;
    }
    return 0;
}

void PN_PTCP_timerHandler(void* arg)
{
    PN_Handle pnHandle = (PN_Handle)arg;
    PN_PTCP_syncHandling(pnHandle);
}


void PN_PTCP_isrHandler(void* arg)
{
    PN_Handle pnHandle = (PN_Handle)arg;
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;
    PN_IntAttrs *ptcpIntConfig = (pnHandle->pnIntConfig)->ptcpIntConfig;

    PN_PTCP_syncHandling(pnHandle);
#ifdef SOC_K2G
    PN_PtcpConfig *ptcpConfig = pnHandle->pnPtcpConfig;
    /*The processing shall be done once the cycle counter is updated. So we check this and
     * start the timer to complete the sync processing. For other SoCs we skip clearing the
     * interrupt and ISR will be triggered again to complete the sync handling */
    if((pnHandle->pnPtcpConfig)->cycleCtrInitPending !=
            0)      /*clear only if cycle initialization is done*/
    {
        TimerP_start(ptcpConfig->ptcpTimer->ptcpTimerHandle);
    }
    PN_clearPruIRQ(emacBaseAddr, ptcpIntConfig->pruIntNum);
#else
    if((pnHandle->pnPtcpConfig)->cycleCtrInitPending ==0)      /*clear only if cycle initialization is done*/
    {
        PN_clearPruIRQ(emacBaseAddr, ptcpIntConfig->pruIntNum);
    }
#endif
    return;
}

int32_t PN_PTCP_enableIsr(PN_Handle pnHandle)
{
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;
    PN_IntAttrs *ptcpIntConfig = (pnHandle->pnIntConfig)->ptcpIntConfig;
    /* clear pending PRU ISR flags*/
    PN_clearPruIRQ(emacBaseAddr, ptcpIntConfig->pruIntNum);
    /*enable IRQ*/
    Hwi_enableInterrupt(ptcpIntConfig->coreIntNum);

    PN_clearPruIRQ(emacBaseAddr, LATCH0_EVENT);
    Hwi_enableInterrupt(ISR_LATCH_NUM_ARM);


    return 0;
}

int32_t PN_PTCP_disableIsr(PN_Handle pnHandle)
{
    /*disable IRQ*/
    PN_IntAttrs *ptcpIntConfig = (pnHandle->pnIntConfig)->ptcpIntConfig;
    Hwi_disableInterrupt(ptcpIntConfig->coreIntNum);
    return 0;
}

void PN_PTCP_syncHandling(PN_Handle pnHandle)
{

    int32_t iep_counter = 0;
    int32_t adjDeltaT = 0;
    uint64_t ptcp_time;
    uint16_t    cycle_counter;
    uint32_t    current_iep_val, processing_delay, incoming_delay,
                raw_incoming_delay;
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;
    uint32_t PN_PTCP_Cyclecounter_in_ISR;
    uint32_t PN_PTCP_Cyclecounter_in_Sync;

    /* Absolute time API */
    Types_Timestamp64 result;
    Timestamp_get64(&result);
    pnHandle->pnPtcpConfig->g_Latch.tick_S = (uint64_t)result.lo;
    pnHandle->pnPtcpConfig->g_Latch.tick_S |= ((uint64_t)result.hi << 32);

    //PN_IntAttrs *ptcpIntConfig = (pnHandle->pnIntConfig)->ptcpIntConfig;
    uint8_t *temp = (uint8_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                                SYNC_CTRL_BYTE_OFFSET);
    uint8_t *pFupCtrlByte = (uint8_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                                        SYNC_W_FUP_CTRL_BYTE_OFFSET);
    uint32_t    *pFupDelay = (uint32_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                                          SYNC_FUP_DELAY_OFFSET);

    uint8_t ctrlByte = *temp;

    if(ctrlByte != 0)
    {
        PN_PTCP_syncPreprocess(pnHandle, ctrlByte);
        *temp = 0;
    }

    if(*pFupCtrlByte == 1)
    {
        *pFupCtrlByte = 0;
        *((pnHandle->pnPtcpConfig)->pSyncInDelayPlusLD) = (*((
                    pnHandle->pnPtcpConfig)->pSyncInDelayPlusLD))
                + PN_PTCP_rotUint((uint32_t *)pFupDelay);
    }

    int32_t send_clock_factor = HWREGH(emacBaseAddr->dataRam0BaseAddr +
                                       RTC_SCF_OFFSET);
    uint32_t    syncRxSOF = *((pnHandle->pnPtcpConfig)->pSyncRxSOF);
    uint32_t syncInDelayPlusLD = *((pnHandle->pnPtcpConfig)->pSyncInDelayPlusLD);
    uint8_t smaFactor = (pnHandle->pnPtcpConfig)->ptcpSyncFilterfactor;

#ifdef  SYNC_SYS_LOG
    uint16_t *pSyncSysLogInfo = (uint16_t *)((uint8_t *)syncSysLogFrame + 44);
    int32_t *pSyncSysLogInfo_32 = (int32_t *)((uint8_t *)syncSysLogFrame + 44);
#endif

    /* If cycle initialization didn't have for the first time due to boundary conditions, then initialization is done here*/
    /* assumption: interrupt latency less than (cycle time - 8 us)*/
    if((pnHandle->pnPtcpConfig)->cycleCtrInitPending == 1)
    {
        current_iep_val = HWREG(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_COUNT_REG0);

        /* 4 us boundary on both sides*/
        if((pnHandle->pnPtcpConfig)->pnCyclePeriod - current_iep_val > 4000
                && current_iep_val > 4000)
        {

            /* Cycle counter update guideline */
            uint32_t curr_cycle_counter     = HWREGH((uint16_t *)(
                                                  emacBaseAddr->dataRam0BaseAddr
                                                  + PTCP_PM_CYCLE_COUNTER_OFFSET));
            uint32_t new_cycle_counter = (pnHandle->pnPtcpConfig)->calculatedCycleCtr;
            uint32_t curr = curr_cycle_counter & 0x00007FFF;
            uint32_t new = new_cycle_counter & 0x00007FFF;

            if(new >= curr)
            {
                (pnHandle->pnPtcpConfig)->calculatedCycleCtr = new_cycle_counter;
            }

            if(new < curr)
            {
                (pnHandle->pnPtcpConfig)->calculatedCycleCtr = new_cycle_counter ^ 0x00008000;
            }

            (pnHandle->pnPtcpConfig)->cycleCtrInitPending = 0;
            *((pnHandle->pnPtcpConfig)->pPmCycleCounter) =
                (pnHandle->pnPtcpConfig)->calculatedCycleCtr;
            *((pnHandle->pnPtcpConfig)->pPmPhaseCounter) = ((
                        pnHandle->pnPtcpConfig)->calculatedCycleCtr / send_clock_factor) & 0x1FF;
            (pnHandle->pnPtcpConfig)->phaseCtrChange = 1;

#ifdef SYNC_SYS_LOG
            syncSysLogFrame[42] = 0x43;
            syncSysLogFrame[43] = 0x43;
            *pSyncSysLogInfo = calculatedCycleCtr;
            *(pSyncSysLogInfo + 4) = 2;
            TxPacketOS(syncSysLogFrame, ICSS_EMAC_PORT_1, ICSS_EMAC_QUEUE1,
                       sizeof(syncSysLogFrame));
#endif
        }

        return;
    }

    PN_PTCP_parseSyncFields(pnHandle, (pnHandle->pnPtcpConfig)->deviceSyncInfo,
                            (uint8_t *)(emacBaseAddr->sharedDataRamBaseAddr + SYNC_SBLOCK_OFFSET));


    if((pnHandle->pnPtcpConfig)->currentPtcpStatus->firstSyncRcv == 1
            && (pnHandle->pnPtcpConfig)->initPmCycleCtrDone == 0)
    {
        /*cycle counter => 31.25*/
        /*cycle         => 31.25 * scf*/
        /*phase     => 31.25 * scf % 512 => cycle counter /scf % 512*/

        /*calculate cycle counter*/

        /* extract the fields from sync packet*/
        ptcp_time = PN_PTCP_rotUint((pnHandle->pnPtcpConfig)->pSyncTorgSecs);
        ptcp_time *= 1000000000;
        ptcp_time += PN_PTCP_rotUint((pnHandle->pnPtcpConfig)->pSyncTorgNs);

        /* take care of processing delay: interrupt latency, computation delay, etc.*/
        current_iep_val = HWREG(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_COUNT_REG0);


        /* Difference in cycle counter used to calculate Sync to ISR latency. Handles multiple wraparounds.*/
        PN_PTCP_Cyclecounter_in_ISR = HWREGH(emacBaseAddr->dataRam0BaseAddr +
                                             RTC_CYCLE_COUNTER_OFFSET);
        PN_PTCP_Cyclecounter_in_Sync = HWREGH(emacBaseAddr->sharedDataRamBaseAddr +
                                              SYNC_CYCLE_COUNTER);

        processing_delay = (PN_PTCP_Cyclecounter_in_ISR - PN_PTCP_Cyclecounter_in_Sync)
                           * RTC_3125_CLK_CONST  - syncRxSOF + current_iep_val;


        /* take incoming delay: only consider the part which is multiple of pn_cycle_period*/
        raw_incoming_delay = syncInDelayPlusLD + processing_delay;
        incoming_delay = raw_incoming_delay / (pnHandle->pnPtcpConfig)->pnCyclePeriod;
        incoming_delay *= (pnHandle->pnPtcpConfig)->pnCyclePeriod;

        /* add incoming delay to ptcp_time*/
        ptcp_time += incoming_delay;

        /* calculate cycle counter*/
        cycle_counter = (ptcp_time / 31250) & 0xFFFF; /* % 65536*/

        /* take more accurate timestamp*/
        current_iep_val = HWREG(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_COUNT_REG0);

        /* 4 us boundary on both sides => 4 us empirical value based on assumption that firmware updation of cycle counter will be done by 4 us.*/
        if((pnHandle->pnPtcpConfig)->pnCyclePeriod - current_iep_val < 4000
                || current_iep_val <
                4000) /* less than 4 us; store the values and do adjustment later to avoid race condition*/
        {
            (pnHandle->pnPtcpConfig)->cycleCtrInitPending = 1;

            if((raw_incoming_delay % (pnHandle->pnPtcpConfig)->pnCyclePeriod) <
                    4000)   /* calculated value => already new value*/
            {
                (pnHandle->pnPtcpConfig)->calculatedCycleCtr  = cycle_counter;
            }

            else
            {
                (pnHandle->pnPtcpConfig)->calculatedCycleCtr  = cycle_counter +
                        send_clock_factor;
            }
        }

        else
        {
            /* Cycle counter update guideline */
            uint32_t curr_cycle_counter     = HWREGH((uint16_t *)(
                                                  emacBaseAddr->dataRam0BaseAddr
                                                  + PTCP_PM_CYCLE_COUNTER_OFFSET));
            uint32_t new_cycle_counter = cycle_counter;
            uint32_t curr = curr_cycle_counter & 0x00007FFF;
            uint32_t new = new_cycle_counter & 0x00007FFF;

            if(new >= curr)
            {
                cycle_counter = new_cycle_counter;
            }

            if(new < curr)
            {
                cycle_counter = new_cycle_counter ^ 0x00008000;
            }

            *((pnHandle->pnPtcpConfig)->pPmCycleCounter) =
                cycle_counter;               /*exit from here and come back here...before clearing..use flag just for updating...and update late*/
            *((pnHandle->pnPtcpConfig)->pPmPhaseCounter) = (cycle_counter /
                    send_clock_factor) & 0x1FF;
            (pnHandle->pnPtcpConfig)->phaseCtrChange = 1;
#ifdef SYNC_SYS_LOG
            syncSysLogFrame[42] = 0x43;
            syncSysLogFrame[43] = 0x43;
            *pSyncSysLogInfo = cycle_counter;
            *(pSyncSysLogInfo + 4) = 1;
            TxPacketOS(syncSysLogFrame, ICSS_EMAC_PORT_1, ICSS_EMAC_QUEUE1,
                       sizeof(syncSysLogFrame));
#endif
        }

        (pnHandle->pnPtcpConfig)->initPmCycleCtrDone = 1;
    }

    /* Absolute time API */


    pnHandle->pnPtcpConfig->g_Latch.TorgSec             = PN_PTCP_rotUint((
                pnHandle->pnPtcpConfig)->pSyncTorgSecs);
    pnHandle->pnPtcpConfig->g_Latch.TorgNsec            = PN_PTCP_rotUint((
                pnHandle->pnPtcpConfig)->pSyncTorgNs);
    pnHandle->pnPtcpConfig->g_Latch.TDelay              = *((
                pnHandle->pnPtcpConfig)->pSyncInDelayPlusLD);
    pnHandle->pnPtcpConfig->g_Latch.IEP_count_tS        = *((
                pnHandle->pnPtcpConfig)->pSyncRxSOF);

    int32_t masterT = ((PN_PTCP_rotUint((pnHandle->pnPtcpConfig)->pSyncTorgNs) %
                        (pnHandle->pnPtcpConfig)->pnCyclePeriod) +
                       ((*((pnHandle->pnPtcpConfig)->pSyncInDelayPlusLD)) %
                        (pnHandle->pnPtcpConfig)->pnCyclePeriod)) % ((
                                    pnHandle->pnPtcpConfig)->pnCyclePeriod);
    int32_t localT  = (*((pnHandle->pnPtcpConfig)->pSyncRxSOF)) % ((
                          pnHandle->pnPtcpConfig)->pnCyclePeriod);

    int32_t deltaT = masterT - localT;
    int32_t revDeltaT = (pnHandle->pnPtcpConfig)->pnCyclePeriod - PN_PTCP_absVal(
                            deltaT);

    if(revDeltaT < PN_PTCP_absVal(deltaT)
            && (pnHandle->pnPtcpConfig)->currentPtcpStatus->firstSyncRcv != 0)
    {
        if(deltaT > 0)
        {
            deltaT = (-1) * revDeltaT;
        }

        else
        {
            deltaT = revDeltaT;
        }
    }

    int32_t tempSum = 0;
    int32_t j = 0;

    for(j = 0; j < smaFactor; j++)
    {
        tempSum = tempSum + (pnHandle->pnPtcpConfig)->syncDeltaTs[j];
    }

    int32_t smaDeltaT = tempSum / smaFactor;

    if((pnHandle->pnPtcpConfig)->currentPtcpStatus->firstSyncRcv == 0)
    {
        /* check whether transmission is going on or not*/
        iep_counter = HWREG(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_COUNT_REG0);
        HWREG(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_COUNT_REG0) = PN_PTCP_modFunc(
                    iep_counter + deltaT + 900, (pnHandle->pnPtcpConfig)->pnCyclePeriod);
        deltaT = 0;
        adjDeltaT = 0;
    }

    else
    {
#ifdef PTCP_ENABLE_FILTER

        if((pnHandle->pnPtcpConfig)->numInSync < 1 *
                smaFactor)      /*1: let it stabilize, before using the filter*/
        {
            adjDeltaT = deltaT;
        }

        else if((pnHandle->pnPtcpConfig)->numInSync == smaFactor)
        {
            adjDeltaT = deltaT + smaDeltaT;
        }

        else
        {
            adjDeltaT = (deltaT / smaFactor) + smaDeltaT;
        }

        if((pnHandle->pnPtcpConfig)->numInSync !=
                smaFactor)     /*Special case: at the time of transition(non filter to filter)*/
        {
            (pnHandle->pnPtcpConfig)->syncDeltaTs[(pnHandle->pnPtcpConfig)->syncIndex] =
                adjDeltaT;
        }

#else
        adjDeltaT = deltaT;
#endif

        (pnHandle->pnPtcpConfig)->numInSync++;
        (pnHandle->pnPtcpConfig)->syncIndex = ((pnHandle->pnPtcpConfig)->syncIndex + 1)
                                              % smaFactor;
    }

    /*calculation of ecap period
    adjust adjDeltaT ns     in      30 ms = 30 000 us
    adjust      1        ns     in      (30 000/adjDeltaT) us*/

    int32_t cmpValue = 0;

    cmpValue = (0x5 << CSL_ICSSIEP_GLOBAL_CFG_REG_DEFAULT_INC_SHIFT) |
               CSL_ICSSIEP_GLOBAL_CFG_REG_CNT_ENABLE_MAX;

    if(adjDeltaT == 0)
    {
        cmpValue |= (0x5 << CSL_ICSSIEP_GLOBAL_CFG_REG_CMP_INC_SHIFT);
        PN_PTCP_syncIepAdjustment(pnHandle, SYNC_INTERVAL, cmpValue);
    }

    else
    {
        int32_t ecapPeriod = 5 * (SYNC_INTERVAL / PN_PTCP_absVal(
                                      adjDeltaT)); /* TODO Verify if double compensation happens on AM571x*/

        if(adjDeltaT > 0)                      /* master is faster*/
        {
            cmpValue |= (0xA << CSL_ICSSIEP_GLOBAL_CFG_REG_CMP_INC_SHIFT);
            PN_PTCP_syncIepAdjustment(pnHandle, ecapPeriod, cmpValue);
        }

        else                                    /* slave is faster*/
        {
            cmpValue |= (0x0 << CSL_ICSSIEP_GLOBAL_CFG_REG_CMP_INC_SHIFT);
            PN_PTCP_syncIepAdjustment(pnHandle, ecapPeriod, cmpValue);
        }
    }

#ifdef  SYNC_ANALYSIS

    if(nResets < SYNC_ANALYSIS_N_RESETS - 1)
    {
        if(debugSyncIndex < SYNC_SINGLE_ITER_SIZE - 1)
        {
            ((syncAnalysis)[nResets]).deltaT[debugSyncIndex]    = deltaT;
            ((syncAnalysis)[nResets]).smaDeltaT[debugSyncIndex] = smaDeltaT;
            ((syncAnalysis)[nResets]).seqId[debugSyncIndex]     = PN_PTCP_rotUshort((
                        uint16_t *)pSeqId);
            ((syncAnalysis)[nResets]).cycleTime[debugSyncIndex] = pn_cycle_period;
        }
    }

#endif

    if((pnHandle->pnPtcpConfig)->numInSync > 0)
    {
        if((pnHandle->pnPtcpConfig)->pnPtcpDebugAttrs->debugSyncIndex <
                DEBUG_SYNC_N_ITER - 1)
        {

#ifdef SYNC_SYS_LOG_1
            syncSysLogFrame[42] = 0x44;
            syncSysLogFrame[43] = 0x54;
            *pSyncSysLogInfo_32 = deltaT;
            *(pSyncSysLogInfo_32 + 1) = masterT;
            *(pSyncSysLogInfo_32 + 2) = localT;
            TxPacketOS(syncSysLogFrame, ICSS_EMAC_PORT_1, ICSS_EMAC_QUEUE1,
                       sizeof(syncSysLogFrame));
#endif

#ifdef PTCP_DEBUG_SI
            (debugSync).syncDeltaT[debugSyncIndex]      = deltaT;
            (debugSync).syncSmaDeltaT[debugSyncIndex]   = smaDeltaT;

            (debugSync).syncTorgT[debugSyncIndex]           = PN_PTCP_rotUint(pSyncTorgNs);
            (debugSync).syncInDelayPlusLDT[debugSyncIndex]  = *pSyncInDelayPlusLD;

            (debugSync).syncMasterT[debugSyncIndex]     = masterT;
            (debugSync).syncLocalT[debugSyncIndex]      = localT;
#endif
            (pnHandle->pnPtcpConfig)->pnPtcpDebugAttrs->debugSyncIndex++;
        }

#ifdef PTCP_DEBUG_LI

        if(deltaT > (pnHandle->pnPtcpConfig)->pnPtcpDebugAttrs->maxDeltaT)
        {
            (pnHandle->pnPtcpConfig)->pnPtcpDebugAttrs->maxDeltaT = deltaT;
            (pnHandle->pnPtcpConfig)->maxSeqId = PN_PTCP_rotUshort((uint16_t *)(
                    emacBaseAddr->sharedDataRamBaseAddr + SYNC_SEQID_OFFSET));
        }

        if(deltaT < (pnHandle->pnPtcpConfig)->pnPtcpDebugAttrs->minDeltaT)
        {
            (pnHandle->pnPtcpConfig)->pnPtcpDebugAttrs->minDeltaT = deltaT;
            (pnHandle->pnPtcpConfig)->minSeqId = PN_PTCP_rotUshort((uint16_t *)(
                    emacBaseAddr->sharedDataRamBaseAddr + SYNC_SEQID_OFFSET));
        }

        if(PN_PTCP_absVal(deltaT) > 1000)
        {
            (pnHandle->pnPtcpConfig)->pnPtcpDebugAttrs->deltaTOutOfRange++;
        }

#endif

    }

    if(PN_PTCP_absVal(deltaT) >
            (pnHandle->pnPtcpConfig)->currentPtcpStatus->syncPllWnd ||
            (pnHandle->pnPtcpConfig)->currentPtcpStatus->firstSyncRcv == 0 ||
            (pnHandle->pnPtcpConfig)->numInSync <= smaFactor +
            2) /*outside PLL window ; let filter get stabilized*/
    {
        (pnHandle->pnPtcpConfig)->deviceSyncInfo->syncState = OUT_OF_SYNC;
    }

    else
    {
        (pnHandle->pnPtcpConfig)->deviceSyncInfo->syncState = IN_SYNC;
    }

    /*update the flags*/
    (pnHandle->pnPtcpConfig)->currentPtcpStatus->syncRcv = 1;
    (pnHandle->pnPtcpConfig)->currentPtcpStatus->firstSyncRcv = 1;

    if((pnHandle->pnPtcpConfig)->currentPtcpStatus->syncState !=
            (pnHandle->pnPtcpConfig)->deviceSyncInfo->syncState ||
            (pnHandle->pnPtcpConfig)->masterChange == 1)
    {
#ifdef PTCP_SYNC_SIGNAL
        GPIO_write(0, (pnHandle->pnPtcpConfig)->deviceSyncInfo->syncState & 0x1);
#endif
        HWREGB(emacBaseAddr->dataRam0BaseAddr + RTC_DEVICE_SYNC_STATUS_OFFSET) =
            (pnHandle->pnPtcpConfig)->deviceSyncInfo->syncState;

        (pnHandle->pnPtcpConfig)->currentPtcpStatus->syncState =
            (pnHandle->pnPtcpConfig)->deviceSyncInfo->syncState;

        if((pnHandle->pnPtcpConfig)->ptcpSyncStatusCall != NULL)
        {
            (pnHandle->pnPtcpConfig)->ptcpSyncStatusCall((
                        pnHandle->pnPtcpConfig)->currentPtcpStatus->syncState, (uint32_t)NULL);
        }

#ifdef SYNC_SYS_LOG

        if(currentPtcpStatus.syncState == IN_SYNC)
        {
            syncSysLogFrame[42] = 0x49;
            syncSysLogFrame[43] = 0x4E;
            syncSysLogFrame[44] = 0x20;

        }

        else
        {
            syncSysLogFrame[42] = 0x4F;
            syncSysLogFrame[43] = 0x55;
            syncSysLogFrame[44] = 0x54;
        }

        TxPacket(syncSysLogFrame, ICSS_EMAC_PORT_1, QUEPRIO1, sizeof(syncSysLogFrame));
#endif

        if((pnHandle->pnPtcpConfig)->masterChange == 1)
        {
            (pnHandle->pnPtcpConfig)->masterChange = 0;
        }

#ifdef  SYNC_ANALYSIS

        if(nResets < SYNC_ANALYSIS_N_RESETS - 1)
        {
            if(nSyncTrans < SYNC_ANALYSIS_N_ITER - 1)
            {
                ((syncAnalysis)[nResets]).tState[nSyncTrans]        = deviceSyncInfo.syncState;
                ((syncAnalysis)[nResets]).tSeqId[nSyncTrans]        = PN_PTCP_rotUshort((
                            uint16_t *)pSeqId);
                nSyncTrans++;
            }
        }

#endif
    }
}

void PN_PTCP_syncIepAdjustment(PN_Handle pnHandle, int32_t ecapPeriod,
                               uint32_t compensation)
{
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;

    if((pnHandle->pnPtcpConfig)->ptcpEnableSlowCompensation)
    {
        HWREG(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_GLOBAL_CFG_REG)=compensation;
        HWREG(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_SLOW_COMPEN_REG)=ecapPeriod/5;
    }
    else
    {
        HWREG(emacBaseAddr->l3OcmcBaseAddr + IEP_CONFIG_ADJ_OFFSET + 0) = compensation;
        PN_PTCP_reconfigEcap(pnHandle, ecapPeriod);
    }


}

/* configure the eCAP0 timer in
 1. sync packet received => deltaT calculation => write 0x0451 /0x0651 in a specific memory location
 2. calculate period for ECAP(i.e. update period) => write ecap period and start ecap
 3. ecap will periocally generate interrupt => EDMA is triggered => writes 451/651 in IEP register and clear ecap bits
 repeate step 1 to 3*/

/**
  * Initialize the ECAP for the first time
  */
void PN_PTCP_initEcap(PN_Handle pnHandle)
{
    /* Putting ECAP in APWM mode, stop TSCTR if running */
    uint32_t pwmssBaseAddress = (pnHandle->pnPtcpConfig)->ecapPwmssBaseAddr;
    HW_WR_FIELD16(pwmssBaseAddress + CSL_ECAP_ECCTL2,
                  CSL_ECAP_ECCTL2_CAP_APWM, CSL_ECAP_ECCTL2_CAP_APWM_APWM_MODE);

    HW_WR_FIELD16(pwmssBaseAddress + CSL_ECAP_ECCTL2,
                  CSL_ECAP_ECCTL2_STOP_WRAP, CSL_ECAP_ECCTL2_STOP_WRAP_CAPT_EVT4);

    /* Set TSCTR to 0 */
    HW_WR_FIELD32(pwmssBaseAddress + CSL_ECAP_TSCTR,
                  CSL_ECAP_TSCTR_TSCTR, CSL_ECAP_TSCTR_TSCTR_RESETVAL);

    /*Load 2560 ns , 2.56 us Tick Time, 200 = 2560/5 = 512 =hex 200*/
    HW_WR_FIELD32(pwmssBaseAddress + CSL_ECAP_CAP1,
                  CSL_ECAP_CAP1_CAP1, (250000 / 10 / 5 - 1));

    /* Clear the Period Event Flag      0x00c1*/
    HW_WR_FIELD16(pwmssBaseAddress + CSL_ECAP_ECCLR,
                  CSL_ECAP_ECCLR_INT, CSL_ECAP_ECCLR_INT_CLEAR);

    HW_WR_FIELD16(pwmssBaseAddress + CSL_ECAP_ECCLR,
                  CSL_ECAP_ECCLR_PRDEQ, CSL_ECAP_ECCLR_PRDEQ_CLEAR);

    HW_WR_FIELD16(pwmssBaseAddress + CSL_ECAP_ECCLR,
                  CSL_ECAP_ECCLR_CMPEQ, CSL_ECAP_ECCLR_CMPEQ_CLEAR);

    /* Enable Period as source of interrupt */
    HW_WR_FIELD16(pwmssBaseAddress + CSL_ECAP_ECEINT,
                  CSL_ECAP_ECEINT_PRDEQ, CSL_ECAP_ECEINT_PRDEQ_ENABLE);
}


/**
  * Reconfigure the ECAP after calculating new period based on sync deltaT calculation
  */

void PN_PTCP_reconfigEcap(PN_Handle pnHandle, int32_t period)
{
    uint32_t pwmssBaseAddr = (pnHandle->pnPtcpConfig)->ecapPwmssBaseAddr;
    /* Stop the counter */
    HW_WR_FIELD16(pwmssBaseAddr + CSL_ECAP_ECCTL2,
                  CSL_ECAP_ECCTL2_TSCTRSTOP, CSL_ECAP_ECCTL2_TSCTRSTOP_STOPPED);

    /* Set TSCTR to 0 */
    HW_WR_FIELD32(pwmssBaseAddr + CSL_ECAP_TSCTR,
                  CSL_ECAP_TSCTR_TSCTR, CSL_ECAP_TSCTR_TSCTR_RESETVAL);

    /* CRYPTO: check whether we can use shadow registers or not?
       set the period */
    HW_WR_FIELD32(pwmssBaseAddr + CSL_ECAP_CAP1,
                  CSL_ECAP_CAP1_CAP1, (((period / 5) / 2) - 1));

    /* Clear the Period Event Flag      0x00c1*/
    HW_WR_FIELD16(pwmssBaseAddr + CSL_ECAP_ECCLR,
                  CSL_ECAP_ECCLR_INT, CSL_ECAP_ECCLR_INT_CLEAR);

    HW_WR_FIELD16(pwmssBaseAddr + CSL_ECAP_ECCLR,
                  CSL_ECAP_ECCLR_PRDEQ, CSL_ECAP_ECCLR_PRDEQ_CLEAR);

    HW_WR_FIELD16(pwmssBaseAddr + CSL_ECAP_ECCLR,
                  CSL_ECAP_ECCLR_CMPEQ, CSL_ECAP_ECCLR_CMPEQ_CLEAR);

    /* Start the counter */
    HW_WR_FIELD16(pwmssBaseAddr + CSL_ECAP_ECCTL2,
                  CSL_ECAP_ECCTL2_TSCTRSTOP, CSL_ECAP_ECCTL2_TSCTRSTOP_FREE_RUNNING);
}

void PN_PTCP_configEdma(PN_Handle pnHandle)
{
    EDMA3_DRV_Result edmaResult = 0;
    PN_PtcpConfig *ptcpConfig = pnHandle->pnPtcpConfig;
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;
    uint32_t ocmcBaseAddress = emacBaseAddr->l3OcmcBaseAddr;

    ptcpConfig->gEdmaHandle = (EDMA3_RM_Handle)edma3init(0, &edmaResult);

    if(edmaResult != EDMA3_DRV_SOK)
    {
        /* Report EDMA Error */
        System_printf("\nEDMA driver initialization FAIL\n");
        return;
    }

    else
    {
        System_printf("\nEDMA driver initialization PASS.\n");
    }

    HWREG(ocmcBaseAddress + IEP_CONFIG_ADJ_OFFSET + 0) = 0x00000551;
    HWREG(ocmcBaseAddress + IEP_CONFIG_ADJ_OFFSET + 4) = 0x00000000;
    HWREG(ocmcBaseAddress + IEP_CONFIG_ADJ_OFFSET + 8) = 0x00000001;

    ptcpConfig->edmaEcapEvt0tcc    = EDMA3_DRV_TCC_ANY;
    edmaResult = EDMA3_DRV_requestChannel(ptcpConfig->gEdmaHandle,
                                          &(ptcpConfig->edmaEcapEvt0ChNum), &(ptcpConfig->edmaEcapEvt0tcc),
                                          (EDMA3_RM_EventQueue)0,
                                          NULL, NULL);

    if(edmaResult == 0)
    {
        ptcpConfig->edmaEcapEvt0Linktcc    = EDMA3_DRV_TCC_ANY;
        edmaResult = EDMA3_DRV_requestChannel(ptcpConfig->gEdmaHandle,
                                              &(ptcpConfig->edmaEcapEvt0LinkChNum), &(ptcpConfig->edmaEcapEvt0Linktcc), 0,
                                              NULL, NULL);

    }

    else
    {
        /* Report EDMA Error */
        System_printf("\nEDMA driver reques FAIL\n");
        return;
    }

    if(edmaResult == 0)
    {
        ptcpConfig->edmaEcapCleartcc    = EDMA3_DRV_TCC_ANY;
        edmaResult = EDMA3_DRV_requestChannel(ptcpConfig->gEdmaHandle,
                                              &(ptcpConfig->edmaEcapClearChNum), &(ptcpConfig->edmaEcapCleartcc),
                                              (EDMA3_RM_EventQueue)0,
                                              NULL, NULL);
    }

    else
    {
        /* Report EDMA Error */
        System_printf("\nEDMA driver reques FAIL\n");
        return;
    }

    if(edmaResult == 0)
    {
        ptcpConfig->edmaEcapClearLinktcc    = EDMA3_DRV_TCC_ANY;
        edmaResult = EDMA3_DRV_requestChannel(ptcpConfig->gEdmaHandle,
                                              &(ptcpConfig->edmaEcapClearLinkChNum), &(ptcpConfig->edmaEcapClearLinktcc),
                                              (EDMA3_RM_EventQueue)0,
                                              NULL, NULL);
    }

    else
    {
        /* Report EDMA Error */
        System_printf("\nEDMA driver reques FAIL\n");
        return;
    }

    if(edmaResult == 0)
    {
        PN_PTCP_setIepEdmaPaRamEntry(pnHandle);
    }

    else
    {
        /* Report EDMA Error */
        System_printf("\nEDMA driver reques FAIL\n");
        return;
    }
}
void PN_PTCP_setIepEdmaPaRamEntry(PN_Handle pnHandle)
{
    PN_PtcpConfig *ptcpConfig = pnHandle->pnPtcpConfig;
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;
    uint32_t ocmcBaseAddress = emacBaseAddr->l3OcmcBaseAddr;
    int32_t result = EDMA3_DRV_SOK;
    EDMA3_DRV_PaRAMRegs paramSet   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    EDMA3_DRV_ChainOptions chain = {EDMA3_DRV_TCCHEN_DIS,
                                    EDMA3_DRV_ITCCHEN_DIS,
                                    EDMA3_DRV_TCINTEN_DIS,
                                    EDMA3_DRV_ITCINTEN_DIS
                                   };
    EDMA3_DRV_getPaRAM((EDMA3_DRV_Handle) ptcpConfig->gEdmaHandle,
                       (uint32_t)ptcpConfig->edmaEcapEvt0ChNum, &paramSet);

    HWREGH(ocmcBaseAddress + ECAP_CLR_CONFIG_OFFSET) =
        0x0041;  /* PRDEQ (6) and INT (0) bits*/
    paramSet.srcAddr    = (ocmcBaseAddress + IEP_CONFIG_ADJ_OFFSET);
    paramSet.destAddr = (uint32_t)emacBaseAddr->prussIepRegs;

    paramSet.aCnt    = 3 * 4;
    paramSet.bCnt    = 1;
    paramSet.cCnt    = 1;

    paramSet.srcBIdx    = 0;
    paramSet.srcCIdx    = 0;

    paramSet.destBIdx   = 0;
    paramSet.destCIdx   = 0;

    paramSet.bCntReload = (uint16_t) 0;
    paramSet.linkAddr = ptcpConfig->edmaEcapEvt0LinkChNum;
    paramSet.bCntReload = (uint16_t) 0;
    paramSet.linkAddr   = 0xFFFFu;
    paramSet.opt = 0x0u;
    /* Src is in INCR mode & Dest is in FIFO modes                            */

    paramSet.opt &= 0xFFFFFFFCu;
    /* Program the TCC */
    paramSet.opt |= (((ptcpConfig->edmaEcapEvt0tcc) << OPT_TCC_SHIFT) &
                     OPT_TCC_MASK);
    paramSet.opt &= 0xFFFFFFFBu;
    result = EDMA3_DRV_setPaRAM(ptcpConfig->gEdmaHandle,
                                ptcpConfig->edmaEcapEvt0ChNum, &paramSet);

    if(result == 0)
    {

        paramSet.srcAddr    = (ocmcBaseAddress + IEP_CONFIG_ADJ_OFFSET);
        paramSet.destAddr = (uint32_t)emacBaseAddr->prussIepRegs;
        paramSet.aCnt    = 3 * 4;
        paramSet.bCnt    = 1;
        paramSet.cCnt    = 1;

        paramSet.srcBIdx    = 0;
        paramSet.srcCIdx    = 0;

        paramSet.destBIdx   = 0;
        paramSet.destCIdx   = 0;

        paramSet.bCntReload = (uint16_t) 0;
        /* paramSet.linkAddr = EDMA3_ECAPEVT0_LINK_CHA_NUM;*/
        paramSet.bCntReload = (uint16_t) 0;
        paramSet.linkAddr   = 0xFFFFu;
        paramSet.opt = 0x0u;
        /* Src is in INCR mode & Dest is in FIFO modes                            */
        paramSet.opt &= 0xFFFFFFFCu;
        /* Program the TCC */
        paramSet.opt |= (((ptcpConfig->edmaEcapEvt0Linktcc) << OPT_TCC_SHIFT) &
                         OPT_TCC_MASK);
        paramSet.opt &= 0xFFFFFFFBu;
        result = EDMA3_DRV_setPaRAM(ptcpConfig->gEdmaHandle,
                                    ptcpConfig->edmaEcapEvt0LinkChNum, &paramSet);
    }

    else
    {
        /* Report EDMA Error */
        System_printf("\nEDMA driver setParam FAIL\n");
        return;
    }


    if(result == 0)
    {
        paramSet.srcAddr    = (uint32_t)(uint32_t)(ocmcBaseAddress +
                              ECAP_CLR_CONFIG_OFFSET);
        paramSet.destAddr = (uint32_t)(ptcpConfig->ecapPwmssBaseAddr + CSL_ECAP_ECCLR);
        paramSet.aCnt    = 2;
        paramSet.bCnt    = 1;
        paramSet.cCnt    = 1;

        paramSet.srcBIdx    = 0;
        paramSet.srcCIdx    = 0;

        paramSet.destBIdx   = 0;
        paramSet.destCIdx   = 0;

        paramSet.bCntReload = (uint16_t) 0;
        /* paramSet.linkAddr = EDMA3_ECAPEVT0_LINK_CHA_NUM;*/
        paramSet.bCntReload = (uint16_t) 0;
        paramSet.linkAddr   = 0xFFFFu;
        paramSet.opt = 0x0u;
        /* Src is in INCR mode & Dest is in FIFO modes                            */
        paramSet.opt &= 0xFFFFFFFCu;
        /* Program the TCC */
        paramSet.opt |= (((ptcpConfig->edmaEcapCleartcc) << OPT_TCC_SHIFT) &
                         OPT_TCC_MASK);

        paramSet.opt |= (1 << OPT_SYNCDIM_SHIFT);
        result = EDMA3_DRV_setPaRAM(ptcpConfig->gEdmaHandle,
                                    ptcpConfig->edmaEcapClearChNum, &paramSet);
    }

    else
    {
        /* Report EDMA Error */
        System_printf("\nEDMA driver setParam FAIL\n");
        return;
    }

    if(result == 0)
    {
        paramSet.srcAddr    = (uint32_t)(uint32_t)(ocmcBaseAddress +
                              ECAP_CLR_CONFIG_OFFSET);
        paramSet.destAddr = (uint32_t)(ptcpConfig->ecapPwmssBaseAddr + CSL_ECAP_ECCLR);
        paramSet.aCnt    = 2;
        paramSet.bCnt    = 1;
        paramSet.cCnt    = 1;

        paramSet.srcBIdx    = 0;
        paramSet.srcCIdx    = 0;

        paramSet.destBIdx   = 0;
        paramSet.destCIdx   = 0;

        paramSet.bCntReload = (uint16_t) 0;
        /* paramSet.linkAddr = EDMA3_ECAPEVT0_LINK_CHA_NUM;*/
        paramSet.bCntReload = (uint16_t) 0;
        paramSet.linkAddr   = 0xFFFFu;
        paramSet.opt = 0x0u;
        /* Src is in INCR mode & Dest is in FIFO modes                            */
        paramSet.opt &= 0xFFFFFFFCu;
        /* Program the TCC */
        paramSet.opt |= ((ptcpConfig->edmaEcapClearLinktcc << OPT_TCC_SHIFT) &
                         OPT_TCC_MASK);
        paramSet.opt |= (1 << OPT_SYNCDIM_SHIFT);
        result = EDMA3_DRV_setPaRAM(ptcpConfig->gEdmaHandle,
                                    ptcpConfig->edmaEcapClearLinkChNum, &paramSet);
    }

    else
    {
        /* Report EDMA Error */
        System_printf("\nEDMA driver setParam FAIL\n");
        return;
    }

    if(result == 0)
    {
        result = EDMA3_DRV_linkChannel((EDMA3_DRV_Handle)ptcpConfig->gEdmaHandle,
                                       (uint32_t) ptcpConfig->edmaEcapEvt0ChNum,
                                       (uint32_t) ptcpConfig->edmaEcapEvt0LinkChNum);
        result = EDMA3_DRV_linkChannel((EDMA3_DRV_Handle)ptcpConfig->gEdmaHandle,
                                       (uint32_t) ptcpConfig->edmaEcapEvt0LinkChNum,
                                       (uint32_t) ptcpConfig->edmaEcapEvt0ChNum);
    }

    else
    {
        /* Report EDMA Error */
        System_printf("\nEDMA driver setParam FAIL\n");
        return;
    }

    if(result == 0)
    {
        result = EDMA3_DRV_linkChannel((EDMA3_DRV_Handle)ptcpConfig->gEdmaHandle,
                                       (uint32_t) ptcpConfig->edmaEcapClearChNum,
                                       (uint32_t) ptcpConfig->edmaEcapClearLinkChNum);
        result = EDMA3_DRV_linkChannel((EDMA3_DRV_Handle)ptcpConfig->gEdmaHandle,
                                       (uint32_t) ptcpConfig->edmaEcapClearLinkChNum,
                                       (uint32_t) ptcpConfig->edmaEcapClearChNum);
    }

    else
    {
        /* Report EDMA Error */
        System_printf("\nEDMA driver link channel FAIL\n");
        return;
    }

    if(result == 0)
    {
        /* Transfer complete chaining enable. */
        chain.tcchEn = EDMA3_DRV_TCCHEN_EN;
        /* Intermediate transfer complete chaining enable. */
        chain.itcchEn = EDMA3_DRV_ITCCHEN_EN;
        /* Transfer complete interrupt is enabled. */
        chain.tcintEn = EDMA3_DRV_TCINTEN_DIS;
        /* Intermediate transfer complete interrupt is disabled. */
        chain.itcintEn = EDMA3_DRV_ITCINTEN_DIS;

        result = EDMA3_DRV_chainChannel(ptcpConfig->gEdmaHandle,
                                        ptcpConfig->edmaEcapEvt0ChNum, ptcpConfig->edmaEcapClearChNum,
                                        (EDMA3_DRV_ChainOptions *)&chain);
        result = EDMA3_DRV_chainChannel(ptcpConfig->gEdmaHandle,
                                        ptcpConfig->edmaEcapEvt0LinkChNum, ptcpConfig->edmaEcapClearChNum,
                                        (EDMA3_DRV_ChainOptions *)&chain);
    }

    else
    {
        /* Report EDMA Error */
        System_printf("\nEDMA driver link channel FAIL\n");
        return;
    }


    if(result == 0)
    {
        result = EDMA3_DRV_enableTransfer(
                     (EDMA3_DRV_Handle) ptcpConfig->gEdmaHandle,
                     (uint32_t) ptcpConfig->edmaEcapEvt0ChNum,
                     (EDMA3_DRV_TrigMode) EDMA3_DRV_TRIG_MODE_EVENT);

        if(result != ((Int32) EDMA3_DRV_SOK))
        {
            /* Report EDMA Error */
            System_printf("\nEDMA driver enable transfer FAIL\n");
            return;
        }
    }

    else
    {
        /* Report EDMA Error */
        System_printf("\nEDMA driver chain channel FAIL\n");
        return;
    }
}

/**
  * Get the current delay values
  *
  * @param portDelays Reference to structure for ptcp delay values
  * @param portNum  port no. for which delay values are requested
  */

void PN_PTCP_getDelayValues(PN_Handle pnHandle, ptcpPortDelayVal_t *portDelays,
                            uint8_t portNum)
{
    portNum = portNum - 1;

    portDelays->rxDelayLocal    =
        (pnHandle->pnPtcpConfig)->portTimes[portNum].rxDelayLocal;
    portDelays->txDelayLocal    =
        (pnHandle->pnPtcpConfig)->portTimes[portNum].txDelayLocal;
    portDelays->rxDelayRemote   =
        (pnHandle->pnPtcpConfig)->portTimes[portNum].rxDelayRemote;
    portDelays->txDelayRemote   =
        (pnHandle->pnPtcpConfig)->portTimes[portNum].txDelayRemote;
    portDelays->cableDelay      =
        (pnHandle->pnPtcpConfig)->portTimes[portNum].cableDelay;
}

void PN_PTCP_getLocalDelayValues(PN_Handle pnHandle, uint8_t portNum,
                                 uint32_t *outRxDelay, uint32_t *outTxDelay)
{
    portNum = portNum - 1;
    *outRxDelay  = (pnHandle->pnPtcpConfig)->portTimes[portNum].rxDelayLocal;
    *outTxDelay  = (pnHandle->pnPtcpConfig)->portTimes[portNum].txDelayLocal;
}

void PN_PTCP_getRemoteDelayValues(PN_Handle pnHandle, uint8_t portNum,
                                  uint32_t *outRxDelay, uint32_t *outTxDelay)
{
    portNum = portNum - 1;
    *outRxDelay = (pnHandle->pnPtcpConfig)->portTimes[portNum].rxDelayRemote;
    *outTxDelay = (pnHandle->pnPtcpConfig)->portTimes[portNum].txDelayRemote;
}

void PN_PTCP_getSyncInfo(PN_Handle pnHandle, ptcpSyncInfo_t *syncInfo)
{
    /*copy the values from internal sync info structure to the application structure*/
    syncInfo->syncState = (pnHandle->pnPtcpConfig)->deviceSyncInfo->syncState;
    memcpy((void *)(&(syncInfo->masterSA)),
           (void *)(&((pnHandle->pnPtcpConfig)->deviceSyncInfo->masterSA)), 6);
    memcpy((void *)(&(syncInfo->subdomainUUID)),
           (void *)(&((pnHandle->pnPtcpConfig)->deviceSyncInfo->subdomainUUID)), 16);
}

void PN_PTCP_getSyncMasterAddress(PN_Handle pnHandle, uint8_t *addr)
{
    /*copy the values from internal sync info structure to the application structure*/
    memcpy((void *)addr, (void *)(&((
                                        pnHandle->pnPtcpConfig)->deviceSyncInfo->masterSA)), 6);
}

void PN_PTCP_syncPreprocess(PN_Handle pnHandle, uint8_t ctrlByte)
{
    uint8_t    *syncPacket;
    uint32_t *pSyncSOF;
    uint8_t    *pDelay1nsByte;
    uint32_t inDelay, lineDelay = 0;
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;

    if(ctrlByte == 1)
    {
        syncPacket      = (uint8_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                                      SYNC_SF_BUF_OFFSET_P1);
        pSyncSOF        = (uint32_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                                       SYNC_RX_SOF_OFFSET_P1);
    }

    else if(ctrlByte == 2)
    {
        syncPacket      = (uint8_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                                      SYNC_SF_BUF_OFFSET_P2);
        pSyncSOF        = (uint32_t *)(emacBaseAddr->sharedDataRamBaseAddr +
                                       SYNC_RX_SOF_OFFSET_P2);
    }

    else
    {
        return;
    }

    /* Parse sync packet*/
    pDelay1nsByte           = (uint8_t *)(syncPacket + 30);
    *((pnHandle->pnPtcpConfig)->pSyncTorgSecs)          = *((uint32_t *)(
                syncPacket + 64));
    *((pnHandle->pnPtcpConfig)->pSyncTorgNs)            = *((uint32_t *)(
                syncPacket + 68));
    *((pnHandle->pnPtcpConfig)->pSyncRxSOF)         = (*pSyncSOF) +
            640;     /*for s/f, still SFD is taken in firmware, so from SOF to SFD*/

    /* Parse the stored sync packet and calculate total delay*/
    inDelay = 10 * (PN_PTCP_rotUint((uint32_t *)(syncPacket + 24))) +
              PN_PTCP_rotUint((uint32_t *)(syncPacket + 32));
    inDelay += (uint8_t)(*pDelay1nsByte);

    if(ctrlByte == 1)
    {
        lineDelay = *((pnHandle->pnPtcpConfig)->devicePortOffsets[ICSS_EMAC_PORT_1 -
                      1].pSmaLineDelay);
    }

    if(ctrlByte == 2)
    {
        lineDelay = *((pnHandle->pnPtcpConfig)->devicePortOffsets[ICSS_EMAC_PORT_2 -
                      1].pSmaLineDelay);
    }

    *((pnHandle->pnPtcpConfig)->pSyncInDelayPlusLD) = inDelay + lineDelay;

    memcpy((void *)((emacBaseAddr->sharedDataRamBaseAddr + SYNC_SBLOCK_OFFSET)),
           (void *)(syncPacket + 32), 32);
}

int32_t PN_PTCP_absVal(int32_t num)
{
    if(num < 0)
    {
        return (-1) * num;
    }

    else
    {
        return        num;
    }
}

void PN_PTCP_setPllWindow(PN_Handle pnHandle, uint32_t pllWindowSize)
{
    (pnHandle->pnPtcpConfig)->currentPtcpStatus->syncPllWnd = pllWindowSize;
}

void PN_PTCP_setSyncTimeoutFactor(PN_Handle pnHandle,
                                  uint32_t syncTimeoutFactor)
{
    (pnHandle->pnPtcpConfig)->currentPtcpStatus->syncTimeoutFactor =
        syncTimeoutFactor;
}

void PN_PTCP_setTakeoverTimeoutFactor(PN_Handle pnHandle,
                                      uint32_t takeoverTimeoutFactor)
{
    (pnHandle->pnPtcpConfig)->currentPtcpStatus->takeoverTimeoutFactor =
        takeoverTimeoutFactor;
}

void PN_PTCP_syncTimeoutMonitor(PN_Handle pnHandle)
{
    uint8_t syncInitFlag;

    while(1)
    {
        syncInitFlag = *((pnHandle->pnPtcpConfig)->pSyncInitFlag);

        PN_PTCP_taskSleep(30 + 1);

        /*put some kind of data synchronization mechanism or lock to avoid corruption
        check whether first sync has been received or not*/
        if((pnHandle->pnPtcpConfig)->currentPtcpStatus->firstSyncRcv)
        {

            if((pnHandle->pnPtcpConfig)->currentPtcpStatus->syncRcv)
            {
                /* sync received in this cycle of 30 ms*/
                (pnHandle->pnPtcpConfig)->currentPtcpStatus->nSyncMissed = 0;
                (pnHandle->pnPtcpConfig)->currentPtcpStatus->syncRcv = 0;
            }

            else
            {
                /*sync not received in this cycle of 30 ms*/
                ((pnHandle->pnPtcpConfig)->currentPtcpStatus->nSyncMissed)++;
#ifdef DEBUG_SYNC_EVENTS
                (pnHandle->pnPtcpConfig->pnPtcpDebugAttrs)->syncmissCounter++;
#endif

                if((pnHandle->pnPtcpConfig)->currentPtcpStatus->nSyncMissed ==
                        (pnHandle->pnPtcpConfig)->currentPtcpStatus->takeoverTimeoutFactor)
                {
                    (pnHandle->pnPtcpConfig)->masterChange = 1;
                    *((pnHandle->pnPtcpConfig)->pSyncInitFlag) = syncInitFlag & (~(1 << 1));

                    if((pnHandle->pnPtcpConfig)->ptcpSyncStatusCall != NULL)
                    {
                        (pnHandle->pnPtcpConfig)->ptcpSyncStatusCall(TAKEOVER_TIMEOUT, (uint32_t)NULL);
                    }
                }

                if((pnHandle->pnPtcpConfig)->currentPtcpStatus->nSyncMissed ==
                        (pnHandle->pnPtcpConfig)->currentPtcpStatus->syncTimeoutFactor)
                {
                    PN_PTCP_reset(pnHandle);
                    (pnHandle->pnPtcpConfig)->masterChange = 0;

                    if((pnHandle->pnPtcpConfig)->ptcpSyncStatusCall != NULL)
                    {
                        (pnHandle->pnPtcpConfig)->ptcpSyncStatusCall(SYNC_TIMEOUT, (uint32_t)NULL);
                    }

                }
            }
        }
    }
}

void PN_PTCP_setSyncUUID(PN_Handle pnHandle, uint8_t *subdomainUUID)
{
    uint8_t syncInitFlag = *((pnHandle->pnPtcpConfig)->pSyncInitFlag);

    if(subdomainUUID == NULL)
    {
        return;
    }

    memcpy((void *)(&((pnHandle->pnPtcpConfig)->currentPtcpStatus->subdomainUUID)),
           (void *)subdomainUUID, 16);
    memcpy((void *)((pnHandle->pnPtcpConfig)->pSubDomainUUID),
           (void *)subdomainUUID, 16);

    *((pnHandle->pnPtcpConfig)->pSyncInitFlag) = syncInitFlag | 1;

    /*Notify the stack about current sync status*/
    if((pnHandle->pnPtcpConfig)->ptcpSyncStatusCall != NULL)
    {
        (pnHandle->pnPtcpConfig)->ptcpSyncStatusCall((
                    pnHandle->pnPtcpConfig)->currentPtcpStatus->syncState, (uint32_t)NULL);
    }
}


void PN_PTCP_taskSleep(uint32_t mSec)
{
    uint32_t    ticks = 0;

    ticks = ((mSec * 1000) / Clock_tickPeriod) + 1;
    TaskP_sleep(ticks);
}

void PN_PTCP_configureSync0Pin(PN_Handle pnHandle)
{
    uint32_t iepCmpCfg = 0;
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;
    /*enable cmp1 : t2 bit of cfg registers*/

    iepCmpCfg = HWREG(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_CMP_CFG_REG);
    iepCmpCfg = iepCmpCfg | 0x4;
    HWREG(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_CMP_CFG_REG) = iepCmpCfg;
    /*program cmp1 reg with period, used for sync0 signal generation: 10 us*/
    HWREG(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_CMP1_REG0) =
        (pnHandle->pnPtcpConfig)->ptcpSync0PinStart;

    /*configure the pulse width for sync0: 5000+1 cycles i.e. 25us*/
    HWREG(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_SYNC_PWIDTH_REG) =
        (pnHandle->pnPtcpConfig)->ptcpSync0PinPulseWidth;


}

void PN_PTCP_configureDelayMeasurement(PN_Handle pnHandle, uint8_t portNum,
                                       ptcpPortStatus_t state)
{
    portNum = portNum - 1;
    (pnHandle->pnPtcpConfig)->currentPtcpStatus->cDelayEnable[portNum] = state;
}

int32_t PN_PTCP_modFunc(int32_t num, uint32_t mod)
{
    if(num >= 0)
    {
        return (num % mod);
    }

    else
    {
        while(num < 0)
        {
            num += mod;
        }

        return num;
    }
}

#endif  /*PTCP_SUPPORT*/

void PN_PTCP_configureSyncFwd(ICSS_EmacBaseAddressHandle_T
                              ICSS_EmacBaseAddressHandle, ptcpPortStatus_t state)
{
    /*configure the sync forwarding*/
    uint8_t *pTemp8 = (uint8_t *)(ICSS_EmacBaseAddressHandle->sharedDataRamBaseAddr
                                  + SYNC_FWD_ENABLED_OFFSET);
    *pTemp8 = state;
}
void PN_PTCP_configureDelayResp(ICSS_EmacBaseAddressHandle_T
                                ICSS_EmacBaseAddressHandle, uint8_t portNum, ptcpPortStatus_t state)
{
    uint8_t *pTemp8;
    portNum = portNum - 1;

    if(portNum == 0)
    {
        pTemp8 = (uint8_t *)(ICSS_EmacBaseAddressHandle->sharedDataRamBaseAddr +
                             P1_DELAY_RESP_CTRL_OFFSET);
    }

    else if(portNum == 1)
    {
        pTemp8 = (uint8_t *)(ICSS_EmacBaseAddressHandle->sharedDataRamBaseAddr +
                             P2_DELAY_RESP_CTRL_OFFSET);
    }

    else
    {
        return;
    }

    *pTemp8 = state;
}
