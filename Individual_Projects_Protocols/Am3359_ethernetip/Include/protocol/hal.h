/*
+------------------------------------------------------------------------------+
| Woodhead Software & Electronics                                              |
| -------------------------------                                              |
|                                                                              |
|  Copyright 2007 (c)   Woodhead Software & Electronics. All rights reserved.  |
+------------------------------------------------------------------------------+
  Description .......:  User implementation of the EIP sample                                                        
                       
+------------------------------------------------------------------------------+
  $Workfile: hal.h $
  $Revision: 5 $
  $Date: 31/05/11 16:03 $
+------------------------------------------------------------------------------+
  Version | Date     | Author               | Comments
  1.0.0     05/01/11   E.LUDE     
+------------------------------------------------------------------------------+
*/

#ifndef HAL__H
#define HAL__H

// ------------------------------------------------------------------------------
// .............................. [DEFINE] ......................................
// ------------------------------------------------------------------------------


// Object 0xF5, Attribut 1 => Status
// Interface Configuration Status
#define F5_ATTR1_NOT_CONFIGURED                   0x0
#define F5_ATTR1_OBTAINED_BOOTP_DHCP_STOREDVALUE  0x1
#define F5_ATTR1_OBTAINED_HARDWARE_SETTING        0x2


// Object 0xF6, Attribut 2 => Interface Flags
// Negociation Status
#define F6_ATTR2_AUTO_NEG_IN_PROGRESS     0x0
#define F6_ATTR2_AUTO_NEG_SPEED_FAILED    0x1
#define F6_ATTR2_AUTO_NEG_DUPLEX_FAILED   0x2
#define F6_ATTR2_AUTO_NEG_SUCCESS         0x3
#define F6_ATTR2_AUTO_NEG_NOT_ATTEMPTED   0x4

// ------------------------------------------------------------------------------
// .............................. [STRUCTURE] ...................................
// ------------------------------------------------------------------------------


typedef struct
{ 
  EIP_BOOL bManualSettingRequiresReset;
  EIP_BOOL bAutoNegotiate;
  EIP_BOOL bAutoMDIX;
  EIP_BOOL bManualSpeedDuplex;
}ST_F6_INTERFACE_CAPABILITY, EIP_NEAR * PST_F6_INTERFACE_CAPABILITY, EIP_FAR * LPST_F6_INTERFACE_CAPABILITY;


typedef struct
{ 
    EIP_BOOL bBootpClient;
    EIP_BOOL bDnsClient;
    EIP_BOOL bDhcpClient;
    EIP_BOOL bDhcpDnsUpdate;
    EIP_BOOL bConfigurationSettable;
    EIP_BOOL bHostNameSettable;
    EIP_BOOL bHardwareConfigurable;
    EIP_BOOL bMcastAndTtlSettable;
    EIP_BOOL bIfConfRequiresReset;
    EIP_BOOL bAcdCapable;
}ST_F5_CONF_CAPABILITY, EIP_NEAR * PST_F5_CONF_CAPABILITY, EIP_FAR * LPST_F5_CONF_CAPABILITY;

typedef struct
{ 
    EIP_BOOL bF6RequiresReset;
    EIP_WORD wInternalETHInstance;
    EIP_WORD wF5IfConfigurationStatus;
    ST_F5_CONF_CAPABILITY stF5ConfCapability;
}ST_CAPABILITY, EIP_NEAR * PST_CAPABILITY, EIP_FAR * LPST_CAPABILITY;

// TCP/IP
typedef struct
{
    EIP_WORD wStartupConfiguration;
    EIP_BOOL bDnsEnable;
}ST_TCPIP_CONTROL, EIP_NEAR * PST_TCPIP_CONTROL, EIP_FAR * LPST_TCPIP_CONTROL;


typedef struct
{
  EIP_DWORD IP_Address;
  EIP_DWORD Network_Mask;
  EIP_DWORD Gateway_Address;
  EIP_DWORD Name_Server;
  EIP_DWORD Name_Server_2;
  EIP_WORD  Domain_Name_Length;
  EIP_CHAR  Domain_Name[TCPIP_ATTR_DOMAINE_NAME_SIZE];
}ST_TCPIP_CONFIGURATION, EIP_NEAR * PST_TCPIP_CONFIGURATION, EIP_FAR * LPST_TCPIP_CONFIGURATION;

// Ethernet
typedef struct
{
    EIP_BOOL bLinkStatus;
    EIP_BOOL bFullDuplex;
    EIP_WORD wNegotiationStatus;
    EIP_BOOL bLocalHardwareFault;
}ST_F6_INTERFACE_FLAGS, EIP_NEAR * PST_F6_INTERFACE_FLAGS, EIP_FAR * LPST_F6_INTERFACE_FLAGS;

typedef struct
{
    EIP_BOOL bAutoNegotiate;        // 1 = Auto
    EIP_BOOL bForceFullDuplex;      // 1 = Full duplex  -  0 = Half duplex
    EIP_WORD wForceInterfaceSpeed;  // 10, 100
}ST_ETH_CONTROL, EIP_NEAR * PST_ETH_CONTROL, EIP_FAR * LPST_ETH_CONTROL;


// ------------------------------------------------------------------------------
// .............................. [FUNCTION] ....................................
// ------------------------------------------------------------------------------
/**
 *  @brief  The function is used to get the TCP Inactivity timeout
 *
 *  @param pTcpInactivityTimeOut [out] TcpInactivityTimeOut pointer
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getTcpInactivityTimeOut(EIP_WORD * pTcpInactivityTimeOut);
/**
 *  @brief  The function sets teh TCP Inactivity timeout
 *
 *  @param TcpInactivityTimeOut [int] Time out value
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_setTcpInactivityTimeOut(EIP_WORD TcpInactivityTimeOut);
/**
 *  @brief  The function informs about the capability of Ethernetip Implementation
 *
 *  @param pstCapability [out] Capability structure pointer
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getCapability(ST_CAPABILITY * pstCapability);

/**
 *  @brief  The function is used to get the Configuration control attribute of TCP/IP object
 *
 *
 *  @param pstF5ConfigurationControl [out]  TCPIP configuration control structure pointer
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getConfigurationControl(ST_TCPIP_CONTROL * pstF5ConfigurationControl);
/**
 *  @brief  The function is called to set Configuration control attribute of TCP/IP object
 *
 *
 *  @param pstF5ConfigurationControl [in]  TCP IP configuration control structure pointer
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_setConfigurationControl(ST_TCPIP_CONTROL * pstF5ConfigurationControl);
/**
 *  @brief  The function is used to get the Interface configuration attribute of TCP/IP object
 *
 *
 *  @param pTcpIpConfiguration [out]  TCP IP configuration structure pointer
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getInterfaceConfiguration(ST_TCPIP_CONFIGURATION * pTcpIpConfiguration);
/**
 *  @brief  The function is called to set Interface configuration attribute of TCP/IP object
 *
 *
 *  @param pTcpIpConfiguration [in] TCP IP configuration structure
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_setInterfaceConfiguration(ST_TCPIP_CONFIGURATION * pTcpIpConfiguration);
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
EIP_BOOL EIPHAL_getHostName(EIP_CHAR * pBuffer, EIP_WORD *pwBufferSize);
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
EIP_BOOL EIPHAL_setHostName(EIP_CHAR * pBuffer, EIP_WORD wBufferSize);
/**
 *  @brief  Function to get the SafetyNetwork Number (Not Implemented)
 *
 *
 *  @param   pSafetyNetworkNumber  [out]  Safety network number
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getSafetyNetworkNumber(EIP_BYTE * pSafetyNetworkNumber);


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
EIP_BOOL HAL_GetPhysicalAddress   (EIP_WORD wInstance, EIP_BYTE * pMacAddress);
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
EIP_BOOL EIPHAL_getInterfaceFlags    (EIP_WORD wInstance, ST_F6_INTERFACE_FLAGS * pstF6InterfaceFlags);
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
EIP_BOOL EIPHAL_getInterfaceSpeed    (EIP_WORD wInstance, EIP_DWORD * pdwIfSpeed);
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
EIP_BOOL EIPHAL_getInterfaceCounters (EIP_WORD wInstance, CLASS_ETH_LINK_INTERFACE_COUNTERS * pIfCnt, EIP_BOOL bClear);
/**
 *  @brief  Function to get the Media Counters
 *
 *          Not supported for the timebeing
 *
 *  @param   wInstance   [in]  Ethernet instance
 *  @param   pMediaCnt   [out] structure where data will be returned
 *  @param   bClear      [in]  specifies whether to clear media counters or not
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPHAL_getMediaCounters     (EIP_WORD wInstance, CLASS_ETH_MEDIA_COUNTERS * pMediaCnt, EIP_BOOL bClear);
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
EIP_BOOL EIPHAL_getInterfaceControl  (EIP_WORD wInstance, ST_ETH_CONTROL * pstF6InterfaceControl);
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
EIP_BOOL EIPHAL_setInterfaceControl(EIP_WORD wInstance, ST_ETH_CONTROL * pstEthControl);
/**
 *  @brief  Function to get the startup type of the application (Static ip or DHCP)
 *
 *  @param   none
 *  @retval  EIP_TRUE  - In case of DHCP
 *           EIP_FALSE - In case of Static IP
 *
 */
EIP_BOOL EIPHAL_getInterfaceStarupType();
/**
 *  @brief  Function to get the Incarnation ID used by the application
 *
 *  @param   incarnVal [out] incarnation ID
 *  @retval  none
 *
 */
EIP_VOID EIPHAL_getIncarnID(uint16_t *incarnVal);
/**
 *  @brief  Function to set the Incarnation ID
 *
 *          Writes the details into non volatile memory
 *
 *  @param   incarnVal [in] incarnation ID
 *  @retval  none
 *
 */
EIP_VOID EIPHAL_setIncarnID(uint16_t *incarnVal);
/**
 *  @brief  Function to set the ACD Conflict details to the Nonvolatile memory
 *
 *          Writes the details into the non volatile memory. Also update the config variable
 *
 *  @param   lastconflictdetails [in] ACD conflict details structure pointer
 *  @retval  none
 *
 */
EIP_VOID EIPHAL_setLastConflictDetails(CLASS_TCP_LAST_CONFLICT_DETECTED* lastconflictdetails);
/**
 *  @brief  Function to get the ACD Conflict details from the application
 *
 *          Informs ACD stack about the Last conflict details
 *
 *  @param   acdEnable [out] ACD enable flag
 *  @retval  none
 *
 */
EIP_VOID EIPHAL_getLastConflictDetails(CLASS_TCP_LAST_CONFLICT_DETECTED* lastconflictdetails);
/**
 *  @brief  Function to get the ACD Status
 *
 *  		Returns whether Conflict detection has happened or not
 *
 *  @param   acdStatus [out] ACD Status
 *  @retval  none
 *
 */
EIP_VOID EIPHAL_getInterfaceConfigACDStatus(DWORD * acdStatus);
/**
 *  @brief  Function to get the ACD enable value from teh application
 *
 *  @param   acdEnable [out] ACD enable flag pointer
 *  @retval  none
 *
 */
EIP_VOID EIPHAL_getACDEnable(EIP_WORD * acdEnable);
/**
 *  @brief  Function to Set the ACD enable in non volatile memory
 *
 *          Writes the ACD enable value into the non volatile memory
 *
 *  @param   acdEnable [in] ACD enable flag pointer
 *  @retval  none
 *
 */
EIP_VOID EIPHAL_setACDEnable(EIP_WORD * acdEnable);
/**
 *  @brief  Function to Set the QOS values
 *
 *          Writes the QOS values into the non volatile memory
 *
 *  @param   lpQosObj [in] Qos object structure pointer
 *  @retval  Success  -   EIP_TRUE
 *
 */
EIP_BOOL EIPHAL_setQoSValues(CLASS_QOS* lpQosObj);
/**
 *  @brief  Function to get the QOS values used by the application
 *
 *          Gets the QOS values stored in the non volatile memory
 *
 *  @param   lpQosObj [out] Qos object structure pointer
 *  @retval  Success  -   EIP_TRUE
 *
 */
EIP_BOOL EIPHAL_getQoSValues(CLASS_QOS * lpQosObj);
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
EIP_BOOL EIPHAL_getInterfaceCapability(EIP_WORD wInstance, CLASS_ETH_INTERFACE_CAPABILIY * pInterface_Capability);
/**
 *  @brief  The function calls ndk driver ioctl to force the Interface properties
 *
 *
 *  @param   pstEthControl [in] structure having the Interface properties
 *  @retval  none
 *
 */
EIP_VOID EIPHAL_setInterfaceSpeed(ST_ETH_CONTROL *pstEthControl);
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
EIP_BOOL EIPHAL_setEthernetLinkConfig();
/**
 *  @brief  Function to get the currnet IP address and other related details
 *
 *
 *  @param   none
 *  @retval  Success  -   EIP_TRUE
 *
 */
EIP_BOOL EIPHAL_setTCPIPInitConfig();
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif // DEVICE__H

/*---------------------------------------------------------------------------*/
/*----------------------------- END OF FILE ---------------------------------*/
/*---------------------------------------------------------------------------*/
