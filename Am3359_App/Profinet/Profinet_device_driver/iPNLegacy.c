/**
 * @file  iPNLegacy.c
 *
 *\par
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
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *\par
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <Profinet/Include_profinet/PN_Handle.h>
#include <Profinet/Include_profinet/PN_HandleDef.h>

#include <Profinet/Include_profinet/iPNLegacy.h>
#include <Profinet/Include_profinet/iPNDrv.h>
#include <Profinet/Include_profinet/iRtcDrv.h>
#include <Profinet/Include_profinet/iPnOs.h>
#include <Profinet/Include_profinet/iPtcpUtils.h>
#include <Profinet/Include_profinet/iRtcDrv.h>
#include <Profinet/Include_profinet/mdio_drv.h>
#include <Profinet/Include_profinet/pn_soc.h>

#include <ti/drv/icss_emac/icss_emac_ver.h>
#include <ti/drv/icss_emac/icss_emacFwInit.h>
#include <ti/drv/icss_emac/icss_emacDrv_Def.h>
#include <ti/drv/icss_emac/icss_emacDrv.h>
#include <ti/drv/icss_emac/icss_emacFwInit.h>
/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */
/* ========================================================================== */
/*                          Static variables                                  */
/* ========================================================================== */
/** \brief Local copy of ICSS_EmacHandle used to maintain old APIs that don't use handles */
static PN_Handle localHandle;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
void PN_setHandle(PN_Handle pnHandle)
{
    localHandle = pnHandle;
}

PN_Handle PN_getPnHandle()
{
    return localHandle;
}

uint8_t PN_EmacSocMACAddrGet(PN_Handle pnHandle, uint8_t instance,
                             uint8_t *macId)
{
    PN_socgetMACAddress(instance,macId);
    return 0;
}

uint8_t pnEmacSocMACAddrGet(uint8_t instance, uint8_t *macId)
{
    return PN_EmacSocMACAddrGet(PN_getPnHandle(), instance, macId);
}

int32_t setStaticFilterTable(const uint8_t *macAddr, uint8_t ctrl,
                             uint8_t portNumber, uint8_t table)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    return PN_setStaticFilterTable((((ICSS_EmacHwAttrs *)
                                     emacHandle->hwAttrs)->emacBaseAddrCfg) , macAddr, ctrl, portNumber, table);
}

int32_t setRtc3PortStatus(uint8_t portNumber, uint8_t status)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    return PN_setRtc3PortStatus((((ICSS_EmacHwAttrs *)
                                  emacHandle->hwAttrs)->emacBaseAddrCfg) , portNumber, status);
}

uint8_t *getLastCpm(t_rtcPacket *pkt)
{
    return PN_getLastCpm(PN_getPnHandle() , pkt);
}

int32_t setRedGuard(uint16_t validLowerFrameId, uint16_t validUpperFrameId)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    return PN_setRedGuard((((ICSS_EmacHwAttrs *)
                            emacHandle->hwAttrs)->emacBaseAddrCfg) , validLowerFrameId, validUpperFrameId);
}

int32_t setMaxLineRxDelay(uint8_t portNumber, int32_t maxLineRxDelayValue)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    return PN_setMaxLineRxDelay((((ICSS_EmacHwAttrs *)
                                  emacHandle->hwAttrs)->emacBaseAddrCfg) , portNumber, maxLineRxDelayValue);
}

int32_t setMaxBridgeDelay(int32_t maxBridgeDelayValue)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    return PN_setMaxBridgeDelay((((ICSS_EmacHwAttrs *)
                                  emacHandle->hwAttrs)->emacBaseAddrCfg) , maxBridgeDelayValue);
}

int32_t setYellowPeriod(int32_t yellowPeriodTime)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    return PN_setYellowPeriod((((ICSS_EmacHwAttrs *)
                                emacHandle->hwAttrs)->emacBaseAddrCfg) , yellowPeriodTime);
}

int32_t mapPhaseToProfile(int32_t portNumber, int32_t phaseNumber,
                          int32_t profileNumberRx, int32_t profileNumberTx)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    return PN_mapPhaseToProfile((((ICSS_EmacHwAttrs *)
                                  emacHandle->hwAttrs)->emacBaseAddrCfg) , portNumber, phaseNumber,
                                profileNumberRx, profileNumberTx);
}

int32_t setProfile(int32_t portNumber, int32_t profileNumber,
                   int32_t rxSoGValue, int32_t txSoGValue)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    return PN_setProfile((((ICSS_EmacHwAttrs *)
                           emacHandle->hwAttrs)->emacBaseAddrCfg) , portNumber, profileNumber, rxSoGValue,
                         txSoGValue);
}

int32_t setDcpFilterStationName(const uint8_t *dcpNameOfStation,
                                uint8_t lengthOfStationName)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    return PN_setDcpFilterStationName((((ICSS_EmacHwAttrs *)
                                        emacHandle->hwAttrs)->emacBaseAddrCfg), dcpNameOfStation, lengthOfStationName);
}

int32_t TxPacketOS(const uint8_t *srcAddress, int32_t portNumber,
                   int32_t queuePriority, int32_t lengthOfPacket)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    return PN_OS_txPacket(emacHandle,  srcAddress, portNumber, queuePriority,
                          lengthOfPacket);
}

int32_t PN_RxPktInfo(int32_t *portNumber, int32_t *queueNumber)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    return ICSS_EmacRxPktInfo(emacHandle, portNumber, queueNumber);
}

int32_t PN_RxPktGet(uint32_t destAddress, int32_t queueNumber, int32_t *port,
                    int32_t *more)
{
    int32_t retval = 0;
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    ICSS_EmacRxArgument rxArgs;
    rxArgs.icssEmacHandle = emacHandle;
    rxArgs.destAddress = destAddress;
    rxArgs.queueNumber = queueNumber;
    rxArgs.more = 0;
    rxArgs.port = 0;
    retval = ICSS_EmacRxPktGet(&rxArgs, NULL);
    *more = rxArgs.more;
    *port = rxArgs.port;
    return retval;
}


uint8_t PN_GetLinkStatus(uint8_t portNum)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    return ((ICSS_EmacObject *)
                   emacHandle->object)->linkStatus[portNum];

}
void PN_SetPhyConfig(uint8_t phyConfig, uint8_t portNum)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    uint32_t mdioBaseAddr = ((((ICSS_EmacHwAttrs *)
                               emacHandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs);
    uint8_t phyAddr = ((ICSS_EmacObject *)
                       emacHandle->object)->emacInitcfg->phyAddr[portNum];
    MDIO_setPhyConfig(mdioBaseAddr, phyAddr, phyConfig);

}

uint8_t PN_GetPhyConfig(uint8_t portNum)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    uint32_t mdioBaseAddr = ((((ICSS_EmacHwAttrs *)
                               emacHandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs);
    uint8_t phyAddr = ((ICSS_EmacObject *)
                       emacHandle->object)->emacInitcfg->phyAddr[portNum];

    return MDIO_getPhyConfig(mdioBaseAddr, phyAddr);
}


uint8_t PN_findMAC(uint8_t *macId)
{
    ICSS_EmacHandle emacHandle;
    ICSSEMAC_IoctlCmd ioctlParams;
    ioctlParams.ioctlVal = macId;

    ioctlParams.command = ICSS_EMAC_LEARN_CTRL_FIND_MAC;

    emacHandle = PN_getPnHandle()->emacHandle;

    return ICSS_EmacIoctl(emacHandle, ICSS_EMAC_IOCTL_LEARNING_CTRL, (uint32_t)NULL,
                          (void *)&ioctlParams);
}

int32_t setCpmDHT(uint16_t dht, uint8_t pos)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    return PN_setCpmDHT((((ICSS_EmacHwAttrs *)emacHandle->hwAttrs)->emacBaseAddrCfg)
                        , dht, pos);
}

int32_t setBaseClock(uint16_t factor)
{
    return PN_setBaseClock(PN_getPnHandle() , factor);
}

int32_t clearList(uint8_t list)
{
    return PN_clearList(PN_getPnHandle() , list);
}

int32_t iRtcEnableIsr()
{
    return PN_RTC_enableISR(PN_getPnHandle());
}

int32_t delPmList(t_rtcPacket *pmPkt)
{
    return PN_delPmList(PN_getPnHandle() , pmPkt);
}

int32_t insPpmList(t_rtcPacket *ppmPkt, uint8_t legMode)
{
    return PN_insPpmList(PN_getPnHandle() , ppmPkt, legMode);
}

int32_t insCpmList(t_rtcPacket *cpmPkt)
{
    return PN_insCpmList(PN_getPnHandle()  , cpmPkt);
}

#ifdef PTCP_SUPPORT
void configureSync0Pin()
{
    PN_PTCP_configureSync0Pin(PN_getPnHandle());
}
#endif

void ptcpConfigureSyncFwd(ptcpPortStatus_t state)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    PN_PTCP_configureSyncFwd((((ICSS_EmacHwAttrs *)
                               emacHandle->hwAttrs)->emacBaseAddrCfg), state);
}

void PN_EmacRegisterProtocolCallback(void *callBackPtr, void *userArg)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    ((((ICSS_EmacObject *)
       emacHandle->object)->callBackHandle)->rxRTCallBack)->callBack = callBackPtr;
    ((((ICSS_EmacObject *)
       emacHandle->object)->callBackHandle)->rxRTCallBack)->userArg = userArg;
}

void PN_EmacRegisterPort0ISRCallback(ICSS_EmacCallBack callBack, void *userArg)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    ((ICSS_EmacObject *)emacHandle->object)->port0ISRCall = callBack;
    ((ICSS_EmacObject *)emacHandle->object)->port0ISRUser = userArg;

}

void PN_EmacRegisterPort1ISRCallback(ICSS_EmacCallBack callBack, void *userArg)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    ((ICSS_EmacObject *)emacHandle->object)->port1ISRCall = callBack;
    ((ICSS_EmacObject *)emacHandle->object)->port1ISRUser = userArg;

}

void PN_purgeTable(uint8_t portNum)
{
    ICSSEMAC_IoctlCmd ioctlParams;
    ICSS_EmacHandle emacHandle;
    ioctlParams.command = ICSS_EMAC_LEARN_CTRL_CLR_TABLE;
    emacHandle = PN_getPnHandle()->emacHandle;
    ICSS_EmacIoctl(emacHandle, ICSS_EMAC_IOCTL_LEARNING_CTRL, portNum,
                   (void *)&ioctlParams);
}

int32_t setMrpPortState(uint8_t portNumber, uint8_t pState)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    return PN_MRP_setPortState((((ICSS_EmacHwAttrs *)
                                 emacHandle->hwAttrs)->emacBaseAddrCfg), portNumber, pState);
}

int32_t getMrpPortState(uint8_t portNumber, uint8_t *pState)
{
    ICSS_EmacHandle emacHandle = PN_getPnHandle()->emacHandle;
    return PN_MRP_getPortState((((ICSS_EmacHwAttrs *)
                                 emacHandle->hwAttrs)->emacBaseAddrCfg), portNumber, pState);
}

int32_t cfgRtcMem(uint8_t ar, uint16_t size)
{
    return PN_cfgRtcMem(PN_getPnHandle(), ar, size);
}

int32_t allocPkt(t_rtcPacket **pPkt, uint8_t type)
{
    return PN_allocPkt(PN_getPnHandle(), pPkt, type);
}
#ifdef PTCP_SUPPORT
void ptcpGetSyncInfo(ptcpSyncInfo_t *syncInfo)
{
    PN_PTCP_getSyncInfo(PN_getPnHandle(), syncInfo);
}

void ptcpRegisterDelayUpdateCall(ptcpCallBack_t callBack)
{
    PN_PTCP_registerDelayUpdateCall(PN_getPnHandle(), callBack);
}

void ptcpRegisterSyncStatusCall(ptcpCallBack_t callBack)
{
    PN_PTCP_registerSyncStatusCall(PN_getPnHandle(), callBack);
}

void ptcpSetPllWindow(uint32_t pllWindowSize)
{
    PN_PTCP_setPllWindow(PN_getPnHandle(), pllWindowSize);
}

void ptcpSetSyncTimeoutFactor(uint32_t syncTimeoutFactor)
{
    PN_PTCP_setSyncTimeoutFactor(PN_getPnHandle(), syncTimeoutFactor);
}

void ptcpSetSyncUUID(uint8_t *subdomainUUID)
{
    PN_PTCP_setSyncUUID(PN_getPnHandle(), subdomainUUID);
}
#endif

void registerCpmCall(pnCallBack_t callBack)
{
    PN_registerCpmCall(PN_getPnHandle(), callBack);
}

void registerStatCall(pnCallBack_t callBack)
{
    PN_registerStatCall(PN_getPnHandle(), callBack);
}

uint8_t *relPpmBuff(t_rtcPacket *pkt)
{
    return PN_relPpmBuff(PN_getPnHandle(), pkt);
}

int32_t rtcDisableISR()
{
    return PN_RTC_disableISR(PN_getPnHandle());
}

