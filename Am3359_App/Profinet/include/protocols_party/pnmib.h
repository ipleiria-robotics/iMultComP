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
/* $Workfile:: pnmib.h                                                      $*/
/* $Revision:: 7                                                            $*/
/* $Date:: 7/04/15 16:03                                                    $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*  -PN MIB management                                                       */
/*---------------------------------------------------------------------------*/
#ifndef _PNMIB_H
#define _PNMIB_H

#define PN_MRP_DOMAIN_UUID_LEN         16
#define PN_UUID_LEN                    16

typedef struct _LLDP_LOCAL_PN_MIB{
  APP_WORD wPNPortStatusRt2;
  APP_WORD wPNPortStatusRt3;


  APP_BYTE UUID_MRPDomain[PN_MRP_DOMAIN_UUID_LEN];
  APP_WORD wMRRT_PortStatus;

  APP_BYTE sPNIOChassisMAC[MAC_ADDRESS_LEN];
  APP_BYTE byPNTxEnabled;
}ST_LLDP_LOCAL_PN_MIB, APP_FAR * LPST_LLDP_LOCAL_PN_MIB;


typedef struct _LLDP_REMOTE_PN_MIB{
  APP_WORD wRemPNPortStatusRt2;
  APP_WORD wRemPNPortStatusRt3;
  APP_BYTE UUID_RemMRPDomain[PN_MRP_DOMAIN_UUID_LEN];
  APP_WORD wMRRT_RemPortStatus;
  APP_BYTE sRemPNIOChassisMAC[MAC_ADDRESS_LEN];
}ST_LLDP_REMOTE_PN_MIB, APP_FAR * LPST_LLDP_REMOTE_PN_MIB;

typedef struct _LLDP_PTCP_STATUS{
  APP_DWORD dwLenghtOfPeriod;
  APP_DWORD dwRedPeriodBegin;
  APP_DWORD dwOrangePeriodBegin;
  APP_DWORD dwGreenPeriodBegin;
  APP_BYTE  bySyncMasterMac[MAC_ADDRESS_LEN];
  APP_BYTE  bySubDomainUUID[PN_UUID_LEN];
  APP_BYTE  byIRDATAUUID[PN_UUID_LEN];

}ST_LLDP_PTCP_STATUS, APP_FAR * LPST_LLDP_PTCP_STATUS;

typedef struct _LLDP_PORT_DELAY{
  APP_DWORD dwPortRxDelayLocal;
  APP_DWORD dwPortRxDelayRemote;
  APP_DWORD dwPortTxDelayLocal;
  APP_DWORD dwPortTxDelayRemote;
  APP_DWORD dwPortCableDelaylocal;
}ST_LLDP_PORT_DELAY, APP_FAR * LPST_LLDP_PORT_DELAY;

/* PN Tx Enabled Masks */
#define LLDP_PNIO_DELAY_MASK           1
#define LLDP_PNIO_PORT_STATUS_MASK     2
#define LLDP_PNIO_ALIAS_MASK           4    /* Not supported Yet */
#define LLDP_PNIO_MRPPORT_STATUS_MASK  8
#define LLDP_PNIO_PTCP_STATUS_MASK     16
#define LLDP_PNIO_CHASSIS_MAC_MASK     32

/* Case no PN */
#ifndef LLDP_SUPPORT_PN
#define LLDP_PNIO_SUPPORTED         0
#endif

/* case pn with out anything */
#ifdef LLDP_SUPPORT_PN

/* Case no MRP */
#ifdef LLDP_SUPPORT_MRP
  #define  LLDP_PNIO_MRP_ACTIVATED (LLDP_PNIO_MRPPORT_STATUS_MASK)
#else
  #define  LLDP_PNIO_MRP_ACTIVATED 0
#endif

/* No IRT */
#ifdef PN_RTC3_SUPPORTED
  #define  LLDP_PNIO_RTC3_ACTIVATED (LLDP_PNIO_DELAY_MASK|LLDP_PNIO_PTCP_STATUS_MASK)
#else
  #define  LLDP_PNIO_RTC3_ACTIVATED 0
#endif

#define LLDP_PNIO_SUPPORTED  (LLDP_PNIO_PORT_STATUS_MASK | LLDP_PNIO_CHASSIS_MAC_MASK | LLDP_PNIO_MRP_ACTIVATED | LLDP_PNIO_RTC3_ACTIVATED)

#endif

/* PORT STATUS RT2 */
#define LLDP_PNIO_PORT_STATUS_RT2_OFF     0
#define LLDP_PNIO_PORT_STATUS_RT2_CNF     1
#define LLDP_PNIO_PORT_STATUS_RT2_RUN     2


/* PORT STATUS RT3 */
#define LLDP_PNIO_PORT_STATUS_RT3_OFF     0
#define LLDP_PNIO_PORT_STATUS_RT3_CNF     1
#define LLDP_PNIO_PORT_STATUS_RT3_UP      2
#define LLDP_PNIO_PORT_STATUS_RT3_DOWN    3
#define LLDP_PNIO_PORT_STATUS_RT3_RUN     4


/* PORT STATUS MRRT */
#define LLDP_PNIO_PORT_STATUS_MRRT_OFF     0
#define LLDP_PNIO_PORT_STATUS_MRRT_CNF     1
#define LLDP_PNIO_PORT_STATUS_MRRT_UP      2



APP_WORD InitialisePNLocalMIB(APP_LPWORD lpwStatus);

#endif  /* END OF HEADER FILE */
