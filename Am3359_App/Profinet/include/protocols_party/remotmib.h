/*---------------------------------------------------------------------------*/
/*  Copyright (C) 2009 Woodhead Software & Electonics. All rights reserved.  */
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
/*  Component           :                                                    */
/* $Workfile:: remotmib.h                                                   $*/
/* $Revision:: 10                                                           $*/
/* $Date:: 5/04/16 13:35                                                    $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                    LLDP Remote mib                                        */
/*---------------------------------------------------------------------------*/
#ifndef _REMOTMIB_H
#define _REMOTMIB_H

#ifdef LLDP_RECEIVER

#define MAX_TLV_PN_INFO_LEN   60    /* Longuest PN tlv is PTCP : 54 bytes   */
#define MAX_TLV_8023_INFO_LEN 18    /* Longuest 8023 tlv is MacPhyconf status :  bytes */

/* Remote Mib definition */
#define MAX_TLV_INFO_LEN  512

typedef struct _LLDP_STATS_REMOTE_TABLES{
  APP_DWORD dwLastChangeTime;
  APP_DWORD dwInserts;
  APP_DWORD dwDeletes;
  APP_DWORD dwAgeouts;
  APP_DWORD dwDrops;
}ST_LLDP_REMOTE_STATS_TABLES, APP_FAR *LPST_LLDP_REMOTE_STATS_TABLES;

typedef struct _REMOTE_ORG_8023_TLV{
  APP_BYTE byRemSystemIndex;
  APP_BYTE by8023TlvSubType;
  APP_BYTE s8023TlvInfo[MAX_TLV_8023_INFO_LEN];
  APP_BOOL bUsedNotUsed;
}ST_REMOTE_ORG_8023_TLV, APP_FAR *LPST_REMOTE_ORG_8023_TLV;

typedef struct _REMOTE_ORG_PN_TLV{
  APP_BYTE byRemSystemIndex;
  APP_BYTE byPNTlvSubType;
  APP_BYTE sPNTlvInfo[MAX_TLV_PN_INFO_LEN];
  APP_BOOL bUsedNotUsed;
}ST_REMOTE_ORG_PN_TLV, APP_FAR *LPST_REMOTE_ORG_PN_TLV;

typedef struct _REMOTE_UNKNOWN_TLV{
  APP_BYTE byRemSystemIndex;
  APP_BYTE byUnknownTlvType;
  APP_WORD wUnknownTlvLength;
  APP_BYTE sUnknownTlvInfo[MAX_TLV_INFO_LEN];
  APP_BOOL bUsedNotUsed;
}ST_REMOTE_UNKNOWN_TLV, APP_FAR * LPST_REMOTE_UNKNOWN_TLV;

typedef struct _REMOTE_ORG_UNKNOWN_TLV{
  APP_BYTE byRemSystemIndex;
  APP_BYTE byUnknownTlvType;
  APP_WORD wUnknownTlvLength;
  APP_BYTE sUnknownTlvInfo[MAX_TLV_INFO_LEN];
  APP_BOOL bUsedNotUsed;
  APP_BYTE byOrganization;
}ST_REMOTE_ORG_UNKNOWN_TLV, APP_FAR * LPST_REMOTE_ORG_UNKNOWN_TLV;

typedef struct _REMOTE_SYS_DATA{
  APP_BYTE  byRemIndex;
  APP_BYTE  byRemLocalPortNum;
  APP_BYTE  byRemChassisIdSubType;
  APP_BYTE  sRemChassisID[LLDP_CHASSIS_ID_LEN_MAX];
  APP_BYTE  byChassisIDLen;
  APP_BYTE  byRemPortIdSubType;
  APP_BYTE  sRemPortId[LLDP_PORT_ID_LEN_MAX];
  APP_BYTE  byPortIdLen;
  ST_LLDP_REMOT_SYSTEM_MGT_ADDR StRemMgtAddr[MAX_LOC_REM_MGT_ADDR];
  APP_BYTE  byNbMgtAddr;  /* ?? */
  ST_REMOTE_UNKNOWN_TLV     RemSysUnknownTlv[MAX_UNKNOWN_TLV_PER_LLDPDU];
  ST_REMOTE_ORG_UNKNOWN_TLV RemSysOrgUnknownTlv[MAX_UNKNOWN_TLV_PER_LLDPDU];
#ifdef LLDP_SUPPORT_PN
  ST_REMOTE_ORG_PN_TLV      RemSysPnTlv[MAX_PN_TLV_PER_LLDPDU];
#endif
#ifdef LLDP_SUPPORT_8023
  ST_REMOTE_ORG_8023_TLV    RemSys8023Tlv[MAX_8023_TLV_PER_LLDPDU];
#endif

#ifdef LLDP_STD_DESC_SUPPORT
  APP_BYTE  sPortDesc[LLDP_PORT_DESC_LEN_MAX];
  APP_BYTE  sRemSysName[LLDP_SYS_NAME_LEN_MAX];
  APP_BYTE  sRemSysDesc[LLDP_SYS_DESC_LEN_MAX];
  APP_WORD  wRemSysCap;
  APP_WORD  wRemSysEnCap;
#endif
  APP_BOOL  bUsedNotUsed;
  APP_DWORD dwTimeMarkLastChange; /* Last change of this remote data sys*/
  APP_BYTE  bySrcMc[MAC_ADDRESS_LEN];
}ST_REMOTE_SYS_DATA, APP_FAR * LPST_REMOTE_SYS_DATA;

typedef struct _MSAP {

  APP_BYTE byRemIndex;
  APP_BYTE sMSAPID[LLDP_CHASSIS_ID_LEN_MAX+LLDP_PORT_DESC_LEN_MAX];
  APP_WORD wMsapLen;
  APP_WORD wRxInfoTTL;
  APP_BOOL bMsapUsed;
  APP_BYTE byLocalPort;
  LPST_REMOTE_SYS_DATA lpsrRemoteSysData;
}ST_MSAP, APP_FAR * LPST_MSAP;

#define GET_MSAP_USED(a)                                a->bMsapUsed
#define SET_MSAP_USED(a, b)                             a->bMsapUsed = b
#define GET_MSAP_LOCAL_PORT(a)                          a->byLocalPort
#define GET_MSAP_REM_SYS_DATA(a)                        a->lpsrRemoteSysData
#define GET_MSAP_RXINFO_TTL(a)                          a->wRxInfoTTL
#define SET_MSAP_RXINFO_TTL(a, b)                       a->wRxInfoTTL = b
#define GET_SYS_DATA_USED(a)                            a->lpsrRemoteSysData->bUsedNotUsed
#define SET_SYS_DATA_USED(a,b)                          a->lpsrRemoteSysData->bUsedNotUsed = b
#define GET_SYS_DATA_REMINDEX(a)                        a->byRemIndex
#define SET_SYS_DATA_REMINDEX(a,b)                      a->byRemIndex = b
#define GET_SYS_DATA_REMLOCALPORT(a)                    a->lpsrRemoteSysData->byRemLocalPortNum
#define SET_SYS_DATA_REMLOCALPORT(a,b)                  a->lpsrRemoteSysData->byRemLocalPortNum = b
#define GET_SYS_DATA_REMCHASSISIDSUBTYPE(a)             a->lpsrRemoteSysData->byRemChassisIdSubType
#define SET_SYS_DATA_REMCHASSISIDSUBTYPE(a,b)           a->lpsrRemoteSysData->byRemChassisIdSubType = b
#define GET_SYS_DATA_REMPORTIDSUBTYPE(a)                a->lpsrRemoteSysData->byRemPortIdSubType
#define SET_SYS_DATA_REMPORTIDSUBTYPE(a,b)              a->lpsrRemoteSysData->byRemPortIdSubType = b
#define SET_SYS_DATA_TIME_MARK(a, b)                    a->lpsrRemoteSysData->dwTimeMarkLastChange = b
#define GET_SYS_DATA_TIME_MARK(a)                       a->lpsrRemoteSysData->dwTimeMarkLastChange
#define GET_SYS_DATA_REMCHASSISID(a)                    a->lpsrRemoteSysData->sRemChassisID
#define SET_SYS_DATA_REMCHASSISID(a, b)                 a->lpsrRemoteSysData->sRemChassisID = b
#define GET_SYS_DATA_REMCHASSISIDLEN(a)                 a->lpsrRemoteSysData->byChassisIDLen
#define SET_SYS_DATA_REMCHASSISIDLEN(a, b)              a->lpsrRemoteSysData->byChassisIDLen = b
#define GET_SYS_DATA_REMPORTID(a)                       a->lpsrRemoteSysData->sRemPortId
#define SET_SYS_DATA_REMPORTID(a,b)                     a->lpsrRemoteSysData->sRemPortId = b
#define GET_SYS_DATA_REMPORTIDLEN(a)                    a->lpsrRemoteSysData->byPortIdLen
#define SET_SYS_DATA_REMPORTIDLEN(a,b)                  a->lpsrRemoteSysData->byPortIdLen = b
#define GET_SYS_DATA_REMPORTDESC(a)                     a->lpsrRemoteSysData->sPortDesc
#define GET_SYS_DATA_REMSYSNAME(a)                      a->lpsrRemoteSysData->sRemSysName
#define GET_SYS_DATA_REMSYSDESC(a)                      a->lpsrRemoteSysData->sRemSysDesc
#define GET_SYS_DATA_REMSYSCAP(a)                       a->lpsrRemoteSysData->wRemSysCap
#define SET_SYS_DATA_REMSYSCAP(a, b)                    a->lpsrRemoteSysData->wRemSysCap = b
#define GET_SYS_DATA_REMSYSENCAP(a)                     a->lpsrRemoteSysData->wRemSysEnCap
#define SET_SYS_DATA_REMSYSENCAP(a, b)                  a->lpsrRemoteSysData->wRemSysEnCap = b

/* Remote Management address */
#define GET_SYS_DATA_REMADDRMGT_SUBTYPE(a)              a->lpsrRemoteSysData->StRemMgtAddr[0].byMgtAddrSubType
#define SET_SYS_DATA_REMADDRMGT_SUBTYPE(a, b)           a->lpsrRemoteSysData->StRemMgtAddr[0].byMgtAddrSubType = b
#define GET_SYS_DATA_REMADDRMGT_SUBTYPE_LEN(a)          a->lpsrRemoteSysData->StRemMgtAddr[0].byMgtAddrSubTypeLen
#define SET_SYS_DATA_REMADDRMGT_SUBTYPE_LEN(a, b)       a->lpsrRemoteSysData->StRemMgtAddr[0].byMgtAddrSubTypeLen = b
#define GET_SYS_DATA_REMADDRMGT_ADDR(a)                 (a)->lpsrRemoteSysData->StRemMgtAddr[0].byMgtAddr
#define SET_SYS_DATA_REMADDRMGT_ADDR(a, b)              OS_MEMCOPY( ((a)->lpsrRemoteSysData->StRemMgtAddr[0].byMgtAddr), b, TLV_MANAGEMENT_ADDR_LEN)
#define GET_SYS_DATA_REMADDRMGT_IFSUBTYPE(a)            a->lpsrRemoteSysData->StRemMgtAddr[0].byMgtAddrItfSubType
#define SET_SYS_DATA_REMADDRMGT_IFSUBTYPE(a, b)         a->lpsrRemoteSysData->StRemMgtAddr[0].byMgtAddrItfSubType = b
#define GET_SYS_DATA_REMADDRMGT_IFNUMBER(a)             a->lpsrRemoteSysData->StRemMgtAddr[0].dwMgtAddrItfNumber
#define SET_SYS_DATA_REMADDRMGT_IFNUMBER(a, b)          a->lpsrRemoteSysData->StRemMgtAddr[0].dwMgtAddrItfNumber = b
#define GET_SYS_DATA_REMADDRMGT_OIDLEN(a)               a->lpsrRemoteSysData->StRemMgtAddr[0].byMgtAddrOidLength
#define SET_SYS_DATA_REMADDRMGT_OIDLEN(a, b)            a->lpsrRemoteSysData->StRemMgtAddr[0].byMgtAddrOidLength = b
#define GET_SYS_DATA_REMADDRMGT_OID(a)                  a->lpsrRemoteSysData->StRemMgtAddr[0].sMgtAddrOid
#define GET_SYS_DATA_REMADDR_COUNT(a)                   a->lpsrRemoteSysData->byNbMgtAddr
#define SET_SYS_DATA_REMADDR_COUNT(a, b)                a->lpsrRemoteSysData->byNbMgtAddr = b
#define GET_UNKOWN_TLV_REM_USED(a,b)                    a->lpsrRemoteSysData->RemSysUnknownTlv[b].bUsedNotUsed
#define SET_UNKOWN_TLV_REM_USED(a,b,c)                  a->lpsrRemoteSysData->RemSysUnknownTlv[b].bUsedNotUsed = c
#define GET_UNKOWN_TLV_REM_SYSINDEX(a,b)                a->lpsrRemoteSysData->RemSysUnknownTlv[b].byRemSystemIndex
#define SET_UNKOWN_TLV_REM_SYSINDEX(a,b,c)              a->lpsrRemoteSysData->RemSysUnknownTlv[b].byRemSystemIndex = c
#define GET_UNKOWN_TLV_REM_TLVTYPE(a,b)                 a->lpsrRemoteSysData->RemSysUnknownTlv[b].byUnknownTlvType
#define SET_UNKOWN_TLV_REM_TLVTYPE(a,b,c)               a->lpsrRemoteSysData->RemSysUnknownTlv[b].byUnknownTlvType = c
#define GET_UNKOWN_TLV_REM_TLVLENGTH(a,b)               a->lpsrRemoteSysData->RemSysUnknownTlv[b].wUnknownTlvLength
#define SET_UNKOWN_TLV_REM_TLVLENGTH(a,b,c)             a->lpsrRemoteSysData->RemSysUnknownTlv[b].wUnknownTlvLength = c
#define GET_UNKOWN_TLV_REM_TLVINFO(a,b)                 a->lpsrRemoteSysData->RemSysUnknownTlv[b].sUnknownTlvInfo

/* Unknown Organization TLV */
#define SET_REM_ORG_UNKNOWN_TLV_USED(a, b, c)           a->lpsrRemoteSysData->RemSysOrgUnknownTlv[b].bUsedNotUsed = c
#define GET_REM_ORG_UNKNOWN_TLV_USED(a, b)              a->lpsrRemoteSysData->RemSysOrgUnknownTlv[b].bUsedNotUsed
#define SET_REM_ORG_UNKNOWN_TLV_REM_INDEX(a, b, c)      a->lpsrRemoteSysData->RemSysOrgUnknownTlv[b].byRemSystemIndex = c
#define GET_REM_ORG_UNKNOWN_TLV_REM_INDEX(a, b)         a->lpsrRemoteSysData->RemSysOrgUnknownTlv[b].byRemSystemIndex
#define SET_REM_ORG_UNKNOWN_TLV_ORGANIZATION(a ,b, c)   a->lpsrRemoteSysData->RemSysOrgUnknownTlv[b].byOrganization = c
#define GET_REM_ORG_UNKNOWN_TLV_ORGANIZATION(a ,b)      a->lpsrRemoteSysData->RemSysOrgUnknownTlv[b].byOrganization
#define SET_REM_ORG_UNKNOWN_TLV_SUBTYPE(a, b, c)        a->lpsrRemoteSysData->RemSysOrgUnknownTlv[b].byUnknownTlvType = c
#define GET_REM_ORG_UNKNOWN_TLV_SUBTYPE(a, b)           a->lpsrRemoteSysData->RemSysOrgUnknownTlv[b].byUnknownTlvType
#define SET_REM_ORG_UNKNOWN_TLV_LENGTH(a ,b, c)         a->lpsrRemoteSysData->RemSysOrgUnknownTlv[b].wUnknownTlvLength = c
#define GET_REM_ORG_UNKNOWN_TLV_LENGTH(a ,b)            a->lpsrRemoteSysData->RemSysOrgUnknownTlv[b].wUnknownTlvLength
#define GET_REM_ORG_UNKNOWN_TLV_INFO(a ,b)              a->lpsrRemoteSysData->RemSysOrgUnknownTlv[b].sUnknownTlvInfo
#define GET_REM_ORG_UNKNOWN_TLV(a, b)                   a->lpsrRemoteSysData->RemSysOrgUnknownTlv[b]

/* PN TLV */
#define SET_REM_ORG_PN_TLV_USED(a, b, c)                a->lpsrRemoteSysData->RemSysPnTlv[b].bUsedNotUsed = c
#define GET_REM_ORG_PN_TLV_USED(a, b)                   a->lpsrRemoteSysData->RemSysPnTlv[b].bUsedNotUsed
#define SET_REM_ORG_PN_TLV_REM_INDEX(a, b, c)           a->lpsrRemoteSysData->RemSysPnTlv[b].byRemSystemIndex = c
#define GET_REM_ORG_PN_TLV_REM_INDEX(a, b)              a->lpsrRemoteSysData->RemSysPnTlv[b].byRemSystemIndex
#define SET_REM_ORG_PN_TLV_SUBTYPE(a, b, c)             a->lpsrRemoteSysData->RemSysPnTlv[b].byPNTlvSubType = c
#define GET_REM_ORG_PN_TLV_SUBTYPE(a, b)                a->lpsrRemoteSysData->RemSysPnTlv[b].byPNTlvSubType
#define GET_REM_ORG_PN_TLV_INFO(a ,b)                   a->lpsrRemoteSysData->RemSysPnTlv[b].sPNTlvInfo

/* 8023 TLV */
#define SET_REM_ORG_8023_TLV_USED(a, b, c)              a->lpsrRemoteSysData->RemSys8023Tlv[b].bUsedNotUsed = c
#define GET_REM_ORG_8023_TLV_USED(a, b)                 a->lpsrRemoteSysData->RemSys8023Tlv[b].bUsedNotUsed
#define SET_REM_ORG_8023_TLV_REM_INDEX(a, b, c)         a->lpsrRemoteSysData->RemSys8023Tlv[b].byRemSystemIndex = c
#define GET_REM_ORG_8023_TLV_REM_INDEX(a, b)            a->lpsrRemoteSysData->RemSys8023Tlv[b].byRemSystemIndex
#define SET_REM_ORG_8023_TLV_SUBTYPE(a, b, c)           a->lpsrRemoteSysData->RemSys8023Tlv[b].by8023TlvSubType = c
#define GET_REM_ORG_8023_TLV_SUBTYPE(a, b)              a->lpsrRemoteSysData->RemSys8023Tlv[b].by8023TlvSubType
#define GET_REM_ORG_8023_TLV_INFO(a ,b)                 a->lpsrRemoteSysData->RemSys8023Tlv[b].s8023TlvInfo

#define LLDP_TABLE_FULL 3

APP_VOID Initialize_Remote_Mib        (APP_VOID);
APP_WORD InitializeTLVValueChecker    (APP_VOID);
APP_VOID DecMsapTTL                   (APP_VOID);
APP_VOID ResetMsapTTL                 (APP_BYTE byPort);
APP_VOID SearchRxinfoTTLNULL          (APP_BYTE byLocalPortIndex, LPST_MSAP *lpstMsapid);
APP_WORD IsMsapPresent                (APP_LPBYTE sMSAPID, APP_WORD wMsapLen, APP_BYTE byLocalPortIndex, LPST_MSAP *lpstMsapid);
APP_WORD CreateMsap                   (APP_LPBYTE sMSAPID, APP_WORD wMsapLen, APP_WORD wTTL, APP_BYTE byLocalPortIndex, LPST_MSAP *lpstMsapid, APP_LPBYTE bySrcMac);
APP_WORD CheckReceivedTlvValues       (LPST_LLDP_PORT lpPort, LPST_LLDP_TLV lpstLldpTlv, LPST_MSAP lpstMsapId);
APP_WORD Check_End_Of_LLDPDU_TLV      (LPST_LLDP_PORT lpPort, LPST_LLDP_TLV lpstLldpTlv, LPST_MSAP lpstMsapId);
APP_WORD Check_Chassis_Id_TLV         (LPST_LLDP_PORT lpPort, LPST_LLDP_TLV lpstLldpTlv, LPST_MSAP lpstMsapId);
APP_WORD Check_Port_Id_TLV            (LPST_LLDP_PORT lpPort, LPST_LLDP_TLV lpstLldpTlv, LPST_MSAP lpstMsapId);
APP_WORD Check_Ttl_TLV                (LPST_LLDP_PORT lpPort, LPST_LLDP_TLV lpstLldpTlv, LPST_MSAP lpstMsapId);
APP_WORD Check_Port_Description_TLV   (LPST_LLDP_PORT lpPort, LPST_LLDP_TLV lpstLldpTlv, LPST_MSAP lpstMsapId);
APP_WORD Check_System_Name_TLV        (LPST_LLDP_PORT lpPort, LPST_LLDP_TLV lpstLldpTlv, LPST_MSAP lpstMsapId);
APP_WORD Check_System_Description_TLV (LPST_LLDP_PORT lpPort, LPST_LLDP_TLV lpstLldpTlv, LPST_MSAP lpstMsapId);
APP_WORD Check_System_Capability_TLV  (LPST_LLDP_PORT lpPort, LPST_LLDP_TLV lpstLldpTlv, LPST_MSAP lpstMsapId);
APP_WORD Check_Mgmt_Addr_TLV          (LPST_LLDP_PORT lpPort, LPST_LLDP_TLV lpstLldpTlv, LPST_MSAP lpstMsapId);
APP_WORD Check_Organization_TLV       (LPST_LLDP_PORT lpPort, LPST_LLDP_TLV lpstLldpTlv, LPST_MSAP lpstMsapId);
APP_WORD CheckBasicTLv                (LPST_LLDP_PORT lpPort, LPST_LLDP_TLV lpstLldpTlv, LPST_MSAP lpstMsapId);
APP_WORD CheckPnTLV                   (LPST_LLDP_PORT lpPort, LPST_LLDP_TLV lpstLldpTlv, LPST_MSAP lpstMsapId);
APP_WORD Check8023TLV                 (LPST_LLDP_PORT lpPort, LPST_LLDP_TLV lpstLldpTlv, LPST_MSAP lpstMsapId);
APP_WORD CheckDiffRemSys              (LPST_MSAP lpstMsapId, LPST_LLDP_PORT lpPort);
APP_VOID ClearRemSysData              (LPST_MSAP lpstMsapId);
APP_BYTE GetVisibleChassiID           (LPST_MSAP lpstMsapId, APP_LPBYTE lpbyVisible);
APP_WORD DeleteMsap                   (LPST_MSAP lpstMsapid);
APP_WORD GetUnknowTlvIndex            (LPST_MSAP lpStMsapRemSys, APP_LPBYTE lpbyNbUnkTlv, APP_LPBYTE lpbyArrayUnkTlvIndex);
APP_WORD GetUnknowOrgTlvIndex         (LPST_MSAP lpStMsapRemSys, APP_LPBYTE lpbyNbRemoteOrgUnkTlv, APP_LPBYTE lpbyRemoteOrgUnkTlvIndex);
APP_WORD IsRecognizedTlvType          (LPST_LLDP_TLV lpstLldpTlv);
APP_WORD WriteOrgUnknownTlv           (LPST_LLDP_TLV lpstLldpTlv, LPST_MSAP lpstMsapId, APP_BYTE byOrgTlv);
APP_WORD CreateOrgUnkTlv              (LPST_LLDP_TLV lpstLldpTlv, LPST_MSAP lpstMsapId, APP_BYTE byOrgTlv, APP_LPBYTE lpbyRemOrgUnknownIndex);
APP_WORD GetRemoteStatsTables         (LPST_LLDP_REMOTE_STATS_TABLES lpstRemoteStatsTables);
LPST_MSAP IsMacPresent                (APP_LPBYTE lpbySrcMac);
#endif
#endif  /* END OF HEADER FILE */
