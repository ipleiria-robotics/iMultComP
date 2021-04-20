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
/* $Workfile:: lmib.h                                                       $*/
/* $Revision:: 17                                                           $*/
/* $Date:: 16/01/17 17:51                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*  - Local mib header                                                       */
/*---------------------------------------------------------------------------*/
#ifndef _LMIB_H
#define _LMIB_H

#include "lldpt.h"
/* Local Mib definition */
typedef struct _LLDP_CONFIG {
  APP_WORD wmsgTxHold;          /* 802.1AB 10.5.3.3 */
  APP_WORD wmsgTxInterval;      /* 802.1AB 10.5.3.3 */
  APP_WORD wreinitDelay;        /* 802.1AB 10.5.3.3 */
  APP_WORD wtxDelay;            /* 802.1AB 10.5.3.3 */
}ST_LLDP_CONFIG, APP_FAR * LPST_LLDP_CONFIG;

/* Constant definition */
#define LLDP_TXDELAY            1
#define LLDP_REINITDELAY        2
#define LLDP_MSGTXHOLD          4
#define LLDP_MSGTXINTERVAL      5

#define LLDP_MSGTXHOLD_MIN      2
#define LLDP_MSGTXHOLD_MAX      10
#define LLDP_MSGTXINTERVAL_MIN  5
#define LLDP_MSGTXINTERVAL_MAX  32768
#define LLDP_REINITDELAY_MIN    1
#define LLDP_REINITDELAY_MAX    10
#define LLDP_TXDELAY_MIN        1
#define LLDP_TXDELAY_MAX        8192

/* Admin status definition */
#define LLDP_ADMSTATUS_ENABLEDTXONLY  1
#define LLDP_ADMSTATUS_ENABLEDRXONLY  2
#define LLDP_ADMSTATUS_ENABLEDRXTX    3
#define LLDP_ADMSTATUS_DISABLED       4

/* Additionnal LLDP TLVs*/
#define LLDP_TXENABLED_PORTDESC_MASK     1
#define LLDP_TXENABLED_SYSNAME_MASK      2
#define LLDP_TXENABLED_SYSDESC_MASK      4
#define LLDP_TXENABLED_SYSCAP_MASK       8
#define LLDP_TXENABLED_ADDRMGT_MASK      16
#define LLDP_TXENABLED_DEFAULT           16

#ifdef LLDP_STD_DESC_SUPPORT
#define LLDP_TXENABLED_MASK_ALL         (LLDP_TXENABLED_PORTDESC_MASK | LLDP_TXENABLED_SYSNAME_MASK | LLDP_TXENABLED_SYSDESC_MASK | LLDP_TXENABLED_SYSCAP_MASK | LLDP_TXENABLED_ADDRMGT_MASK)
#endif
#ifndef LLDP_STD_DESC_SUPPORT
#define LLDP_TXENABLED_MASK_ALL         (LLDP_TXENABLED_ADDRMGT_MASK)
#endif

#define LLDP_PORT_ID_LEN_MIN         1
#define LLDP_PORT_ID_LEN_MAX         255
#define LLDP_PORT_DESC_LEN_MIN       1
#define LLDP_PORT_DESC_LEN_MAX       255

typedef struct _LLDP_PORT_CONFIG {
  APP_BYTE byPortNumber;
  APP_BYTE byLLDP_TxEnabled;
  APP_BYTE byPortMAC[MAC_ADDRESS_LEN];
  APP_BYTE byPortId[LLDP_PORT_ID_LEN_MAX+1];
  APP_BYTE byPortSubType;

#ifdef LLDP_STD_DESC_SUPPORT
  APP_BYTE sPortDesc[LLDP_PORT_DESC_LEN_MAX+1];
#endif

  APP_LPVOID lpvWriteHandle;
} ST_LLDP_PORT_CONFIG, APP_FAR * LPST_LLDP_PORT_CONFIG;

/* Port sub type */
#define LLDP_PORT_SUBTYPE_IFALIAS        1
#define LLDP_PORT_SUBTYPE_PORTCOMPONENT  2
#define LLDP_PORT_SUBTYPE_MACADDR        3
#define LLDP_PORT_SUBTYPE_NETADDR        4
#define LLDP_PORT_SUBTYPE_IFNAME         5
#define LLDP_PORT_SUBTYPE_AGCIRCID       6
#define LLDP_PORT_SUBTYPE_LOCAL          7

/* LLDP port structure */
typedef struct _LLDP_PORT {
  APP_BYTE                 byLLDP_AdminStatus;        /* 802.1AB 10.5.1 */
  APP_BOOL                 bLLDP_PortEnaled;          /* 802.1AB 10.5.1 */
  APP_BOOL                 bTooManyNeighbors;
  APP_BOOL                 bSomethingChangeRemote;
  APP_BOOL                 bNewTimeParameters;

  ST_LLDP_TX_SM            stLldpTx;

#ifdef LLDP_RECEIVER
  ST_LLDP_RX_SM            stLldpRx;
#endif

  /* Mib access */
  LPST_LLDP_CONFIG         lpstLLDPConfig;
  LPST_LLDP_PORT_CONFIG    lpstLLDPPortConfig;

#ifdef LLDP_SUPPORT_PN
  LPST_LLDP_LOCAL_PN_MIB   lpstLLDPPNlocal;
#endif

#ifdef LLDP_SUPPORT_8023
  LPST_LLDP_LOCAL_8023_MIB lpstLLDP8023;
#endif

#ifdef PN_RTC3_SUPPORTED
  LPST_LLDP_PTCP_STATUS lpstLLDPPTCPSTATUSLocal;
#endif
  LPST_LLDP_PORT_DELAY  lpstLLDPPORTDELAYLocal;

}ST_LLDP_PORT, APP_FAR * LPST_LLDP_PORT;

/* ADDRESS MANAGMENT **/
#define LLDP_OID_LENGTH       128
#define MAX_LOC_REM_MGT_ADDR  1     /* Only 1 is supported */

/* Supported management subtype */
#define LLDP_MGT_ADDR_SUB_TYPE_IPV4     1

#define LLDP_MGT_ADDR_OID_STRING "1.3.6.1.4.1.24686"

/*management address*/
#define TLV_MANAGEMENT_ADDR_LEN 31

/* Interface Subtype */
#define LLDP_MGT_ADDR_IF_SUBTYPE_UNKNOWN  1
#define LLDP_MGT_ADDR_IF_SUBTYPE_IFINDEX  2
#define LLDP_MGT_ADDR_IF_SUBTYPE_SYSPORT  3

typedef struct _LLDP_SYSTEM_MGT_ADDR {
  APP_BYTE  byMgtAddrSubTypeLen;
  APP_BYTE  byMgtAddrSubType;
  APP_BYTE  byMgtAddr[TLV_MANAGEMENT_ADDR_LEN];
  APP_BYTE  byMgtAddrItfSubType;
  APP_DWORD dwMgtAddrItfNumber;
  APP_BYTE  byMgtAddrOidLength;
  APP_BYTE  sMgtAddrOid[LLDP_OID_LENGTH];
}ST_LLDP_LOCAL_SYSTEM_MGT_ADDR, APP_FAR * LPST_LLDP_LOCAL_SYSTEM_MGT_ADDR,ST_LLDP_REMOT_SYSTEM_MGT_ADDR, APP_FAR * LPST_LLDP_REMOT_SYSTEM_MGT_ADDR;

#define LLDP_CHASSIS_ID_LEN_MIN     1
#define LLDP_CHASSIS_ID_LEN_MAX     255

#define LLDP_SYS_NAME_LEN_MIN       1
#define LLDP_SYS_NAME_LEN_MAX       255

#define LLDP_SYS_DESC_LEN_MIN       1
#define LLDP_SYS_DESC_LEN_MAX       255

typedef struct _LLDP_LOCAL_SYSTEM_CONFIG {
  APP_CHAR sChassisID[LLDP_CHASSIS_ID_LEN_MAX+1];
  APP_BYTE byChassisIDLen;
  APP_BYTE byChassisIdSubType;
  APP_BYTE byLocalSystemMAC[MAC_ADDRESS_LEN];
  ST_LLDP_LOCAL_SYSTEM_MGT_ADDR StLocMgtAddr[MAX_LOC_REM_MGT_ADDR];

#ifdef LLDP_STD_DESC_SUPPORT
  APP_BYTE sSysName[LLDP_SYS_NAME_LEN_MAX+1];
  APP_BYTE sSysDesc[LLDP_SYS_DESC_LEN_MAX+1];
  APP_WORD wSysCap;
  APP_WORD wSysCapEnabled;
#endif

}ST_LLDP_LOCAL_SYSTEM_CONFIG, APP_FAR * LPST_LLDP_LOCAL_SYSTEM_CONFIG;

/* ChassisidSubType*/
#define LLDP_CHASSISID_COMPONENT      1   /* */
#define LLDP_CHASSISID_IFALIAS        2   /* */
#define LLDP_CHASSISID_PORTCOMPONENT  3   /* */
#define LLDP_CHASSISID_MAC            4   /* */
#define LLDP_CHASSISID_NETWORKADDR    5   /* */
#define LLDP_CHASSISID_IFNAME         6   /* */
#define LLDP_CHASSISID_LOCALLY        7   /* */
#define LLDP_CHASSISID_DEFAULT        LLDP_CHASSISID_MAC

/* Macros */
/***************** RX ***************/
#define INC_RX_STAT_AGEOUTTOTAL(a)                  (a)->stLldpRx.stLldpRxStats.dwStatsAgeOutsTotal++
#define GET_RX_STAT_AGEOUTTOTAL(a)                  (a)->stLldpRx.stLldpRxStats.dwStatsAgeOutsTotal
#define INC_RX_STAT_FRAMEDISCARDEDTOTAL(a)          (a)->stLldpRx.stLldpRxStats.dwStatsFramesDiscardedTotal++
#define GET_RX_STAT_FRAMEDISCARDEDTOTAL(a)          (a)->stLldpRx.stLldpRxStats.dwStatsFramesDiscardedTotal
#define INC_RX_STAT_FRAMEINERRORTOTAL(a)            (a)->stLldpRx.stLldpRxStats.dwStatsFramesInErrorsTotal++
#define GET_RX_STAT_FRAMEINERRORTOTAL(a)            (a)->stLldpRx.stLldpRxStats.dwStatsFramesInErrorsTotal
#define INC_RX_STAT_FRAMEINTOTAL(a)                 (a)->stLldpRx.stLldpRxStats.dwStatsFramesInTotal++
#define GET_RX_STAT_FRAMEINTOTAL(a)                 (a)->stLldpRx.stLldpRxStats.dwStatsFramesInTotal
#define INC_RX_STAT_TLVDISCARDEDTOTAL(a)            (a)->stLldpRx.stLldpRxStats.dwStatsTLVsDiscardedTotal++
#define GET_RX_STAT_TLVDISCARDEDTOTAL(a)            (a)->stLldpRx.stLldpRxStats.dwStatsTLVsDiscardedTotal
#define INC_RX_STAT_TLVUNRECOGNIZEDTOTAL(a)         (a)->stLldpRx.stLldpRxStats.dwStatsTLVsUnrecognizedTotal++
#define GET_RX_STAT_TLVUNRECOGNIZEDTOTAL(a)         (a)->stLldpRx.stLldpRxStats.dwStatsTLVsUnrecognizedTotal

#define GET_RX_STATE(a)               (a)->stLldpRx.byRxState
#define SET_RX_STATE(a, b)            (a)->stLldpRx.byRxState = b ;LLDP_RX_TRACE("Rx Go to %d\n",b)
#define GET_RX_INFOAGE(a)             (a)->stLldpRx.bRxInfoAge
#define SET_RX_INFOAGE(a, b)          (a)->stLldpRx.bRxInfoAge = b
#define GET_RX_RCVFRAME(a)            (a)->stLldpRx.bRcvFrame
#define SET_RX_RCVFRAME(a, b)         (a)->stLldpRx.bRcvFrame = b
#define GET_RX_TTL(a)                 (a)->stLldpRx.wRxTTL
#define SET_RX_TTL(a, b)              (a)->stLldpRx.wRxTTL = b
#define GET_RX_RXCHANGE(a)            (a)->stLldpRx.bRxChanges
#define SET_RX_RXCHANGE(a, b)         (a)->stLldpRx.bRxChanges = b
#define GET_RX_BAD_FRAME(a)            (a)->stLldpRx.bBadFrame
#define SET_RX_BAD_FRAME(a, b)         (a)->stLldpRx.bBadFrame = b
#define GET_RX_BUFFER(a)              (a)->stLldpRx.lpbyRxFrameBuffer
#define GET_RX_BUFFER_LEN(a)          (a)->stLldpRx.wRxFrameLen
#define SET_RX_BUFFER_LEN(a, b)       (a)->stLldpRx.wRxFrameLen = b
#define GET_RX_TLV_REF(a,b)           &((a)->stLldpRx.stRxTLV[b])

/***************** TX ***************/
#define INC_TX_STAT_FRAMEOUTTOTAL(a) (a)->stLldpTx.stLldpTxStats.dwStatsFrameOutTotal++
#define GET_TX_STAT_FRAMEOUTTOTAL(a) (a)->stLldpTx.stLldpTxStats.dwStatsFrameOutTotal

#define GET_TX_STATE(a)                      (a)->stLldpTx.byTxState
#define SET_TX_STATE(a, b)                   (a)->stLldpTx.byTxState = b;LLDP_TX_TRACE("Tx Go to %d\n", b)
#define GET_TX_TTL(a)                        (a)->stLldpTx.wTxTTL
#define SET_TX_TTL(a, b)                     (a)->stLldpTx.wTxTTL = (APP_WORD)(b)
#define GET_ADMIN_STATUS(a)                  (a)->byLLDP_AdminStatus
#define SET_ADMIN_STATUS(a, b)               (a)->byLLDP_AdminStatus = b
#define GET_PORT_STATE(a)                    (a)->bLLDP_PortEnaled
#define SET_PORT_STATE(a, b)                 (a)->bLLDP_PortEnaled = b
#define SET_PORT_TOOMANYNEIGHBORS(a, b)      (a)->bTooManyNeighbors = b
#define GET_PORT_TOOMANYNEIGHBORS(a)         (a)->bTooManyNeighbors
#define SET_PORT_SOMETHINGCHANGEREMOTE(a, b) (a)->bSomethingChangeRemote = b
#define GET_PORT_SOMETHINGCHANGEREMOTE(a)    (a)->bSomethingChangeRemote
#define SET_PORT_NEWTIMEPARAM(a,b)           (a)->bNewTimeParameters = b
#define GET_PORT_NETTIMEPARAM(a)             (a)->bNewTimeParameters
#define GET_TX_SOMETHINGCHANGE(a)            (a)->stLldpTx.bySomethingChangeLocal
#define SET_TX_SOMETHINGCHANGE(a, b)         (a)->stLldpTx.bySomethingChangeLocal = b
#define CLEAR_FRAMEOUTTOTAL(a)               (a)->stLldpTx.stLldpTxStats.dwStatsFrameOutTotal = 0
#define INC_FRAMEOUTTOTAL(a)                 (a)->stLldpTx.stLldpTxStats.dwStatsFrameOutTotal++
#define GET_TX_DELAYWHILE(a)                 (a)->stLldpTx.stLLDPTXTimers.wTxDelayWhile
#define SET_TX_DELAYWHILE(a, b)              (a)->stLldpTx.stLLDPTXTimers.wTxDelayWhile = b
#define GET_TX_TTR(a)                        (a)->stLldpTx.stLLDPTXTimers.wTxTTR
#define SET_TX_TTR(a, b)                     (a)->stLldpTx.stLLDPTXTimers.wTxTTR = b
#define GET_TX_SHUTDOWNWHILE(a)              (a)->stLldpTx.stLLDPTXTimers.wTxShutdownWhile
#define SET_TX_SHUTDOWNWHILE(a, b)           (a)->stLldpTx.stLLDPTXTimers.wTxShutdownWhile = b
#define GET_TX_HOLD(a)                       (a)->lpstLLDPConfig->wmsgTxHold
#define GET_TX_INTERVAL(a)                   (a)->lpstLLDPConfig->wmsgTxInterval
#define GET_TX_REINITDELAY(a)                (a)->lpstLLDPConfig->wreinitDelay
#define GET_TX_DELAY(a)                      (a)->lpstLLDPConfig->wtxDelay
#define GET_PORT_MAC(a)                      (a)->lpstLLDPPortConfig->byPortMAC
#define GET_PORTID(a)                        (a)->lpstLLDPPortConfig->byPortId
#define GET_PORTID_SUBTYPE(a)                (a)->lpstLLDPPortConfig->byPortSubType
#define GET_PORT_NUMBER(a)                   (a)->lpstLLDPPortConfig->byPortNumber
#define GET_WRITE_HANDLE(a)                  (a)->lpstLLDPPortConfig->lpvWriteHandle
#define SET_WRITE_HANDLE(a, b)               (a)->lpstLLDPPortConfig->lpvWriteHandle = b

#ifdef LLDP_STD_DESC_SUPPORT
#define GET_PORT_DESC(a)         (a)->lpstLLDPPortConfig->sPortDesc
#endif

#define GET_PORT_TX_ENABLED(a)   (a)->lpstLLDPPortConfig->byLLDP_TxEnabled


#ifdef LLDP_SUPPORT_PN

#define GET_PN_TX_ENABLE(a)             (a)->lpstLLDPPNlocal->byPNTxEnabled
#define SET_PN_TX_ENABLE(a, b)          (a)->lpstLLDPPNlocal->byPNTxEnabled = b
#define GET_MRP_DOMAINUUID(a)           (a)->lpstLLDPPNlocal->UUID_MRPDomain
#define GET_MRP_PORTSTATUS(a)           (a)->lpstLLDPPNlocal->wMRRT_PortStatus
#define SET_MRP_PORTSTATUS(a, b)        (a)->lpstLLDPPNlocal->wMRRT_PortStatus = b
#define GET_PN_CHASSISMAC(a)            (a)->lpstLLDPPNlocal->sPNIOChassisMAC
#define GET_PN_PORTRT2(a)               (a)->lpstLLDPPNlocal->wPNPortStatusRt2
#define SET_PN_PORTRT2(a,b)             (a)->lpstLLDPPNlocal->wPNPortStatusRt2 = b
#define GET_PN_PORTRT3(a)               (a)->lpstLLDPPNlocal->wPNPortStatusRt3
#define SET_PN_PORTRT3(a,b)             (a)->lpstLLDPPNlocal->wPNPortStatusRt3 = b

#endif

#ifdef LLDP_SUPPORT_8023

#define GET_8023_AUTONEGSUPPORTED(a)      (a)->lpstLLDP8023->b8023AutoNegSupported
#define SET_8023_AUTONEGSUPPORTED(a,b)    (a)->lpstLLDP8023->b8023AutoNegSupported = b
#define GET_8023_AUTONEGENABLED(a)        (a)->lpstLLDP8023->b8023AutoNegEnabled
#define SET_8023_AUTONEGENABLED(a,b)      (a)->lpstLLDP8023->b8023AutoNegEnabled = b
#define GET_8023_AUTONEGADVERTIZED(a)     (a)->lpstLLDP8023->w8023AutoNegAdvertisedCap
#define SET_8023_AUTONEGADVERTIZED(a, b)  (a)->lpstLLDP8023->w8023AutoNegAdvertisedCap = b
#define GET_8023_OPERATIONMAU(a)          (a)->lpstLLDP8023->w8023OperationMauType
#define SET_8023_OPERATIONMAU(a,b)        (a)->lpstLLDP8023->w8023OperationMauType = b
#define GET_8023_TX_ENABLED(a)            (a)->lpstLLDP8023->by8023TxEnabled
#define SET_8023_TX_ENABLED(a, b)         (a)->lpstLLDP8023->by8023TxEnabled = b

#endif

#define  GET_PTCPSTATUS_LENGHTPERIOD(a)           (a)->lpstLLDPPTCPSTATUSLocal->dwLenghtOfPeriod
#define  SET_PTCPSTATUS_LENGHTPERIOD(a,b)         (a)->lpstLLDPPTCPSTATUSLocal->dwLenghtOfPeriod = b
#define  GET_PTCPSTATUS_REDPERIODBEGIN(a)         (a)->lpstLLDPPTCPSTATUSLocal->dwRedPeriodBegin
#define  SET_PTCPSTATUS_REDPERIODBEGIN(a, b)      (a)->lpstLLDPPTCPSTATUSLocal->dwRedPeriodBegin = b
#define  GET_PTCPSTATUS_ORANGEPERIODBEGIN(a)      (a)->lpstLLDPPTCPSTATUSLocal->dwOrangePeriodBegin
#define  SET_PTCPSTATUS_ORANGEPERIODBEGIN(a, b)   (a)->lpstLLDPPTCPSTATUSLocal->dwOrangePeriodBegin = b
#define  GET_PTCPSTATUS_GREENPERIODBEGIN(a)       (a)->lpstLLDPPTCPSTATUSLocal->dwGreenPeriodBegin
#define  SET_PTCPSTATUS_GREENPERIODBEGIN(a, b)    (a)->lpstLLDPPTCPSTATUSLocal->dwGreenPeriodBegin = b
#define  GET_PTCPSTATUS_SYNCMASTERMAC(a)          (a)->lpstLLDPPTCPSTATUSLocal->bySyncMasterMac
#define  SET_PTCPSTATUS_SYNCMASTERMAC(a, b)       OS_MEMCOPY((a)->lpstLLDPPTCPSTATUSLocal->bySyncMasterMac, b, MAC_ADDRESS_LEN)
#define  GET_PTCPSTATUS_SUBDOMAINUUID(a)          (a)->lpstLLDPPTCPSTATUSLocal->bySubDomainUUID
#define  SET_PTCPSTATUS_SUBDOMAINUUID(a, b)       OS_MEMCOPY((a)->lpstLLDPPTCPSTATUSLocal->bySubDomainUUID, b, PN_UUID_LEN)
#define  GET_PTCPSTATUS_IRDATAUUID(a)             (a)->lpstLLDPPTCPSTATUSLocal->byIRDATAUUID
#define  SET_PTCPSTATUS_IRDATAUUID(a, b)          OS_MEMCOPY((a)->lpstLLDPPTCPSTATUSLocal->byIRDATAUUID, b, PN_UUID_LEN)

#define  GET_PORTDELAY_RXLOCAL(a)                (a)->lpstLLDPPORTDELAYLocal->dwPortRxDelayLocal
#define  SET_PORTDELAY_RXLOCAL(a, b)             (a)->lpstLLDPPORTDELAYLocal->dwPortRxDelayLocal = b
#define  GET_PORTDELAY_RXREMOTE(a)               (a)->lpstLLDPPORTDELAYLocal->dwPortRxDelayRemote
#define  SET_PORTDELAY_RXREMOTE(a, b)            (a)->lpstLLDPPORTDELAYLocal->dwPortRxDelayRemote = b
#define  GET_PORTDELAY_TXLOCAL(a)                (a)->lpstLLDPPORTDELAYLocal->dwPortTxDelayLocal
#define  SET_PORTDELAY_TXLOCAL(a, b)             (a)->lpstLLDPPORTDELAYLocal->dwPortTxDelayLocal = b
#define  GET_PORTDELAY_TXREMOTE(a)               (a)->lpstLLDPPORTDELAYLocal->dwPortTxDelayRemote
#define  SET_PORTDELAY_TXREMOTE(a, b)            (a)->lpstLLDPPORTDELAYLocal->dwPortTxDelayRemote = b
#define  GET_PORTDELAY_CABLELOCAL(a)             (a)->lpstLLDPPORTDELAYLocal->dwPortCableDelaylocal
#define  SET_PORTDELAY_CABLELOCAL(a, b)          (a)->lpstLLDPPORTDELAYLocal->dwPortCableDelaylocal = b

/* Function prototypes */
APP_WORD InitialiseLocalMIB          (LPST_LLDP_INIT lpstLldpInit, APP_LPWORD lpwStatus);
APP_WORD CleanLocalMIB               (APP_LPWORD lpwStatus);
APP_WORD UpdateLocalPort             (APP_BYTE byPortNumber, APP_BOOL bLLDPPortEnabled, APP_LPWORD lpwStatus);
APP_WORD SetLocalConfig              (LPST_LLDP_CONFIG lpstLldpConfig);
APP_WORD GetLocalConfig              (LPST_LLDP_CONFIG lpstLldpConfig);
APP_WORD SetPortConfig               (LPST_LLDP_PORT_CONFIG lpstPortConfig, APP_BOOL bUpdatePort);
APP_WORD GetPortConfig               (LPST_LLDP_PORT_CONFIG lpstPortConfig, APP_BYTE byIndex);
APP_WORD SetLocSystemConfig          (LPST_LLDP_LOCAL_SYSTEM_CONFIG lpstLocSystemConfig, APP_BOOL bFullUpdate);
APP_WORD GetLocSystemConfig          (LPST_LLDP_LOCAL_SYSTEM_CONFIG lpstLocSystemConfig);
APP_WORD SetAdminStatusPort          (APP_BYTE byPortIndex, APP_BYTE byAdminStatus, APP_LPWORD lpwStatus);
APP_WORD GetAdminStatusPort          (APP_BYTE byPortIndex, APP_LPBYTE lpbyAdminStatus, APP_LPWORD lpwStatus);
APP_WORD SetLocalPort                (LPST_LLDP_PORT lpstPort, APP_BYTE byIndex, APP_LPWORD lpwStatus);
APP_WORD GetLocalPort                (LPST_LLDP_PORT lpstPort, APP_BYTE byIndex);
APP_BYTE GetPortIndex                (LPST_LLDP_PORT lpstPort);
APP_VOID SetLocMgtAddr               (APP_BYTE byIndex, APP_BYTE byMgtAddrSubType, APP_DWORD dwMgtAddr, APP_BYTE byMgtAddrItfSubType, APP_DWORD dwMgtAddrItfNumber, APP_BYTE byMgtAddrOidLength, APP_LPBYTE sOID) ;
APP_WORD GetLocMgtAddr               (APP_BYTE byIndex, APP_LPBYTE lpbyMgtAddrSubType, APP_LPDWORD lpdwMgtAddr, APP_LPBYTE lpbyMgtAddrItfSubType, APP_LPDWORD lpdwMgtAddrItfNumber, APP_LPBYTE lpbyMgtAddrOidLength, APP_LPBYTE sOID);
APP_WORD GetPortRef                  (APP_BYTE byPortIndex, LPST_LLDP_PORT *lpstPort);
APP_VOID SetPortSomethingChanged     (APP_BYTE byPortIndex);
APP_VOID SetALLPortNewTimeParameters (APP_VOID);
APP_VOID SetAllPortSomethingChanged  (APP_VOID);
APP_VOID SendFrameIfAllPortSomethingChanged  (APP_VOID);
APP_VOID SendFrameIfPortSomethingChanged(APP_BYTE byPortIndex);

#ifdef LLDP_STD_DESC_SUPPORT
APP_WORD SetPortDescription  (APP_BYTE byPortIndex, APP_LPBYTE byPortDesc);
APP_WORD GetPortDescription  (APP_BYTE byPortIndex, APP_LPBYTE byPortDesc);
APP_WORD SetSystemDescription(APP_LPBYTE bySysName, APP_LPBYTE bySysDesc, APP_WORD wSysCap, APP_WORD wSysCapEnabled);
APP_WORD GetSystemDescription(APP_LPBYTE bySysName, APP_LPBYTE bySysDesc, APP_LPWORD lpwSysCap, APP_LPWORD lpwSysCapEnabled);
#endif

APP_VOID LLDPDUHeader      (LPST_LLDP_PORT lpPort);
APP_VOID CreateChassisIDTLV(LPST_LLDP_PORT lpPort);
APP_VOID CreatePortIDTLV   (LPST_LLDP_PORT lpPort);
APP_VOID CreateENDOFLLDPDU (LPST_LLDP_PORT lpPort);
APP_VOID CreatePNAliasTLV  (LPST_LLDP_PORT lpPort);
APP_VOID CleanTlvList      (LPST_LLDP_PORT lpPort);
APP_VOID CreateTTLTLV      (LPST_LLDP_PORT lpPort, APP_BOOL bUseNullTTL);
APP_VOID CreateMgtAddrTLV  (LPST_LLDP_PORT lpPort, APP_BYTE byMgtAddrIndex);
APP_WORD BuildMsapID       (APP_LPBYTE sMSAPID, LPST_LLDP_PORT lpPort);

#ifdef LLDP_STD_DESC_SUPPORT
APP_VOID CreatePortDescTLV(LPST_LLDP_PORT lpPort);
APP_VOID CreateSysNameTLV (LPST_LLDP_PORT lpPort);
APP_VOID CreateSysDescTLV (LPST_LLDP_PORT lpPort);
APP_VOID CreateSysCapTLV  (LPST_LLDP_PORT lpPort);
#endif

#endif  /* END OF HEADER FILE */
