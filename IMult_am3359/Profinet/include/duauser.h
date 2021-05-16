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
/*  Project             : mrp callback                                       */
/*  Component           : MRP  managment                                     */
/* $Workfile:: duauser.h                                                    $*/
/* $Revision:: 3                                                            $*/
/* $Date:: 19/10/16 14:38                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#ifndef _USER_H
#define _USER_H

#include "dua.h"

#ifdef NETWORK_USER
#define _PN_EVENT_PORT_ 0x8899 /* port where event can be read */
#define _PN_EVENT_SIZE_ 5 /* port where event can be read : we want read an APP_WORD*/

APP_WORD initReceiveEvent();
APP_WORD receiveNetworkEvent();
#endif


int main_pn();
APP_VOID   UserChangeName           (APP_LPCHAR lpDeviceName, APP_WORD wBlockQ);
APP_WORD   UserChangeIp             (APP_DWORD dwIp, APP_DWORD dwSubMask, APP_DWORD dwGateway, APP_WORD wBlockQualifier);
APP_WORD   UserResetIp              (APP_VOID);
APP_DWORD  UserLoadRemanentConf(LPST_PND_REMANENT_DATA lpstRemData);
APP_DWORD  UserSaveRemanentConf(LPST_PND_REMANENT_DATA lpstRemData, APP_LPBYTE lpbyDataChange);
APP_DWORD  UserSetDefaultRemanentConf(LPST_PND_REMANENT_DATA lpstRemData);
APP_CHAR * UserGetInterfaceDeviceIPString(APP_VOID);
APP_DWORD  UserGetDeviceIP(LPST_PND_REMANENT_DATA lpstRemData);
APP_DWORD  UserGetDeviceMASK(LPST_PND_REMANENT_DATA lpstRemData);
APP_DWORD  UserGetDeviceGATEWAY(LPST_PND_REMANENT_DATA lpstRemData);
APP_WORD   UserSetDeviceIP(APP_DWORD dwIpAddr, APP_DWORD dwMask, APP_DWORD dwGtw);
APP_WORD   UserApplyConf(LPST_PND_REMANENT_DATA lpstOldRemData, LPST_PND_REMANENT_DATA lpstRemData);
APP_DWORD  UserResetToFactory(LPST_PND_REMANENT_DATA lpstRemData);
APP_DWORD  UserFactoryReset(LPST_PND_REMANENT_DATA lpstRemData);
#ifdef PN_RTC3_SUPPORTED
APP_WORD   UserRTC3PhaseAssigment(APP_DWORD dwNumberofPorts, LPST_PDIRBEGINENDDATA lpstPDIrBeginData);
APP_WORD   UserRTC3PortAssigment(APP_DWORD dwNumberofPorts, LPST_PDIRBEGINENDDATA lpstPDIrBeginData);
APP_WORD   UserRTC3SetFrameID(APP_WORD wStartOfFrameId, APP_WORD wEndOfFrameId);
APP_WORD   UserRTC3SetInterface( LPST_CLR_RPC_UUID lpIrdataUUID, APP_DWORD dwMaxBrideDelay, APP_DWORD dwNumberOfPort, APP_DWORD dwMaxPortTxDelay1, APP_DWORD dwMaxPorRxDelay1, APP_DWORD dwMaxLineRxDelayport1, APP_DWORD dwYellowtimePort1, APP_DWORD dwMaxPortTxDelay2, APP_DWORD dwMaxPortRxDelay2, APP_DWORD dwMaxLineRxDelayport2, APP_DWORD dwYellowtimePort2);
APP_WORD   UserSetPTCP(LPST_PND_REMANENT_DATA lpstRemData);
APP_WORD   UserSysEthGetCableDelay(APP_WORD wPort, APP_LPDWORD lpdwCableDelay);
#endif

APP_WORD   UserSysEthSetPortMautype(APP_WORD wPort, APP_WORD wMAUType, APP_BYTE byAutoNeg);
APP_WORD   UserSysEthGetPortMautype(APP_WORD wPort, APP_LPWORD lpwMAUType);
APP_WORD   UserSysEthGetStatistics(APP_WORD wPort, APP_LPDWORD lpdwIfInOctets,  APP_LPDWORD lpdwifInDiscards, APP_LPDWORD lpdwifInErrors, APP_LPDWORD lpdwifOutOctets, APP_LPDWORD lpdwifOutDiscards, APP_LPDWORD lpdwifOutErrors);
APP_WORD   UserSysEthGetLinkState(APP_WORD wPort, APP_LPWORD  lpwLinkState);

#define EXIT_PN_EVT               0     /* 'e' */
#define ALARM_PN_EVT              1     /* 'a' */
#define PLUG_SUBMODULE_EVT        2     /* 'p' */
#define PULL_SUBMODULE_EVT        3     /* 'u' */
#define GET_INPUT                 4     /* 'i' */
#define SET_OUTPUT                5     /* 'o' */
#define CHANGE_OUTPUT_APDU_STATUS 6     /* 'S' */
#define CHANGE_OUTPUT_DATA        7     /* 'O' */
#define TOGGLE_OUTPUT_IOPS        8     /* 'P' */
#define TOGGLE_OUTPUT_IOCS        9     /* 'C' */
#define ABORT_REQ                10     /* 'A' */
#define SEND_APP_READY           11     /* 'r' */
#define START_TEST               13     /* 't' */
#define SEND_DCP_HELLO           14     /* 'h' */
#define DISPLAY_SHARED_MEMORY    15     /* 's' */

#define UNSUPPORTED_PN_EVT      254
#define NO_PN_EVENT             255

APP_WORD UserEvent(APP_VOID);

typedef struct _USER_MAILBOX_TYPE
{
  APP_BYTE    byMsg[512];
  APP_WORD    wCodeFnct;
}USER_MAILBOX_TYPE, APP_FAR * LPUSER_MAILBOX_TYPE;

#define MAX_USER_MSG 10
#define USER_LINKINFO 1

APP_DWORD UserInit        (APP_VOID);
APP_DWORD UserExit        (APP_VOID);
APP_WORD  UserEvent       (APP_VOID);
APP_DWORD UserManageEvent (APP_DWORD dwTimeout);
APP_WORD  UserApplyConf   (LPST_PND_REMANENT_DATA lpstOldRemData, LPST_PND_REMANENT_DATA lpstRemData);

#define USER_SYS_SWI_PORT_DISABLE       0
#define USER_SYS_SWI_PORT_BLOCKING      1
#define USER_SYS_SWI_PORT_LEARNING      2
#define USER_SYS_SWI_PORT_FORWARDING    3

/* Link state */
#define USER_SYS_SWI_LINK_DOWN          0
#define USER_SYS_SWI_LINK_UP            1

/* Mautype */
#define USER_SYS_ETH_MAUTYPE_NONE               0x0000
#define USER_SYS_ETH_MAUTYPE_10BASETXFD         0x000A
#define USER_SYS_ETH_MAUTYPE_10BASETXHD         0x000B
#define USER_SYS_ETH_MAUTYPE_100BASETXFD        0x0010  /* Default (MediaType Copper) */
#define USER_SYS_ETH_MAUTYPE_100BASETXHD        0x0011

#define REMANENT_DATA_VERSION   0x01000001 /*1.0.0.1*/

/************************** EEPROM Related **********************/
#define SPI_EEPROM_DEVICECONF_OFFSET 0x300000

#define DWORD_BITFIELD_BIT0 0x0001
#define DWORD_BITFIELD_BIT1 0x0002
#define DWORD_BITFIELD_BIT2 0x0004
#define DWORD_BITFIELD_BIT3 0x0008

#endif /* _USER_H */

