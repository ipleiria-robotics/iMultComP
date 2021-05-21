/**
 * \file hal.c
 * \brief File to define HAL related functions
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
#include <ti/ndk/inc/stkmain.h>
#include <Include/protocol/eip_main.h>
#include <Include/protocol/hal.h>

#include <ti/drv/spi/SPIver.h>
#include <ti/drv/spi/test/qspi_flash/src/Flash_S25FL/S25FL.h>

#include <Include/board/board_tlkphy.h>
#include <Include/board/board_spi.h>
#include <Include/board/mdio_drv.h>
#include <Include/protocol/eip_soc.h>


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/**Structure to store Ethernet interface control parameters*/
ST_ETH_CONTROL          gstEthControl[2];
/**Structure to store TCP/IP control parameters*/
ST_TCPIP_CONTROL        gstTcpIpControl;
/**Structure to store TCP/IP Configuration  parameters*/
ST_TCPIP_CONFIGURATION  gstTcpIpConfiguration;
/**Structure to store initial TCP/IP Configuration  parameters*/
extern ST_TCPIP_CONFIGURATION  eipDevTcpIpConfiguration;
DWORD ifConnfigACDStat = 0;

EIP_WORD gTcpInactivityTimeOut = 120; /* Default value 120 Sec*/
/**Host Name of the Device*/
EIP_CHAR gszHostName[65];
/**EMAC LLD handle*/
extern ICSSEMAC_Handle emachandle;
/**Flash handle*/
extern S25FL_Handle flashHandle;

tlkMDIXTaskParam_t tlkTaskparam;
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
/*
 *
 */
/**
 *  @brief  The function is used to get the TCP Inactivity timeout
 *
 *  @param pTcpInactivityTimeOut [out] TcpInactivityTimeOut pointer
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getTcpInactivityTimeOut(EIP_WORD *pTcpInactivityTimeOut)
{
    *pTcpInactivityTimeOut = gTcpInactivityTimeOut;
    return EIP_TRUE;
}

/**
 *  @brief  The function sets teh TCP Inactivity timeout
 *
 *  @param TcpInactivityTimeOut [int] Time out value
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_setTcpInactivityTimeOut(EIP_WORD TcpInactivityTimeOut)
{
    gTcpInactivityTimeOut = TcpInactivityTimeOut;
    return EIP_TRUE;
}
/**
 *  @brief  The function informs about the capability of Ethernetip Implementation
 *
 *  @param pstCapability [out] Capability structure pointer
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getCapability(ST_CAPABILITY *pstCapability)
{
    pstCapability->wInternalETHInstance = 1;
    pstCapability->bF6RequiresReset = EIP_TRUE;
    pstCapability->wF5IfConfigurationStatus                   =
        F5_ATTR1_OBTAINED_BOOTP_DHCP_STOREDVALUE;
    pstCapability->stF5ConfCapability.bBootpClient            = EIP_FALSE;
    pstCapability->stF5ConfCapability.bDnsClient              = EIP_FALSE;
    pstCapability->stF5ConfCapability.bDhcpClient             = EIP_TRUE;
    pstCapability->stF5ConfCapability.bDhcpDnsUpdate          = EIP_FALSE;
    pstCapability->stF5ConfCapability.bConfigurationSettable  = EIP_TRUE;
    pstCapability->stF5ConfCapability.bHostNameSettable       = EIP_TRUE;
    pstCapability->stF5ConfCapability.bHardwareConfigurable   = EIP_FALSE;
    pstCapability->stF5ConfCapability.bMcastAndTtlSettable    = EIP_FALSE;
    pstCapability->stF5ConfCapability.bIfConfRequiresReset    = EIP_TRUE;
    pstCapability->stF5ConfCapability.bAcdCapable             =
        EIP_TRUE;

    return EIP_TRUE;
}
/**
 *  @brief  The function is used to get the Interface capability of the application
 *
 *
 *  @param wInstance             [in]   Ethernet instance
 *  @param pInterface_Capability [out] Interface capability structure pointer
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getInterfaceCapability(EIP_WORD wInstance,
                                       CLASS_ETH_INTERFACE_CAPABILIY *pInterface_Capability)
{
    EIP_BOOL bManualSettingRequiresReset = 1;
    EIP_BOOL bAutoNegotiate = 1;
    EIP_BOOL bAutoMDIX = 1;
    EIP_BOOL bManualSpeedDuplex = 1;


    pInterface_Capability->Capability_Bits |= bManualSettingRequiresReset << 0;
    pInterface_Capability->Capability_Bits |= bAutoNegotiate << 1;
    pInterface_Capability->Capability_Bits |= bAutoMDIX << 2;
    pInterface_Capability->Capability_Bits |= bManualSpeedDuplex << 3;

    pInterface_Capability->SpeedDuplexOptions.SpeedDuplexArrayCount = 4;

    if(pInterface_Capability->SpeedDuplexOptions.SpeedDuplexArrayCount >
            ETH_NUM_SPEED_DUPLEX_PAIR_MAX)
    {
        return EIP_FALSE;
    }

    pInterface_Capability->SpeedDuplexOptions.SpeedDuplex[0].Interface_Speed = 10;
    pInterface_Capability->SpeedDuplexOptions.SpeedDuplex[0].Interface_Duplex_Mode =
        0;
    pInterface_Capability->SpeedDuplexOptions.SpeedDuplex[1].Interface_Speed = 10;
    pInterface_Capability->SpeedDuplexOptions.SpeedDuplex[1].Interface_Duplex_Mode =
        1;
    pInterface_Capability->SpeedDuplexOptions.SpeedDuplex[2].Interface_Speed = 100;
    pInterface_Capability->SpeedDuplexOptions.SpeedDuplex[2].Interface_Duplex_Mode =
        0;
    pInterface_Capability->SpeedDuplexOptions.SpeedDuplex[3].Interface_Speed = 100;
    pInterface_Capability->SpeedDuplexOptions.SpeedDuplex[3].Interface_Duplex_Mode =
        1;

    return EIP_TRUE;
}

/*---------------------------------------------------------------------------*/
/* ----------------------------- [ TCP/IP ] -------------------------------- */
/*---------------------------------------------------------------------------*/
/**
 *  @brief  The function is used to get the Configuration control attribute of TCP/IP object
 *
 *
 *  @param pstF5ConfigurationControl [out]  TCPIP configuration control structure pointer
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getConfigurationControl(ST_TCPIP_CONTROL
                                        *pstF5ConfigurationControl)
{
    *pstF5ConfigurationControl = gstTcpIpControl;

    return EIP_TRUE;
}
/**
 *  @brief  The function is called to set Configuration control attribute of TCP/IP object
 *
 *
 *  @param pstF5ConfigurationControl [in]  TCP IP configuration control structure pointer
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_setConfigurationControl(ST_TCPIP_CONTROL
                                        *pstF5ConfigurationControl)
{
    EPROM_TCPIP tempTCPIPConfig;
    gstTcpIpControl = *pstF5ConfigurationControl;
    uint32_t blockNum, pageNum;

    tempTCPIPConfig.deviceIP = gstTcpIpConfiguration.IP_Address;
    tempTCPIPConfig.gatewayAddress = gstTcpIpConfiguration.Gateway_Address;
    tempTCPIPConfig.networkMask = gstTcpIpConfiguration.Network_Mask;
    tempTCPIPConfig.serverName = gstTcpIpConfiguration.Name_Server;

    switch(gstTcpIpControl.wStartupConfiguration)
    {
        case 2:
            tempTCPIPConfig.startupType = EIP_STARTUP_DHCP;

            Board_flashOffsetToBlkPage((Board_flashHandle)flashHandle, SPI_EEPROM_DEVICEIP_OFFSET,
                                       &blockNum, &pageNum);

            Board_flashEraseBlk((Board_flashHandle)flashHandle, blockNum);
            Board_flashWrite((Board_flashHandle)flashHandle, SPI_EEPROM_DEVICEIP_OFFSET,
                             (uint8_t *)&tempTCPIPConfig, sizeof(EPROM_TCPIP), NULL);

            break;

        case 0:
            tempTCPIPConfig.startupType = EIP_STARTUP_STATICIP;
            Board_flashOffsetToBlkPage((Board_flashHandle)flashHandle, SPI_EEPROM_DEVICEIP_OFFSET,
                                       &blockNum, &pageNum);

            Board_flashEraseBlk((Board_flashHandle)flashHandle, blockNum);
            Board_flashWrite((Board_flashHandle)flashHandle, SPI_EEPROM_DEVICEIP_OFFSET,
                             (uint8_t *)&tempTCPIPConfig, sizeof(EPROM_TCPIP), NULL);
            break;

        default:
            break;
    }

    return EIP_TRUE;
}
/**
 *  @brief  The function is used to get the Interface configuration attribute of TCP/IP object
 *
 *
 *  @param pTcpIpConfiguration [out]  TCP IP configuration structure pointer
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getInterfaceConfiguration(ST_TCPIP_CONFIGURATION
        *pTcpIpConfiguration)
{
    *pTcpIpConfiguration = gstTcpIpConfiguration;
    return EIP_TRUE;
}
/**
 *  @brief  The function is called to set Interface configuration attribute of TCP/IP object
 *
 *
 *  @param pTcpIpConfiguration [in] TCP IP configuration structure
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_setInterfaceConfiguration(ST_TCPIP_CONFIGURATION
        *pTcpIpConfiguration)
{
    EPROM_TCPIP tempTCPIPConfig;
    uint32_t blockNum, pageNum;

    tempTCPIPConfig.startupType = EIP_STARTUP_STATICIP;
    tempTCPIPConfig.deviceIP = pTcpIpConfiguration->IP_Address;
    tempTCPIPConfig.gatewayAddress = pTcpIpConfiguration->Gateway_Address;
    tempTCPIPConfig.networkMask = pTcpIpConfiguration->Network_Mask;
    tempTCPIPConfig.serverName = pTcpIpConfiguration->Name_Server;

    Board_flashOffsetToBlkPage((Board_flashHandle)flashHandle, SPI_EEPROM_DEVICEIP_OFFSET,
                               &blockNum, &pageNum);

    Board_flashEraseBlk((Board_flashHandle)flashHandle, blockNum);
    Board_flashWrite((Board_flashHandle)flashHandle, SPI_EEPROM_DEVICEIP_OFFSET,
                     (uint8_t *)&tempTCPIPConfig, sizeof(EPROM_TCPIP), NULL);

    gstTcpIpConfiguration = *pTcpIpConfiguration;
    return EIP_TRUE;
}
/**
 *  @brief  The function is called to obtain the Host Name
 *
 *
 *  @param pBuffer     [out]  Host name buffer
 *  @param wBufferSize [out]  size of hostname
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getHostName(EIP_CHAR *pBuffer, EIP_WORD *pwBufferSize)
{

    strncpy(pBuffer, gszHostName, *pwBufferSize);
    *pwBufferSize = (EIP_WORD)strlen(gszHostName);

    return EIP_TRUE;
}
/**
 *  @brief  Function to set the Host Name
 *
 *
 *  @param pBuffer     [in]  Host name buffer
 *  @param wBufferSize [in]  size of hostname
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_setHostName(EIP_CHAR *pBuffer, EIP_WORD wBufferSize)
{
    strncpy(gszHostName, pBuffer, wBufferSize);
    pBuffer[wBufferSize] = 0;

    return EIP_TRUE;
}

/**
 *  @brief  Function to get the SafetyNetwork Number (Not Implemented)
 *
 *
 *  @param   pSafetyNetworkNumber  [out]  Safety network number
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getSafetyNetworkNumber(EIP_BYTE *pSafetyNetworkNumber)
{
    pSafetyNetworkNumber[0] = 1;
    pSafetyNetworkNumber[1] = 2;
    pSafetyNetworkNumber[2] = 3;
    pSafetyNetworkNumber[3] = 4;
    pSafetyNetworkNumber[4] = 5;
    pSafetyNetworkNumber[5] = 6;

    return EIP_TRUE;
}

/*---------------------------------------------------------------------------*/
/* ----------------------------- [ ETHERNET ] ------------------------------ */
/*---------------------------------------------------------------------------*/

/**
 *  @brief  Function to get the Interface speed
 *
 *
 *  @param   wInstance  [in]  Ethernet instance
 *  @param   pdwIfSpeed [out] Interface Speed
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getInterfaceSpeed(EIP_WORD wInstance, EIP_DWORD *pdwIfSpeed)
{
    uint8_t phyVal;

    if(((ICSS_EmacObject *)emachandle->object)->linkStatus[wInstance - 1])
    {
        phyVal = MDIO_getPhyConfig((((ICSS_EmacHwAttrs *)
                                     emachandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs,
                                   (((ICSS_EmacObject *)emachandle->object)->emacInitcfg)->phyAddr[wInstance
                                           - 1]);

        if((phyVal == PHY_CONFIG_100FD) || (phyVal == PHY_CONFIG_100HD))
        {
            gstEthControl[wInstance - 1].wForceInterfaceSpeed = 100;
        }

        else if((phyVal == PHY_CONFIG_10FD) || (phyVal == PHY_CONFIG_10HD))
        {
            gstEthControl[wInstance - 1].wForceInterfaceSpeed = 10;
        }

        *pdwIfSpeed = gstEthControl[wInstance - 1].wForceInterfaceSpeed;
    }

    else
    {
        *pdwIfSpeed = 0;
    }

    return EIP_TRUE;
}

/**
 *  @brief  Function to get the Interface properites
 *
 *
 *  @param   wInstance           [in]  Ethernet instance
 *  @param   pstF6InterfaceFlags [out] Interface flag structure pointer
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getInterfaceFlags(EIP_WORD wInstance,
                                  ST_F6_INTERFACE_FLAGS *pstF6InterfaceFlags)
{
    uint8_t phyVal;
    pstF6InterfaceFlags->bLinkStatus          = ((ICSS_EmacObject *)
            emachandle->object)->linkStatus[wInstance - 1];

    if(pstF6InterfaceFlags->bLinkStatus)
    {
        phyVal = MDIO_getPhyConfig((((ICSS_EmacHwAttrs *)
                                     emachandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs,
                                   (((ICSS_EmacObject *)emachandle->object)->emacInitcfg)->phyAddr[wInstance
                                           - 1]);

        if((phyVal == PHY_CONFIG_100FD) || (phyVal == PHY_CONFIG_10FD))
        {
            pstF6InterfaceFlags->bFullDuplex          = 1;
        }

        else if((phyVal == PHY_CONFIG_100HD) || (phyVal == PHY_CONFIG_10HD))
        {
            pstF6InterfaceFlags->bFullDuplex          = 0;
        }

        if(MDIO_getAutoNegCompleteStat((((ICSS_EmacHwAttrs *)
                                         emachandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs,
                                       (((ICSS_EmacObject *)emachandle->object)->emacInitcfg)->phyAddr[wInstance
                                               - 1]))
        {
            if(MDIO_getLinkPartnerAutonegAble((((ICSS_EmacHwAttrs *)
                                                emachandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs,
                                              (((ICSS_EmacObject *)emachandle->object)->emacInitcfg)->phyAddr[wInstance
                                                      - 1]))
            {
                pstF6InterfaceFlags->wNegotiationStatus   = F6_ATTR2_AUTO_NEG_SUCCESS;
            }

            else
            {
                pstF6InterfaceFlags->wNegotiationStatus   = F6_ATTR2_AUTO_NEG_NOT_ATTEMPTED;
            }
        }

        else
        {
            pstF6InterfaceFlags->wNegotiationStatus   = F6_ATTR2_AUTO_NEG_NOT_ATTEMPTED;
        }
    }

    else
    {
        pstF6InterfaceFlags->wNegotiationStatus = F6_ATTR2_AUTO_NEG_NOT_ATTEMPTED;
        pstF6InterfaceFlags->bFullDuplex = 0;

    }

    pstF6InterfaceFlags->bLocalHardwareFault  = EIP_FALSE;

    return EIP_TRUE;
}
/**
 *  @brief  Function to get the Port statistics
 *
 *          Uses EMAC LLD Ioctl API to get the statistics
 *
 *  @param   wInstance  [in]  Ethernet instance
 *  @param   pIfCnt     [out] structure where statistics will be returned
 *  @param   bClear     [in]  specifies whether to clear statistics or not
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getInterfaceCounters(EIP_WORD wInstance,
                                     CLASS_ETH_LINK_INTERFACE_COUNTERS *pIfCnt, EIP_BOOL bClear)
{

#if TEMP
    static EIP_DWORD dwValue;
    NIMU_IF_REQ nimudevice;
    nimudevice.index = 1;
    nimudevice.name[0] = 0;

    StatsCmd statStr;
    statStr.opcode = ICSS_IOCTL_STAT_GET;
    statStr.portNum = wInstance;

    NIMUIoctl(ICSS_IOCTL_STAT_CMD, &nimudevice, &statStr, sizeof(statStr));
    pIfCnt->In_Octets           = statStr.statsObj.rxNetOctets;
    pIfCnt->In_Ucast_Packets    = statStr.statsObj.rxUcast;
    pIfCnt->In_NUcast_Packets   = statStr.statsObj.rxBcast +
                                  statStr.statsObj.rxMcast;
    pIfCnt->In_Errors           = 0;
    pIfCnt->In_Unknown_Protos   = statStr.statsObj.rxUnknownProtocol;
    pIfCnt->Out_Octets          = statStr.statsObj.txNetOctets;
    pIfCnt->Out_Ucast_Packets   = statStr.statsObj.txUcast;
    pIfCnt->Out_NUcast_Packets  = statStr.statsObj.txBcast +
                                  statStr.statsObj.txMcast;
    pIfCnt->Out_Discards        = 0;
    pIfCnt->Out_Errors          = 0;

    if(bClear)
    {
        dwValue = 0;
        statStr.opcode = ICSS_IOCTL_STAT_CLEAR;
        NIMUIoctl(ICSS_IOCTL_STAT_CMD, &nimudevice, &statStr, sizeof(statStr));
    }

    else
    {
        dwValue++;
    }

#endif
    return EIP_TRUE;
}

/**
 *  @brief  Function to get the Media Counters
 *
 *  @param   wInstance  [in]  Ethernet instance
 *  @param   pMediaCnt  [out] structure where data will be returned
 *  @param   bClear     [in]  specifies whether to clear media counters or not
 *
 *  @retval  EIP_TRUE  - On success
 *
 */

EIP_BOOL EIPHAL_getMediaCounters(EIP_WORD wInstance,
                                 CLASS_ETH_MEDIA_COUNTERS *pMediaCnt,
                                 EIP_BOOL bClear)
{
    ICSSEMAC_IoctlCmd ioctlParams;

    ICSS_EmacPruStatistics_t *pruStatStructPtr = (ICSS_EmacPruStatistics_t *)(((
                ICSS_EmacObject *)emachandle->object)->pruStat);

    if(bClear)
    {
        /*CLEAR MEDIA COUNTERS*/
        ioctlParams.command = ICSS_EMAC_IOCTL_STAT_CTRL_CLEAR;
        ICSS_EmacIoctl(emachandle, ICSS_EMAC_IOCTL_STATS_CTRL,
                       ICSS_EMAC_PORT_1 + wInstance - 1, (void *)&ioctlParams);
    }

    else
    {

        /*READ MEDIA COUNTERS*/

        ioctlParams.command = ICSS_EMAC_IOCTL_STAT_CTRL_GET;
        ICSS_EmacIoctl(emachandle, ICSS_EMAC_IOCTL_STATS_CTRL,
                       ICSS_EMAC_PORT_1 + wInstance - 1, (void *)&ioctlParams);

        pMediaCnt->Alignment_Errors = (pruStatStructPtr + wInstance -
                                       1)->rxMisAlignmentFrames;
        pMediaCnt->FCS_Errors = (pruStatStructPtr + wInstance - 1)->SFDError;
        pMediaCnt->Single_Collisions = (pruStatStructPtr + wInstance - 1)->singleColl;
        pMediaCnt->Multiple_Collisions = (pruStatStructPtr + wInstance - 1)->multiColl;
        pMediaCnt->Deferred_Transmissions = (pruStatStructPtr + wInstance - 1)->defTx;
        pMediaCnt->Late_Collisions = (pruStatStructPtr + wInstance - 1)->lateColl;
        pMediaCnt->Excessive_Collisions = (pruStatStructPtr + wInstance -
                                           1)->excessColl;
        pMediaCnt->MAC_Transmit_Errors = (pruStatStructPtr + wInstance - 1)->macTxError;
        pMediaCnt->Frame_Too_Long = (pruStatStructPtr + wInstance -
                                     1)->rxOverSizedFrames;
        pMediaCnt->MAC_Receive_Errors = (pruStatStructPtr + wInstance - 1)->macRxError;
        pMediaCnt->SQE_Test_Errors        =
            0x0;    // Not supported in FW, SQETestError is not an applicable statistic
        pMediaCnt->Carrier_Sense_Errors   =
            0x0;    // Not supported in FW, CSError can be gathered from PHY status directly

    }

    return EIP_TRUE;
}
/**
 *  @brief  Function to get the Mac address of the device
 *
 *          Uses EMAC LLD IOCTL to get the details
 *
 *  @param   wInstance     [in]  Ethernet instance
 *  @param   pMacAddress   [out] Mac Address pointer
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getPhysicalAddress(EIP_WORD wInstance, EIP_BYTE *pMacAddress)
{
    NIMU_IF_REQ device;
    device.index = 1;
    device.name[0] = 0;

    NIMUIoctl(NIMU_GET_DEVICE_MAC, &device, (Uint8 *)pMacAddress, 6);

    return EIP_TRUE;
}

/**
 *  @brief  The function returns the Interface configuration attribute of TCP/IP object
 *
 *
 *  @param   wInstance     [in] Ethernet instance
 *  @param   pstEthControl [out] Structure pointer to get the attribute details
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getInterfaceControl(EIP_WORD wInstance,
                                    ST_ETH_CONTROL *pstEthControl)
{
    uint8_t phyVal;

    if(((ICSS_EmacObject *)emachandle->object)->linkStatus[wInstance - 1])
    {
        if(gstEthControl[wInstance - 1].bAutoNegotiate)
        {
            gstEthControl[wInstance - 1].wForceInterfaceSpeed = 0;
        }

        else
        {
            phyVal = MDIO_getPhyConfig((((ICSS_EmacHwAttrs *)
                                         emachandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs,
                                       (((ICSS_EmacObject *)emachandle->object)->emacInitcfg)->phyAddr[wInstance
                                               - 1]);

            if((phyVal == PHY_CONFIG_100FD) || (phyVal == PHY_CONFIG_100HD))
            {
                gstEthControl[wInstance - 1].wForceInterfaceSpeed = 100;
            }

            else if((phyVal == PHY_CONFIG_10FD) || (phyVal == PHY_CONFIG_10HD))
            {
                gstEthControl[wInstance - 1].wForceInterfaceSpeed = 10;
            }


        }
    }

    *pstEthControl = gstEthControl[wInstance - 1];
    return EIP_TRUE;
}

/**
 *  @brief  The function sets the Interface configuration attribute of TCP/IP object
 *
 *           Stores the details in non volatile storage and update the global variable
 *
 *  @param   wInstance     [in] Ethernet instance
 *  @param   pstEthControl [in] Structure pointer to get the attribute details
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_setInterfaceControl(EIP_WORD wInstance,
                                    ST_ETH_CONTROL *pstEthControl)
{
    uint32_t blockNum, pageNum;
    Board_flashOffsetToBlkPage((Board_flashHandle)flashHandle, SPI_EEPROM_SPEED_OFFSET,
                               &blockNum, &pageNum);

    Board_flashEraseBlk((Board_flashHandle)flashHandle, blockNum);
    Board_flashWrite((Board_flashHandle)flashHandle, SPI_EEPROM_SPEED_OFFSET,
                     (uint8_t *)pstEthControl, sizeof(ST_ETH_CONTROL), NULL);

    gstEthControl[0] = *pstEthControl;
    gstEthControl[1] = *pstEthControl;

    return EIP_TRUE;
}

/**
 *  @brief  Function to get the startup type of the application (Static ip or DHCP)
 *
 *  @param   none
 *  @retval  EIP_TRUE  - In case of DHCP
 *           EIP_FALSE - In case of Static IP
 *
 */
EIP_BOOL EIPHAL_getInterfaceStarupType()
{

    if(eipDevInitConfig.tcpipConfig.startupType == EIP_STARTUP_DHCP)
    {
        return EIP_TRUE;
    }

    else
    {
        return EIP_FALSE;
    }
}
/**
 *  @brief  Function to get the Incarnation ID used by the application
 *
 *  @param   incarnVal [out] incarnation ID
 *  @retval  none
 *
 */
EIP_VOID EIPHAL_getIncarnID(uint16_t *incarnVal)
{
    *incarnVal = eipDevInitConfig.incarnNum;
}
/**
 *  @brief  Function to set the Incarnation ID
 *
 *          Writes the details into non volatile memory
 *
 *  @param   incarnVal [in] incarnation ID
 *  @retval  none
 *
 */
EIP_VOID EIPHAL_setIncarnID(uint16_t *incarnVal)
{
    uint16_t test;
    test = *incarnVal;
    uint32_t blockNum, pageNum;
    Board_flashOffsetToBlkPage((Board_flashHandle)flashHandle, SPI_EEPROM_INCARN_OFFSET,
                               &blockNum, &pageNum);

    Board_flashEraseBlk((Board_flashHandle)flashHandle, blockNum);
    Board_flashWrite((Board_flashHandle)flashHandle, SPI_EEPROM_INCARN_OFFSET,
                     (uint8_t *)&test, sizeof(uint16_t), NULL);
}
/**
 *  @brief  Function to get the ACD Conflict details from the application
 *
 *          Informs ACD stack about the Last conflict details
 *
 *  @param   acdEnable [out] ACD enable flag
 *  @retval  none
 *
 */
EIP_VOID EIPHAL_getLastConflictDetails(CLASS_TCP_LAST_CONFLICT_DETECTED
                                       *lastconflictdetails)
{
    * lastconflictdetails = eipDevInitConfig.acdConflictDetails;
}
/**
 *  @brief  Function to set the ACD Conflict details to the Nonvolatile memory
 *
 *          Writes the details into the non volatile memory. Also update the config variable
 *
 *  @param   lastconflictdetails [in] ACD conflict details structure pointer
 *  @retval  none
 *
 */
EIP_VOID EIPHAL_setLastConflictDetails(CLASS_TCP_LAST_CONFLICT_DETECTED
                                       *lastconflictdetails)
{
    uint32_t blockNum, pageNum;
    Board_flashOffsetToBlkPage((Board_flashHandle)flashHandle, SPI_EEPROM_ACD_CONFLICT_OFFSET,
                               &blockNum, &pageNum);

    Board_flashEraseBlk((Board_flashHandle)flashHandle, blockNum);
    Board_flashWrite((Board_flashHandle)flashHandle, SPI_EEPROM_ACD_CONFLICT_OFFSET,
                     (uint8_t *)lastconflictdetails, sizeof(CLASS_TCP_LAST_CONFLICT_DETECTED), NULL);

    eipDevInitConfig.acdConflictDetails = *
                                          lastconflictdetails;
}
/**
 *  @brief  Function to get the ACD Status
 *
 *          Returns whether Conflict detection has happened or not
 *
 *  @param   acdStatus [out] ACD Status
 *  @retval  none
 *
 */
EIP_VOID EIPHAL_getInterfaceConfigACDStatus(DWORD *acdStatus)
{
    *acdStatus |= ifConnfigACDStat;
}

/**
 *  @brief  Function to get the ACD enable value from teh application
 *
 *  @param   acdEnable [out] ACD enable flag pointer
 *  @retval  none
 *
 */
EIP_VOID EIPHAL_getACDEnable(EIP_WORD *acdEnable)
{
    * acdEnable = eipDevInitConfig.acdEnable;
}

/**
 *  @brief  Function to Set the ACD enable in non volatile memory
 *
 *          Writes the ACD enable value into the non volatile memory
 *
 *  @param   acdEnable [in] ACD enable flag pointer
 *  @retval  none
 *
 */
EIP_VOID EIPHAL_setACDEnable(EIP_WORD *acdEnable)
{
    uint32_t blockNum, pageNum;
    Board_flashOffsetToBlkPage((Board_flashHandle)flashHandle, SPI_EEPROM_ACD_OFFSET,
                               &blockNum, &pageNum);

    Board_flashEraseBlk((Board_flashHandle)flashHandle, blockNum);
    Board_flashWrite((Board_flashHandle)flashHandle, SPI_EEPROM_ACD_OFFSET,
                     (uint8_t *)acdEnable, sizeof(EIP_WORD), NULL);
}
/**
 *  @brief  Function to Set the QOS values
 *
 *          Writes the QOS values into the non volatile memory
 *
 *  @param   lpQosObj [in] Qos object structure pointer
 *  @retval  Success  -   EIP_TRUE
 *
 */
EIP_BOOL EIPHAL_setQoSValues(CLASS_QOS *lpQosObj)
{
    uint32_t blockNum, pageNum;
    Board_flashOffsetToBlkPage((Board_flashHandle)flashHandle, SPI_EEPROM_QOS_OFFSET,
                               &blockNum, &pageNum);

    Board_flashEraseBlk((Board_flashHandle)flashHandle, blockNum);
    Board_flashWrite((Board_flashHandle)flashHandle, SPI_EEPROM_QOS_OFFSET,
                     (uint8_t *)lpQosObj, sizeof(CLASS_QOS), NULL);

    return EIP_TRUE;
}
/**
 *  @brief  Function to get the QOS values used by the application
 *
 *          Gets the QOS values stored in the non volatile memory
 *
 *  @param   lpQosObj [out] Qos object structure pointer
 *  @retval  Success  -   EIP_TRUE
 *
 */
EIP_BOOL EIPHAL_getQoSValues(CLASS_QOS *lpQosObj)
{
    Board_flashRead((Board_flashHandle)flashHandle, SPI_EEPROM_QOS_OFFSET,
                    (uint8_t *)lpQosObj, sizeof(CLASS_QOS), NULL);
    return EIP_TRUE;
}
/**
 *  @brief  The function calls ndk driver ioctl to force the Interface properties
 *
 *
 *  @param   pstEthControl [in] structure having the Interface properties
 *  @retval  none
 *
 */
EIP_VOID EIPHAL_setInterfaceSpeed(ST_ETH_CONTROL *pstEthControl)
{
    if(0 == pstEthControl->bAutoNegotiate)
    {
        if(pstEthControl->wForceInterfaceSpeed == 10)
        {
            if(pstEthControl->bForceFullDuplex)
            {
                MDIO_setPhyConfig((((ICSS_EmacHwAttrs *)
                                    emachandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs,
                                  ((ICSS_EmacObject *)emachandle->object)->emacInitcfg->phyAddr[0],
                                  PHY_CONFIG_10FD);
                MDIO_setPhyConfig((((ICSS_EmacHwAttrs *)
                                    emachandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs,
                                  ((ICSS_EmacObject *)emachandle->object)->emacInitcfg->phyAddr[1],
                                  PHY_CONFIG_10FD);
            }

            else
            {
                MDIO_setPhyConfig((((ICSS_EmacHwAttrs *)
                                    emachandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs,
                                  ((ICSS_EmacObject *)emachandle->object)->emacInitcfg->phyAddr[0],
                                  PHY_CONFIG_10HD);
                MDIO_setPhyConfig((((ICSS_EmacHwAttrs *)
                                    emachandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs,
                                  ((ICSS_EmacObject *)emachandle->object)->emacInitcfg->phyAddr[1],
                                  PHY_CONFIG_10HD);
            }
        }

        else if(pstEthControl->wForceInterfaceSpeed == 100)
        {
            if(pstEthControl->bForceFullDuplex)
            {
                MDIO_setPhyConfig((((ICSS_EmacHwAttrs *)
                                    emachandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs,
                                  ((ICSS_EmacObject *)emachandle->object)->emacInitcfg->phyAddr[0],
                                  PHY_CONFIG_100FD);
                MDIO_setPhyConfig((((ICSS_EmacHwAttrs *)
                                    emachandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs,
                                  ((ICSS_EmacObject *)emachandle->object)->emacInitcfg->phyAddr[1],
                                  PHY_CONFIG_100FD);
            }

            else
            {
                MDIO_setPhyConfig((((ICSS_EmacHwAttrs *)
                                    emachandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs,
                                  ((ICSS_EmacObject *)emachandle->object)->emacInitcfg->phyAddr[0],
                                  PHY_CONFIG_100HD);
                MDIO_setPhyConfig((((ICSS_EmacHwAttrs *)
                                    emachandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs,
                                  ((ICSS_EmacObject *)emachandle->object)->emacInitcfg->phyAddr[1],
                                  PHY_CONFIG_100HD);
            }
        }
    }

    else
    {

        pstEthControl->bAutoNegotiate = 1;
        pstEthControl->bForceFullDuplex = 0;
        pstEthControl->wForceInterfaceSpeed = 0;

        MDIO_setPhyConfig((((ICSS_EmacHwAttrs *)
                            emachandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs,
                          ((ICSS_EmacObject *)emachandle->object)->emacInitcfg->phyAddr[0],
                          PHY_CONFIG_AUTONEG);
        MDIO_setPhyConfig((((ICSS_EmacHwAttrs *)
                            emachandle->hwAttrs)->emacBaseAddrCfg)->prussMiiMdioRegs,
                          ((ICSS_EmacObject *)emachandle->object)->emacInitcfg->phyAddr[1],
                          PHY_CONFIG_AUTONEG);
    }
}

/**
 *  @brief  Function to set Ethernet Link configuration
 *
 *          Reads the Ethernet Link configuration from Flash and configures the PHY in
 *          desired mode
 *
 *  @param   none
 *  @retval  Success  -   EIP_TRUE
 *
 */
EIP_BOOL EIPHAL_setEthernetLinkConfig()
{
    Board_flashRead((Board_flashHandle)flashHandle, SPI_EEPROM_SPEED_OFFSET,
                    (uint8_t *)&gstEthControl[0], sizeof(ST_ETH_CONTROL), NULL);
    Board_flashRead((Board_flashHandle)flashHandle, SPI_EEPROM_SPEED_OFFSET,
                    (uint8_t *)&gstEthControl[1], sizeof(ST_ETH_CONTROL), NULL);
    EIPHAL_setInterfaceSpeed(&gstEthControl[0]);

    tlkTaskparam.mdioBaseAddress = ((ICSS_EmacHwAttrs *)
                                    emachandle->hwAttrs)->emacBaseAddrCfg->prussMiiMdioRegs;
    tlkTaskparam.numPorts = 2;
    tlkTaskparam.phyAddress[0] = ((ICSS_EmacObject *)
                                  emachandle->object)->emacInitcfg->phyAddr[0];
    tlkTaskparam.phyAddress[1] = ((ICSS_EmacObject *)
                                  emachandle->object)->emacInitcfg->phyAddr[1];
    Board_phyMDIXFixInit(&tlkTaskparam);

    return EIP_TRUE;
}
/**
 *  @brief  Function to get the currnet IP address and other related details
 *
 *
 *  @param   none
 *  @retval  Success  -   EIP_TRUE
 *
 */
EIP_BOOL EIPHAL_setTCPIPInitConfig()
{
    EIPNDK_getTCPIPConfigDetails(&eipDevTcpIpConfiguration);
    gstTcpIpControl.wStartupConfiguration =
        eipDevInitConfig.tcpipConfig.startupType;
    gstTcpIpConfiguration = eipDevTcpIpConfiguration;
    return EIP_TRUE;
}
