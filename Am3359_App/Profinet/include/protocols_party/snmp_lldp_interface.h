/**
 * \file snmp_lldp_interface.h
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

#ifndef _LLDP_INTERFACE_H
#define _LLDP_INTERFACE_H

#include "ti/osal/SemaphoreP.h"

#include "lldpapi.h"
#include <Profinet/include/protocols_party/RT_MRP/platform_pn.h>

extern SemaphoreP_Handle gMUX_LLDP;
#define NUMBER_OF_LLDP_PORTS 2


typedef struct _ST_LLDP_INTERFACE_GET_TIME_CONFIG
{
    SemaphoreP_Handle MUX_LLDP;
    APP_WORD wResult;
    APP_WORD wMsgHold;
    APP_WORD wMsgTxInterval;
    APP_WORD wMsgTxReInitDelay;
    APP_WORD wMsgTxDelay;
} ST_LLDP_INTERFACE_GET_TIME_CONFIG;

typedef struct _ST_LLDP_INTERFACE_SYSTEM_CONFIG
{
    SemaphoreP_Handle MUX_LLDP;
    APP_WORD wResult;
    APP_BYTE byChassisIDSubType;
    APP_BYTE sChassisID[256];
} ST_LLDP_INTERFACE_SYSTEM_CONFIG;



typedef struct _ST_LLDP_INTERFACE_DESCRIPTION
{
    SemaphoreP_Handle   MUX_LLDP;
    APP_WORD wResult;
    APP_BYTE byPortIndex;
    APP_BYTE sPortDesc[255];
    APP_BYTE sSysDesc[255];
    APP_BYTE sSysName[255];
    APP_WORD wSysCap;
    APP_WORD wSysCapEnabled;
} ST_LLDP_INTERFACE_DESCRIPTION;

typedef struct _ST_LLDP_INTERFACE_PORT_CONFIG
{
    SemaphoreP_Handle   MUX_LLDP;
    APP_WORD wResult;
    APP_BYTE byPortIndex;
    APP_BYTE byLLDPAdminStatus;
    APP_BOOL bLLDPortEnabled;
    APP_BYTE byLLDPTlvEnabled;
    APP_BYTE sPortID[255];
    APP_BYTE byPortSubType;
} ST_LLDP_INTERFACE_PORT_CONFIG;


typedef struct _ST_LLDP_INTERFACE_RX_STATS
{
    SemaphoreP_Handle    MUX_LLDP;
    APP_WORD  wResult;
    APP_BYTE  byPortIndex;
    APP_DWORD dwStatsAgeOutsTotal;          /* 802.1AB 10.5.2.2*/
    APP_DWORD dwStatsFramesDiscardedTotal;  /* 802.1AB 10.5.2.2*/
    APP_DWORD dwStatsFramesInErrorsTotal;   /* 802.1AB 10.5.2.2*/
    APP_DWORD dwStatsFramesInTotal;         /* 802.1AB 10.5.2.2*/
    APP_DWORD dwStatsTLVsDiscardedTotal;    /* 802.1AB 10.5.2.2*/
    APP_DWORD dwStatsTLVsUnrecognizedTotal; /* 802.1AB 10.5.2.2*/
} ST_LLDP_INTERFACE_RX_STATS;

typedef struct _ST_LLDP_STATS_REMOTE_TABLES
{
    SemaphoreP_Handle    MUX_LLDP;
    APP_WORD  wResult;
    APP_DWORD dwLastChangeTime;
    APP_DWORD dwInserts;
    APP_DWORD dwDeletes;
    APP_DWORD dwAgeouts;
    APP_DWORD dwDrops;
} ST_LLDP_REMOTE_STATS_TABLES;

typedef struct _ST_LLDP_PORT_INFORMATION
{
    SemaphoreP_Handle    MUX_LLDP;
    APP_WORD  wResult;
    APP_BYTE byPortIndex;
    APP_BYTE byRemSysOnPortIndex;
    APP_BYTE byRemSysOnPort[WSE_LLDP_REMOTE_SYSTEM_PORT];
    APP_BOOL bTooManyNeighors;
    APP_BOOL bSomethingChangeRemote;
} ST_LLDP_PORT_INFORMATION;




typedef struct _ST_LLDP_INTERFACE_TX_STATS
{
    SemaphoreP_Handle    MUX_LLDP;
    APP_WORD  wResult;
    APP_BYTE  byPortIndex;
    APP_DWORD dwFrameOutTotal; /* 802.1AB 10.5.2.2*/
} ST_LLDP_INTERFACE_TX_STATS;

typedef struct _ST_LLDP_INTERFACE_ADR_MANAGEMENT
{
    SemaphoreP_Handle    MUX_LLDP;
    APP_WORD  wResult;
    APP_BYTE byMgtAddrSubType;
    APP_DWORD dwMgtAddr;
    APP_BYTE byMgtAddrItfSubType;
    APP_DWORD dwMgtAddrItfNumber;
    APP_BYTE byMgtAddrOidLength;
    APP_BYTE sOID[255];
} ST_LLDP_INTERFACE_ADR_MANAGEMENT;

typedef struct _ST_LLDP_INTERFACE_REMOTE_INFORMATION
{
    SemaphoreP_Handle     MUX_LLDP;
    APP_WORD   wResult;
    APP_BYTE   byRemIndex;
    APP_BYTE   byRemChassisIdSubType;
    APP_BYTE   byRemChassisId[255];
    APP_BYTE   byChassisIdLen;
    APP_BYTE   byRemPortIdSubType;
    APP_BYTE   byRemPortId[255];
    APP_BYTE   byPortIdLen;
    APP_BYTE   sPortDescription[255];
    APP_BYTE   sSysName[255];
    APP_BYTE   sSysDescription[255];
    APP_WORD   wRemCapability;
    APP_WORD   wRemCapabilityEnabled;
    APP_DWORD  dwTimeMark;
} ST_LLDP_INTERFACE_REMOTE_INFORMATION;


typedef struct _ST_LLDP_INTERFACE_REMOTE_ADDR_MGT_LIST
{
    SemaphoreP_Handle     MUX_LLDP;
    APP_WORD   wResult;
    APP_BYTE byRemIndex;
    APP_BYTE byNumberOfAddrMgtIndex;
    APP_BYTE byMgtAddrIndex[255];
} ST_LLDP_INTERFACE_REMOTE_ADDR_MGT_LIST;

typedef struct _ST_LLDP_8023_AUTONEG
{
    SemaphoreP_Handle     MUX_LLDP;
    APP_WORD   wResult;
    APP_BYTE byIndex;
    APP_BOOL bAutoNegSupported;
    APP_BOOL bAutoNegEnabled;
    APP_WORD wAutoNegAdvCapability;
    APP_WORD wOperationMauType;
} ST_LLDP_8023_AUTONEG;

typedef struct _ST_LLDP_INTERFACE_REMOTE_ADDR_MGT
{
    SemaphoreP_Handle    MUX_LLDP;
    APP_WORD  wResult;
    APP_BYTE  byRemIndex;
    APP_BYTE  byAddrMgtSubType;
    APP_BYTE  byAddrMgtLen;
    APP_BYTE  byAddrMgtAddr[255];
    APP_BYTE  byAddrMgtIfSubType;
    APP_DWORD dwAddrMgtIfNumber;
    APP_BYTE  byAddrMgtOidLen;
    APP_BYTE  sAddrMgtOid[255];
} ST_LLDP_INTERFACE_REMOTE_ADDR_MGT;


typedef struct _ST_LLDP_INTERFACE_REMOTE_UNK_TLV_LIST
{
    SemaphoreP_Handle     MUX_LLDP;
    APP_WORD   wResult;
    APP_BYTE byRemIndex;
    APP_BYTE byNumberUnknownTlvIndex;
    APP_BYTE byUnknownTlvIndex[MAX_UNKNOWN_TLV_PER_LLDPDU];
} ST_LLDP_INTERFACE_REMOTE_UNK_TLV_LIST;

typedef struct _ST_LLDP_INTERFACE_REMOTE_UNK_TLV
{
    SemaphoreP_Handle     MUX_LLDP;
    APP_WORD   wResult;
    APP_BYTE byRemIndex;
    APP_BYTE byUnknownTlvIndex;
    APP_BYTE byUnknownTlvType;
    APP_WORD wUnknownTlvLenght;
    APP_BYTE sUnknownTlvInfo[512];/* 512*/
} ST_LLDP_INTERFACE_REMOTE_UNK_TLV;

typedef struct _ST_LLDP_INTERFACE_ORG_UNK_TLV_LIST
{
    SemaphoreP_Handle     MUX_LLDP;
    APP_WORD   wResult;
    APP_BYTE byRemIndex;
    APP_BYTE byNumberOrgTlvIndex;
    APP_BYTE byRemoteOrgUnkTlvIndex[MAX_UNKNOWN_TLV_PER_LLDPDU];
} ST_LLDP_INTERFACE_ORG_UNK_TLV_LIST;

typedef struct _ST_LLDP_INTERFACE_ORG_UNK_TLV
{
    SemaphoreP_Handle     MUX_LLDP;
    APP_WORD   wResult;
    APP_BYTE byRemIndex;
    APP_BYTE byNbRemOrgUnkTlvIndex;
    APP_BYTE byRemOrgUnkSubType;
    APP_BYTE sRemOrgUnkOID[255];
    APP_BYTE  byRemOrgUnkOIDLen;
    APP_WORD wRemOrgUnkInfoLength;
    APP_BYTE sRemOrgUnkInfo[512];
} ST_LLDP_INTERFACE_ORG_UNK_TLV;

typedef struct _ST_LLDP_INTERFACE_DEVICENAME_CONFIG
{
    SemaphoreP_Handle MUX_LLDP;
    APP_WORD wResult;
    APP_BYTE sDeviceName[256];
} ST_LLDP_INTERFACE_DEVICENAME_CONFIG ;

typedef struct _ST_LLDP_INTERFACE_AUTONEG {
  SemaphoreP_Handle MUX_LLDP;
  APP_WORD wResult;
  APP_WORD w8023AutoNegAdvCapability;
  APP_WORD wOperationMauType;
  APP_BYTE byPortIndex;
  APP_BOOL b8023AutoNegSupported;
  APP_BOOL b8023AutoNegEnabled;
} ST_LLDP_INTERFACE_AUTONEG, APP_FAR * LPST_LLDP_INTERFACE_AUTONEG;

/*Interface between SNMP and LLDP layer.
 * Use a mutex to synchronize the Request Answer*/

APP_WORD LLDP_GetAddressMgt(ST_LLDP_INTERFACE_ADR_MANAGEMENT *lpInterface);
APP_WORD LLDP_GetPortConfig(ST_LLDP_INTERFACE_PORT_CONFIG *lpPortConfig,
                            APP_BYTE byport);
APP_WORD LLDP_GetSystemConfig(ST_LLDP_INTERFACE_SYSTEM_CONFIG *lpSystem);
APP_WORD LLDP_GetDescription(ST_LLDP_INTERFACE_DESCRIPTION *lpDescription,
                             APP_BYTE byPort);

APP_WORD LLDP_GetPortRemoteInformation(ST_LLDP_INTERFACE_REMOTE_INFORMATION
                                       *lpRemInfo, APP_BYTE RemoteIndex);
APP_WORD LLDP_TimeMark(APP_LPDWORD lpdwTimeMark, APP_BYTE RemoteIndex);
APP_WORD LLDP_GetTimeConfig(ST_LLDP_INTERFACE_GET_TIME_CONFIG *lpGetTime);
APP_WORD LLDP_SetTimeConfig(ST_LLDP_INTERFACE_GET_TIME_CONFIG *lpGetTime);
APP_WORD LLDP_Tx_Stats ( ST_LLDP_INTERFACE_TX_STATS *lpTxStats,
                         APP_BYTE byport );
APP_WORD LLDP_Rx_Stats(ST_LLDP_INTERFACE_RX_STATS *lpRxStats, APP_BYTE port);
APP_WORD LLDP_GetStatsRemoteTables(ST_LLDP_REMOTE_STATS_TABLES
                                   *lpRemStatsTables);
APP_WORD LLDP_GetDeviceNameConfig(ST_LLDP_INTERFACE_DEVICENAME_CONFIG
                                  *lpDevName);

APP_WORD LLDP_Init(APP_VOID);

APP_WORD LLDP_GetPortSysList ( ST_LLDP_PORT_INFORMATION *lpinfo,
                               APP_BYTE port );
APP_WORD LLDP_GetPortRemoteInformation(ST_LLDP_INTERFACE_REMOTE_INFORMATION
                                       *lpRemInfo, APP_BYTE byRemIndex);

APP_WORD LLDP_GetRemoteAddressMgt ( ST_LLDP_INTERFACE_REMOTE_ADDR_MGT
                                    *lpAddress, APP_BYTE byRemIndex, APP_BYTE byMgtAddrIndex );
APP_WORD LLDP_GetRemoteAddrMgtList(ST_LLDP_INTERFACE_REMOTE_ADDR_MGT_LIST
                                   *lpList, APP_BYTE RemIndex);
APP_WORD LLDP_GetPortForIndex(APP_LPBYTE lpPort, APP_BYTE byRemIndex);

APP_WORD LLDP_8023_GetLocAutoNeg(ST_LLDP_8023_AUTONEG *lpAddress,
                                 APP_BYTE byPortIndex);
APP_WORD LLDP_8023_GetRemAutoNeg(ST_LLDP_8023_AUTONEG *lpAddress,
                                 APP_BYTE byRemIndex);

uint16_t numberOfLldpInterfaces();


APP_WORD LLDP_GetPortRemotePeer(ST_LLDP_PORT_INFORMATION *lpPeerInfo,
                                APP_BYTE byPortIndex);

APP_WORD LLDP_GetRemAddressMgtList(ST_LLDP_INTERFACE_REMOTE_ADDR_MGT_LIST
                                   *lpRemAddrList, APP_BYTE byRemoteSysIndex);
APP_WORD LLDP_GetRemAddressMgt(ST_LLDP_INTERFACE_REMOTE_ADDR_MGT *lpRemAddr,
                               APP_BYTE byRemoteSysIndex, APP_BYTE byRemoteMgtAddrIndex);

APP_WORD LLDP_GetRemoteUnknownTlvList(ST_LLDP_INTERFACE_REMOTE_UNK_TLV_LIST
                                      *lpUnkTlv, APP_BYTE byRemoteSysIndex);
APP_WORD LLDP_GetRemoteUnknownTlv(ST_LLDP_INTERFACE_REMOTE_UNK_TLV *lpUnkTlv,
                                  APP_BYTE byRemoteSysIndex, APP_BYTE byRemoteUnkTlvIndex);

APP_WORD LLDP_GetRemoteOrgTlvList(ST_LLDP_INTERFACE_ORG_UNK_TLV_LIST
                                  *lpOrgUnkTlv, APP_BYTE byRemoteSysIndex);
APP_WORD LLDP_GetOrgUnknownTlv(ST_LLDP_INTERFACE_ORG_UNK_TLV *lpOrgTlv,
                               APP_BYTE byRemoteSysIndex, APP_BYTE byRemoteOrgTlvIndex);

#endif
