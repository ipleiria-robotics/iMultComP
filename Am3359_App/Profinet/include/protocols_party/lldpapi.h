/*---------------------------------------------------------------------------*/
/*  Copyright (C) 2006 Woodhead Software & Electonics. All rights reserved.  */
/*---------------------------------------------------------------------------*/
/*  This program is protected by international copyright.                    */
/*                                                                           */
/*  The use of this software including but not limited to its Source Code    */
/*  is subject to restrictions as agreed in the license agreement between    */
/*  you and Woodhead.                                                        */
/*  Copying or distribution is not allowed unless expressly permitted        */
/*  according to your license agreement with Woodhead.                       */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  Project             : STACK PROFINET Controller                          */
/*  Component           : LLDP                                               */
/* $Workfile:: lldpapi.h                                                    $*/
/* $Revision:: 12                                                           $*/
/* $Date:: 16/11/16 9:10                                                    $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*  - LLDP API header                                                        */
/*---------------------------------------------------------------------------*/
#ifndef _LLDPAPI_H
#define _LLDPAPI_H

#include "system.h"
#include "types_pn.h"

#define LLDP_STAT_OK            0
#define LLDP_STAT_PAR           1
#define LLDP_STAT_RES           2
#define LLDP_STAT_OUT_OF_RANGE  3
#define LLDP_STAT_INIT_TWICE    4
#define LLDP_STAT_NOT_SUPPORTED 5
#define LLDP_STAT_NO_RECORD     6

#define LLDP_STAT_ALREADY_STOPPED 7
#define LLDP_STAT_ALREADY_STARTED 8
#define LLDP_STAT_STARTED         9

/* LLDP API DEFINITION */
APP_WORD WSE_LLDP_Init_Req                       (APP_LPVOID lpContext, APP_LPVOID lpvHandle, APP_LPBYTE lpbyMacAddr, APP_BYTE byMacAddrBufferLen, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_Start_Req                      (APP_LPVOID lpContext, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_SetTimeConfig_Req              (APP_LPVOID lpContext, APP_WORD wMsgTxHold, APP_WORD wMsgTxInterval, APP_WORD wMsgTxReInitDelay, APP_WORD wMsgTxDelay, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_GetTimeConfig_Req              (APP_LPVOID lpContext, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_SetPortConfig_Req              (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_BOOL bLLDPPortEnabled, APP_BYTE byLLDPTlvTxEnabled, APP_LPBYTE sPortID, APP_BYTE byPortSubType, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_GetPortConfig_Req              (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_SetSystemConfig_Req            (APP_LPVOID lpContext, APP_LPBYTE sChassisID, APP_BYTE byChassisIDSubType,  APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_GetSystemConfig_Req            (APP_LPVOID lpContext, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_SetAddressMgt_Req              (APP_LPVOID lpContext, APP_BYTE byMgtAddrSubType, APP_DWORD dwMgtAddr, APP_BYTE byMgtAddrItfSubType, APP_DWORD dwMgtAddrItfNumber, APP_BYTE byMgtAddrOidLength, APP_LPBYTE sMgtAddrOid, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_GetAddressMgt_Req              (APP_LPVOID lpContext, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_SetDescription_Req             (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_LPBYTE sPortDesc, APP_LPBYTE sSysName, APP_LPBYTE sSysDesc, APP_WORD wSysCap, APP_WORD wSysCapEnabled, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_GetDescription_Req             (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_PN_SetTxEnable_Req             (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_BYTE byPnTxEnable, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_PN_SetTxDisable_Req            (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_BYTE byPnTxDisable, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_PN_GetTxEnable_Req             (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_PN_SetPortStatus_Req           (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_WORD wPNPortStatusRt2, APP_WORD wPNPortStatusRt3, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_PN_GetPortStatus_Req           (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_PN_SetChassisMac_Req           (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_LPBYTE sChassisMac, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_PN_GetChassisMac_Req           (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_PN_SetMrp_Req                  (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_WORD wMRRT_PortStatus, APP_LPBYTE sUUIDDomain, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_PN_GetMrp_Req                  (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_8023_SetTxEnable_Req           (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_BYTE by802TxEnable, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_8023_GetTxEnable_Req           (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_8023_SetPhyConfig_Req          (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_BOOL b8023AutoNegEnabled, APP_WORD wOperationMauType, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_8023_GetAutoNeg_Req            (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_GetPortSysList_Req             (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_GetPortRemoteInformation_Req   (APP_LPVOID lpContext, APP_BYTE byRemIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_GetPortAddrMgtList_Req         (APP_LPVOID lpContext, APP_BYTE byRemoteIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_GetRemoteStatsTables_Req       (APP_LPVOID lpContext, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_GetRemoteAddressMgt_Req        (APP_LPVOID lpContext, APP_BYTE byRemoteIndex, APP_BYTE byMgtAddrIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_GetRemoteOrganizationUnkTlv_Req(APP_LPVOID lpContext, APP_BYTE byRemoteIndex, APP_BYTE byRemoteOrgUnknownTlvIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_GetRemoteOrgUnkTlvList_Req     (APP_LPVOID lpContext, APP_BYTE byRemoteIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_GetRemoteUnknownTlv_Req        (APP_LPVOID lpContext, APP_BYTE byRemoteIndex, APP_BYTE byRemoteUnknownTlvIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_GetRemoteUnknownTlvList_Req    (APP_LPVOID lpContext, APP_BYTE byRemoteIndex, APP_LPWORD  lpwStatus);
APP_WORD WSE_LLDP_GetRemoteAddressMgt_Req        (APP_LPVOID lpContext, APP_BYTE byRemoteIndex, APP_BYTE byMgtAddrIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_GetPortAddrMgtList_Req         (APP_LPVOID lpContext, APP_BYTE byRemoteIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_PN_GetRemPortStatus_Req        (APP_LPVOID lpContext, APP_BYTE byRemIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_PN_GetRemChassisMac_Req        (APP_LPVOID lpContext, APP_BYTE byRemIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_PN_GetRemMrp_Req               (APP_LPVOID lpContext, APP_BYTE byRemIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_8023_GetRemAutoNeg_Req         (APP_LPVOID lpContext, APP_BYTE byRemIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_Rx_Stats_Req                   (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_Tx_Stats_Req                   (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_GetDeviceNameConfig_Req        (APP_LPVOID lpContext, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_Stop_Req                       (APP_LPVOID lpContext, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_Exit_Req                       (APP_LPVOID lpContext, APP_LPWORD lpwStatus);
#ifdef LLDP_RECEIVER
APP_WORD WSE_LLDP_SignalLocalLinkLost            (APP_BYTE   byPortIndex, APP_LPWORD lpwStatus);
#endif
APP_WORD WSE_LLDP_SetDeviceNameConfig_Req        (APP_LPCHAR lpszDeviceName, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_SetPortAdminStatus_Req         (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_BYTE byLLDPAdminStatus, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_RTC3_SetPTCPStatus_Req         (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_LPCHAR lpSyncMasterMac, APP_LPBYTE sSubDomainUUID, APP_LPBYTE sIRDATAUUID, APP_DWORD dwLenghtOfPeriod, APP_DWORD dwRedPeriodBegin, APP_DWORD dwOrangePeriodBegin, APP_DWORD dwGreenPeriodBegin, APP_LPWORD lpwStatus);
APP_WORD WSE_LLDP_RTC3_SetPortDelay_Req          (APP_LPVOID lpContext, APP_BYTE byPortIndex, APP_DWORD dwPortRxDelayLocal, APP_DWORD dwPortRxDelayRemote, APP_DWORD dwPortTxDelayLocal, APP_DWORD dwPortTxDelayRemote, APP_DWORD dwPortCableDelaylocal, APP_LPWORD lpwStatus);

#endif  /* END OF HEADER FILE */
