/**
 * @file  iPNDrv.c
 *
 * @brief Basic interface to Profinet Application
 *  This file contains most of the Profinet initialization APIs
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

/*
 * pnDrvConfig.h needs to be first first file included in driver!
 * It is application dependent and as such part of the application code.
 * It defines all basic feature options in driver (compile time!)
 */
#include <ti/ndk/inc/stkmain.h>
#include <pnDrvConfig.h>
#include <protocols/profinet_slave/include/PN_Handle.h>
#include <protocols/profinet_slave/include/PN_HandleDef.h>
#include <protocols/profinet_slave/include/iPNDrv.h>
#include <protocols/profinet_slave/include/iRtcDrv2.h>
#include <protocols/profinet_slave/include/iPNLegacy.h>
#include <protocols/profinet_slave/include/iPtcpDrv.h>
#include <protocols/profinet_slave/include/iPtcpUtils.h>
#include <protocols/profinet_slave/include/PN_ForwardDecisionTable.h>
#include <protocols/profinet_slave/include/PN_ReceiveDecisionTable.h>

#include <ti/csl/hw_types.h>

#include <ti/drv/icss_emac/icss_emac_ver.h>
#include <ti/drv/icss_emac/src/icss_emacLoc.h>
#include <ti/drv/icss_emac/icss_emacFwInit.h>
#include <ti/drv/pruss/pruicss.h>
#include <ti/drv/pruss/soc/pruicss_v1.h>

#include <examples/board/include/soc_icss_header.h>
#include <profinet_irt_PRU0_bin.h>
#include <profinet_irt_PRU1_bin.h>
#include <firmware_version.h>
/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

/**
* @internal
* @def PRU0_FIRMWARE_NAME
*      name of the C struct in PRU header file. For PRU0
*/
#define PRU0_FIRMWARE_NAME  PRU0_FIRMWARE

/**
* @internal
* @def PRU1_FIRMWARE_NAME
*      name of the C struct in PRU header file. For PRU1
*/
#define PRU1_FIRMWARE_NAME  PRU1_FIRMWARE
/**
* @internal
* @def FILTER_MAC_ID_TYPE1_OCTET0
*      MAC_ID Type 1 Octets
*/
#define FILTER_MAC_ID_TYPE1_OCTET0 0x01
#define FILTER_MAC_ID_TYPE1_OCTET1 0x0E
#define FILTER_MAC_ID_TYPE1_OCTET2 0xCF
#define FILTER_MAC_ID_TYPE1_OCTET3 0x00
#define FILTER_MAC_ID_TYPE1_OCTET4 0x05
#define FILTER_MAC_ID_TYPE1_OCTET5 0xFF

/**
* @internal
* @def FILTER_MAC_ID_TYPE2_OCTET0
*      MAC_ID Type 2 Octets
*/
#define FILTER_MAC_ID_TYPE2_OCTET0 0x01
#define FILTER_MAC_ID_TYPE2_OCTET1 0x15
#define FILTER_MAC_ID_TYPE2_OCTET2 0x4E
#define FILTER_MAC_ID_TYPE2_OCTET3 0x00
#define FILTER_MAC_ID_TYPE2_OCTET4 0x00
#define FILTER_MAC_ID_TYPE2_OCTET5 0x1F

/**
* @internal
* @def FILTER_MAC_ID_TYPE3_OCTET0
*      MAC_ID Type 3 Octets
*/
#define FILTER_MAC_ID_TYPE3_OCTET0 0x01
#define FILTER_MAC_ID_TYPE3_OCTET1 0x80
#define FILTER_MAC_ID_TYPE3_OCTET2 0xC2
#define FILTER_MAC_ID_TYPE3_OCTET3 0x00
#define FILTER_MAC_ID_TYPE3_OCTET4 0x00
#define FILTER_MAC_ID_TYPE3_OCTET5 0x1F

/* ========================================================================== */
/*                          Static variables                                  */
/* ========================================================================== */
/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 * @internal
 * @brief This API is an internal API which is used by setStaticFilterTable.
 * @param macAddr
 *      Following are the valid ranges of multicast addresses:
 *     (01-0E-CF-00-00-00 TO 01-0E-CF-00-05-FF)
 *     (01-15-4E-00-00-00 TO 01-15-4E-00-00-1F)
 *     (01-80-C2-00-00-00 TO 01-80-C2-00-00-1F)
 * @param filterTableAddr Base Address of static filter table
 * @param enable Set or Clear the receive/forwarding corresponding the passed MAC address in the filter table
 *          0
 *          1
 * @retval 0 on success
 */
int32_t PN_writeFilterTable(const uint8_t *macAddr, uint32_t filterTableAddr,
                            int32_t enable);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */



int32_t PN_initDrv(PN_Handle pnHandle)
{
    uint8_t firmwareLoad_done = FALSE;
    uint8_t *pTemp8;
    uint16_t *pTemp16;
    ICSS_EmacBaseAddressHandle_T emacBaseAddr;
    PRUICSS_Handle pruHandle;
    /* Local copy stored to contain old APIs that don't use handles*/

    /* Storing a local copy of the handle to support legacy APIs */
    PN_setHandle(pnHandle);
    /*
     * set Port MAC Addresses
     */
    /* PORT1 MAC Address*/
    emacBaseAddr = ((ICSS_EmacHwAttrs *)(pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;
    pTemp8 = (uint8_t *)(emacBaseAddr->dataRam1BaseAddr + P1_MAC_ADDR);
    PN_EmacSocMACAddrGet(pnHandle, PORT1_MAC, pTemp8);

    pTemp8 = (uint8_t *)(emacBaseAddr->dataRam1BaseAddr + P2_MAC_ADDR);
    PN_EmacSocMACAddrGet(pnHandle, PORT2_MAC, pTemp8);

    pTemp8 = (uint8_t *)(emacBaseAddr->dataRam0BaseAddr + ISOM_TIO_TIMEVAL1);
    memset((void*)pTemp8, 0, (size_t)24);

    pTemp8 = (uint8_t *)(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_CMP_STATUS_REG);
    *pTemp8 = 0xFE;

    pTemp16 = (uint16_t *)(emacBaseAddr->dataRam0BaseAddr + PRU0_PHASE_EVENT_OFFSET);
    *pTemp16 = 0x0101;

    /* remaining (internal) phase management code*/
    PN_setCompensationValue(emacBaseAddr, 75); /*  75ns default*/

    /* init RTC driver*/
    if(PN_initRtcDrv(pnHandle) != 0)
    {
        DbgPrintf(DBG_WARN, "RTC Driver init failed - aborting\n");
        return ERR_DRIVER_INIT_FAIL;
    }

#ifdef PTCP_SUPPORT
    PN_PTCP_init(pnHandle);
#ifdef  ENABLE_LATCH_SUPPORT
    PN_PTCP_latchInit(
        pnHandle);/*Setup latch ISR for absolute time API - needs emac handle*/
#endif
#else
    /*disable sync forwarding*/
    PN_PTCP_configureSyncFwd(emacBaseAddr, disable);

    /*disable delay_resp, in rensponse to delay_req from peer*/
    PN_PTCP_configureDelayResp(emacBaseAddr, 1, disable);
    PN_PTCP_configureDelayResp(emacBaseAddr, 2, disable);

    /*disabling transmission of delay request from the device*/
    /* this gets disabled by disabling ptcpTask*/

    /* enabling Single shot mode for capture register 4/5, i.e. TX PORT1 and TX PORT2*/
    HWREG(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_CAP_CFG_REG) = 0x0001FC30;
#endif
    /* Write the Profinet Static Tables for PORT1*/
    PN_loadStaticTable(emacBaseAddr, PN_Forward_Decision_Table, 50,
                       STATIC_FORWARD_TABLE, ICSS_EMAC_PORT_1);
    PN_loadStaticTable(emacBaseAddr, PN_Receive_Decision_Table, 50,
                       STATIC_RECEIVE_TABLE, ICSS_EMAC_PORT_1);


    /* Write the Profinet Static Tables for PORT2*/
    PN_loadStaticTable(emacBaseAddr, PN_Forward_Decision_Table, 50,
                       STATIC_FORWARD_TABLE, ICSS_EMAC_PORT_2);
    PN_loadStaticTable(emacBaseAddr, PN_Receive_Decision_Table, 50,
                       STATIC_RECEIVE_TABLE, ICSS_EMAC_PORT_2);


    pruHandle = ((ICSS_EmacObject *)(pnHandle->emacHandle)->object)->pruIcssHandle;

    if(PRUICSS_pruWriteMemory(pruHandle, PRU_ICSS_IRAM(0) , 0,
                              (uint32_t *) PRU0_FIRMWARE_NAME,
                              sizeof(PRU0_FIRMWARE_NAME)))
    {
        if(PRUICSS_pruWriteMemory(pruHandle, PRU_ICSS_IRAM(1) , 0,
                                  (uint32_t *) PRU1_FIRMWARE_NAME,
                                  sizeof(PRU1_FIRMWARE_NAME)))
        {
            firmwareLoad_done = TRUE;
        }
    }


    if(!firmwareLoad_done)
    {
        DbgPrintf(DBG_WARN, "PRU Loading failed - aborting\n");
        return ERR_FIRMWARE_LOAD_FAIL;
    }


    if(HWREG(((PRUICSS_HwAttrs *)(pruHandle->hwAttrs))->baseAddr + PRU_ICSS_DATARAM(
                 0)) != ICSS_FIRMWARE_RELEASE_1)     /*hard coded version number address!*/
    {
        DbgPrintf(DBG_WARN, "PRU firmware bad - aborting\n");
        return ERR_FIRMWARE_VERSION_BAD;
    }

    else
    {
        if(HWREG(((PRUICSS_HwAttrs *)(pruHandle->hwAttrs))->baseAddr + PRU_ICSS_DATARAM(
                     0) + 4) != ICSS_FIRMWARE_RELEASE_2)
        {
            DbgPrintf(DBG_WARN, "PRU firmware bad - aborting\n");
            return ERR_FIRMWARE_VERSION_BAD;
        }
    }

    /*By default MRP Ports are in FORWARDING Mode*/
    PN_MRP_setPortState(emacBaseAddr, ICSS_EMAC_PORT_1, FORWARDING);
    PN_MRP_setPortState(emacBaseAddr, ICSS_EMAC_PORT_2, FORWARDING);
#ifdef WATCHDOG_SUPPORT
    PN_setWatchDogTimer(pnHandle,
                        watchDogExpireDuration); /*ICSS WatchDog Expiry duration */
#endif

    return 0;
}

int32_t PN_setDcpFilterStationName(ICSS_EmacBaseAddressHandle_T
                                   ICSS_EmacBaseAddressHandle, const uint8_t *dcpNameOfStation,
                                   uint8_t lengthOfStationName)
{
    int i = 0;
    int j = 0;
    int numOfCharInStationName = 0;

    if(lengthOfStationName > 240)
    {
        return ERR_STATION_NAME_LENGTH;
    }

    numOfCharInStationName = lengthOfStationName;

    if(lengthOfStationName > DCP_NAME_CMP_NO_OF_CHAR)
    {
        j = (lengthOfStationName - DCP_NAME_CMP_NO_OF_CHAR);
        numOfCharInStationName = DCP_NAME_CMP_NO_OF_CHAR;
    }

    /* Load the DCP station name*/
    for(i = 0; i < numOfCharInStationName; i++)
    {
        HWREGB(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr + PN_DCPF_NAME_OFFSET + i) =
            dcpNameOfStation[j++];
    }

    /* Length byte is after the DCP_NAME_CMP_NO_OF_CHAR nunber of characters in the memory*/
    HWREGB(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr +
           PN_DCPF_NAME_LENGTH_OFFSET) = lengthOfStationName;

    return 0;
}

/*
    This function programs the Profile. In a profile the SoG time for Rx and Tx is set for a port.
*/
int32_t PN_setProfile(ICSS_EmacBaseAddressHandle_T ICSS_EmacBaseAddressHandle,
                      int32_t portNumber, int32_t profileNumber, int32_t rxSoGValue,
                      int32_t txSoGValue)
{
    int32_t port1ProfileBaseAddr = RTC_GREEN_BEGIN_P1_1_RX_OFFSET;
    int32_t port2ProfileBaseAddr = RTC_GREEN_BEGIN_P2_1_RX_OFFSET;
    int32_t profile_base_addr = 0;

    if(profileNumber == 0 || profileNumber > 5)
    {
        return -1;    /* out of range*/
    }
    /*TBD .. param checking for errors*/
    if(portNumber == ICSS_EMAC_PORT_1)
    {
        profile_base_addr = port1ProfileBaseAddr + (profileNumber - 1) * 8;
    }
    else
    {
        profile_base_addr = port2ProfileBaseAddr + (profileNumber - 1) * 8;
    }

    HWREG(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr + profile_base_addr)     =
        rxSoGValue;
    HWREG(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr + profile_base_addr + 4) =
        txSoGValue;

    return 0;
}

/*
  This funtion does the mapping for Phase to Profile for SoG
*/
int32_t PN_mapPhaseToProfile(ICSS_EmacBaseAddressHandle_T
                             ICSS_EmacBaseAddressHandle, int32_t portNumber, int32_t phaseNumber,
                             int32_t profileNumberRx, int32_t profileNumberTx)
{
    int32_t phaseToProfileBaseAddr = 0;
    int32_t portRxProfileOffset = 0;
    int32_t portTxProfileOffset = 0;

    if(profileNumberRx == 0 || profileNumberRx > 5)
    {
        return -1;    /* out of range*/
    }

    if(profileNumberTx == 0 || profileNumberTx > 5)
    {
        return -2;
    }

    if(phaseNumber == 0 || phaseNumber > 16)
    {
        return -3;    /* out of range*/
    }

    phaseToProfileBaseAddr = RTC_PAHSE_MAPPING_OFFSET + (phaseNumber - 1) *
                             4; /* per phase four bytes are allocated.Two bytes for each port, one for Rx and second one for Tx.*/

    if(portNumber == ICSS_EMAC_PORT_2)
    {
        phaseToProfileBaseAddr = phaseToProfileBaseAddr + 2;
    }

    /* Compute the offset of Profile Rx and Tx SoG registers for Port1*/
    if(portNumber == ICSS_EMAC_PORT_1)
    {
        portRxProfileOffset = RTC_GREEN_BEGIN_P1_1_RX_OFFSET + (profileNumberRx - 1) *
                              8;
        portTxProfileOffset = RTC_GREEN_BEGIN_P1_1_TX_OFFSET + (profileNumberTx - 1) *
                              8;

    }

    else
    {
        /* Compute the offset of Profile Rx and Tx SoG registers for Port2*/
        portRxProfileOffset = RTC_GREEN_BEGIN_P2_1_RX_OFFSET + (profileNumberRx - 1) *
                              8;
        portTxProfileOffset = RTC_GREEN_BEGIN_P2_1_TX_OFFSET + (profileNumberTx - 1) *
                              8;
    }

    HWREGB(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr + phaseToProfileBaseAddr)
        =  portRxProfileOffset;
    HWREGB(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr + phaseToProfileBaseAddr +
           1) =  portTxProfileOffset;

    return 0;
}

int32_t PN_setYellowPeriod(ICSS_EmacBaseAddressHandle_T
                           ICSS_EmacBaseAddressHandle, int32_t yellowPeriodTime)
{
    HWREG(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr + RTC_IRT_YELLOW_TIME_OFFSET)
        =  yellowPeriodTime;
    return 0;

}

int32_t PN_setMaxBridgeDelay(ICSS_EmacBaseAddressHandle_T
                             ICSS_EmacBaseAddressHandle, int32_t maxBridgeDelayValue)
{
    HWREG(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr + MAXBRIDGE_DELAY_OFFSET)
        =  maxBridgeDelayValue;
    return 0;
}

int32_t PN_setMaxLineRxDelay(ICSS_EmacBaseAddressHandle_T
                             ICSS_EmacBaseAddressHandle, uint8_t portNumber, int32_t maxLineRxDelayValue)
{
    if(portNumber == ICSS_EMAC_PORT_1)
    {
        HWREG(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr + MAXLINE_RXDELAY_P1_OFFSET)
            =  maxLineRxDelayValue;
    }

    else
    {
        HWREG(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr + MAXLINE_RXDELAY_P2_OFFSET)
            =  maxLineRxDelayValue;
    }

    return 0;
}

void PN_setCompensationValue(ICSS_EmacBaseAddressHandle_T
                             ICSS_EmacBaseAddressHandle, uint16_t compensationValue)
{
    HWREG(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr + COMPENSATION_OFFSET)     =
        compensationValue;
    return;
}

int32_t PN_setRedGuard(ICSS_EmacBaseAddressHandle_T ICSS_EmacBaseAddressHandle,
                       uint16_t validLowerFrameId, uint16_t validUpperFrameId)
{

    uint16_t tempFrameId = validLowerFrameId;
    uint16_t tempFrameIdLowerByte = (tempFrameId >> 8);
    uint16_t tempFrameIdHigherByte = (tempFrameId << 8);
    tempFrameId = (tempFrameIdLowerByte | tempFrameIdHigherByte);

    HWREGH(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr +
           RTC3_SOF_RedFrameID_OFFSET)     =  tempFrameId;

    tempFrameId = validUpperFrameId;
    tempFrameIdLowerByte = (tempFrameId >> 8);
    tempFrameIdHigherByte = (tempFrameId << 8);
    tempFrameId = (tempFrameIdLowerByte | tempFrameIdHigherByte);
    HWREGH(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr +
           RTC3_EOF_RedFrameID_OFFSET)     =  tempFrameId;

    return 0;
}

int32_t PN_setRtc3PortStatus(ICSS_EmacBaseAddressHandle_T
                             ICSS_EmacBaseAddressHandle, uint8_t portNumber, uint8_t status)
{

    if((status != OFF) && (status != UP) && (status != RUN))
    {
        return -1;
    }

    if(portNumber == ICSS_EMAC_PORT_1)
    {
        HWREGB(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr + PORT1_STATUS_OFFSET) =
            status;
    }

    else
    {
        HWREGB(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr + PORT2_STATUS_OFFSET) =
            status;
    }

    return 0;
}
int32_t PN_MRP_setPortState(ICSS_EmacBaseAddressHandle_T
                            ICSS_EmacBaseAddressHandle, uint8_t portNumber, uint8_t pState)
{
    if((pState != DISABLED) && (pState != BLOCKING) && (pState != FORWARDING))
    {
        return -1;
    }

    if(portNumber == ICSS_EMAC_PORT_1)
    {
        HWREGB(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr + MRP_PORT1_STATE_OFFSET) =
            pState;
    }

    else
    {
        HWREGB(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr + MRP_PORT2_STATE_OFFSET) =
            pState;
    }

    return 0;
}

int32_t PN_MRP_getPortState(ICSS_EmacBaseAddressHandle_T
                            ICSS_EmacBaseAddressHandle, uint8_t portNumber, uint8_t *pState)
{
    if(0 == pState)
    {
        return -1;
    }

    if(portNumber == ICSS_EMAC_PORT_1)
    {
        *pState = HWREGB(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr +
                         MRP_PORT1_STATE_OFFSET);
    }

    else
    {
        *pState = HWREGB(ICSS_EmacBaseAddressHandle->dataRam0BaseAddr +
                         MRP_PORT2_STATE_OFFSET);
    }

    return 0;
}

int32_t PN_setStaticFilterTable(ICSS_EmacBaseAddressHandle_T
                                ICSS_EmacBaseAddressHandle, const uint8_t *macAddr, uint8_t ctrl,
                                uint8_t portNumber, uint8_t table)
{

    uint32_t rcvTableAddr = 0;
    uint32_t fwdTableAddr = 0;

    if(table == BLOCKING)
    {
        rcvTableAddr = ICSS_EmacBaseAddressHandle->dataRam0BaseAddr +
                       BLOCKING_STATIC_MAC_TABLE_RCV;
        fwdTableAddr = ICSS_EmacBaseAddressHandle->dataRam0BaseAddr +
                       BLOCKING_STATIC_MAC_TABLE_FWD;
    }

    else if(table == FORWARDING)
    {
        if(portNumber == ICSS_EMAC_PORT_1)
        {
            rcvTableAddr = ICSS_EmacBaseAddressHandle->sharedDataRamBaseAddr +
                           STATIC_MAC_TABLE_RCV_PORT1;
            fwdTableAddr = ICSS_EmacBaseAddressHandle->sharedDataRamBaseAddr +
                           STATIC_MAC_TABLE_FWD_PORT1;
        }

        else
        {

            rcvTableAddr = ICSS_EmacBaseAddressHandle->sharedDataRamBaseAddr +
                           STATIC_MAC_TABLE_RCV_PORT2;
            fwdTableAddr = ICSS_EmacBaseAddressHandle->sharedDataRamBaseAddr +
                           STATIC_MAC_TABLE_FWD_PORT2;

        }
    }

    else
    {
        return -1;
    }

    switch(ctrl)
    {
        case NO_RCV_NO_FWD:
            PN_writeFilterTable(macAddr, rcvTableAddr, 0);
            PN_writeFilterTable(macAddr, fwdTableAddr, 0);
            break;

        case RCV_NO_FWD:
            PN_writeFilterTable(macAddr, rcvTableAddr, 1);
            PN_writeFilterTable(macAddr, fwdTableAddr, 0);
            break;

        case NO_RCV_FWD:
            PN_writeFilterTable(macAddr, rcvTableAddr, 0);
            PN_writeFilterTable(macAddr, fwdTableAddr, 1);
            break;

        case RCV_FWD:
            PN_writeFilterTable(macAddr, rcvTableAddr, 1);
            PN_writeFilterTable(macAddr, fwdTableAddr, 1);
            break;

        default:
            return -2;

    }

    return 0;
}

int32_t PN_writeFilterTable(const uint8_t *macAddr, uint32_t filterTableAddr,
                            int32_t enable)
{
    uint16_t    filterTableRowAddr = 0;
    uint16_t    tempRowAddr = 0;
    uint32_t  filterTableRowValue = 0;
    uint32_t  filterBitValue = 1;

    /* Check if MAC Address falls within range 0x010ECF000000 .. 0x010ECF05FF*/
    if((macAddr[0] == FILTER_MAC_ID_TYPE1_OCTET0)
            && (macAddr[1] == FILTER_MAC_ID_TYPE1_OCTET1) &&
            (macAddr[2] == FILTER_MAC_ID_TYPE1_OCTET2)
            && (macAddr[3] == FILTER_MAC_ID_TYPE1_OCTET3) &&
            (macAddr[4] <= FILTER_MAC_ID_TYPE1_OCTET4)
            && (macAddr[5] <= FILTER_MAC_ID_TYPE1_OCTET5))
    {
        filterTableRowAddr =  macAddr[4] << 3;
        tempRowAddr = macAddr[5] & 0xF0;
        tempRowAddr = tempRowAddr >> 5;
        filterTableRowAddr = filterTableRowAddr + tempRowAddr;
        filterTableRowAddr = filterTableRowAddr << 2;
        filterTableRowValue = HWREG(filterTableAddr + filterTableRowAddr);
        tempRowAddr = macAddr[5] & 0x1F;
        filterBitValue = filterBitValue << tempRowAddr;

        if(enable == 0)
        {
            filterTableRowValue = filterTableRowValue & (~filterBitValue);
        }

        else
        {
            filterTableRowValue = filterTableRowValue | filterBitValue;
        }

        HWREG(filterTableAddr + filterTableRowAddr) = filterTableRowValue;
    }

    else
    {
        /*Check if MAC Address falls within range 0x01154E000000 .. 0x01154E00001F*/
        if((macAddr[0] == FILTER_MAC_ID_TYPE2_OCTET0)
                && (macAddr[1] == FILTER_MAC_ID_TYPE2_OCTET1) &&
                (macAddr[2] == FILTER_MAC_ID_TYPE2_OCTET2)
                && (macAddr[3] == FILTER_MAC_ID_TYPE2_OCTET3) &&
                (macAddr[4] == FILTER_MAC_ID_TYPE2_OCTET4)
                && (macAddr[5] <= FILTER_MAC_ID_TYPE2_OCTET5))
        {
            filterTableRowValue = HWREG(filterTableAddr + 0xC0);
            tempRowAddr = macAddr[5] & 0x1F;
            filterBitValue = filterBitValue << tempRowAddr;

            if(enable == 0)
            {
                filterTableRowValue = filterTableRowValue & (~filterBitValue);
            }

            else
            {
                filterTableRowValue = filterTableRowValue | filterBitValue;
            }

            HWREG(filterTableAddr + 0xC0) = filterTableRowValue;
        }

        /* Check if MAC Address falls within range 0x0180C2000000 .. 0x0180C200001F*/
        else if((macAddr[0] == FILTER_MAC_ID_TYPE3_OCTET0)
                && (macAddr[1] == FILTER_MAC_ID_TYPE3_OCTET1) &&
                (macAddr[2] == FILTER_MAC_ID_TYPE3_OCTET2)
                && (macAddr[3] == FILTER_MAC_ID_TYPE3_OCTET3) &&
                (macAddr[4] == FILTER_MAC_ID_TYPE3_OCTET4)
                && (macAddr[5] <= FILTER_MAC_ID_TYPE3_OCTET5))
        {
            filterTableRowValue = HWREG(filterTableAddr + 0xC4);
            tempRowAddr = macAddr[5] & 0x1F;
            filterBitValue = filterBitValue << tempRowAddr;

            if(enable == 0)
            {
                filterTableRowValue = filterTableRowValue & (~filterBitValue);
            }

            else
            {
                filterTableRowValue = filterTableRowValue | filterBitValue;
            }

            HWREG(filterTableAddr + 0xC4) = filterTableRowValue;
        }

        else
        {
            /*MAC Address is out of valid range*/
            return -1;
        }
    }

    return 0;
}

/*
    This function programs ICSS Watchdog Timer to detect if Application is alive or not.
*/
#ifdef WATCHDOG_SUPPORT
int32_t PN_setWatchDogTimer(PN_Handle pnHandle
                            , int32_t timerPeriod)
{
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;

    if((timerPeriod < 2) || (timerPeriod > 6553))
    {
        return -1;    /* out of range*/
    }

    HWREG(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_PD_WD_TIM_REG) = 10 *
            timerPeriod;
    HWREG(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_WD_CTRL_REG)   = 1;

    pnHandle->icssWachDogTimerPeriod = 10 * timerPeriod;
    pnHandle->icssWatchDogEnabled = 1;

    return 0;
}
#endif


int32_t PN_loadStaticTable(ICSS_EmacBaseAddressHandle_T
                           ICSS_EmacBaseAddressHandle,
                           const uint32_t *staticTable,
                           uint8_t staticTableLength,
                           uint8_t staticTableType,
                           uint8_t portNumber)
{
    int32_t i;
    int32_t staticTableOffset;

    uint32_t temp_addr = 0U;
    int32_t ret_val = 0;

    if(portNumber == (uint8_t)ICSS_EMAC_PORT_1)
    {
        if(staticTableType == STATIC_FORWARD_TABLE)
        {
            staticTableOffset = (int32_t)STATIC_MAC_TABLE_FWD_PORT1;
        }

        else if(staticTableType == STATIC_RECEIVE_TABLE)
        {
            staticTableOffset = (int32_t)STATIC_MAC_TABLE_RCV_PORT1;
        }

        else
        {
            ret_val = -1;
        }
    }

    else if(portNumber == (uint8_t)ICSS_EMAC_PORT_2)
    {
        if(staticTableType == STATIC_FORWARD_TABLE)
        {
            staticTableOffset = (int32_t)STATIC_MAC_TABLE_FWD_PORT2;
        }

        else if(staticTableType == STATIC_RECEIVE_TABLE)
        {
            staticTableOffset = (int32_t)STATIC_MAC_TABLE_RCV_PORT2;
        }

        else
        {
            ret_val = -1;
        }
    }

    else
    {
        ret_val = -1;
    }

    if(ret_val == 0)
    {
        for(i = 0; i < (int32_t)staticTableLength; i++)
        {
            temp_addr = (ICSS_EmacBaseAddressHandle->sharedDataRamBaseAddr + ((uint32_t)(
                             staticTableOffset)) + (((uint32_t)i) << 2U));
            HWREG(temp_addr) = staticTable[i];
        }
    }

    return ret_val;
}
