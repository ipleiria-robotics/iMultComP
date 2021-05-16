/*
*---------------------------------------------------------------------------*
*  Copyright (C) 2012 Woodhead Software & Electonics. All rights reserved.  *
*---------------------------------------------------------------------------*
*  This program is protected by international copyright.                    *
*                                                                           *
*  The use of this software including but not limited to its Source Code    *
*  is subject to restrictions as agreed in the license agreement between    *
*  you and Woodhead.                                                        *
*  Copying or distribution is not allowed unless expressly permitted        *
*  according to your license agreement with Woodhead.                       *
*---------------------------------------------------------------------------*
*                                                                           *
*  Project             : STACK PROFINET Device                              *
*  Component           :                                                    *
* $Workfile:: rdcbdev.h                                                    $*
* $Revision:: 27                                                           $*
* $Date:: 3/01/17 15:18                                                    $*
*                                                                           *
*                                                                           */
/****************************************************************************/
/**
* @file rdcbdev.h
* @brief Callback from stack to Device User Application (for internal read)
*
* Callback from stack to Device User Application (for internal read).
*
*****************************************************************************/
#ifndef _RDCBDEV_H
#define _RDCBDEV_H

  /*Define from specification: these value can be use but not change */

  /*MAUType: for more  value see PN Specification Table 404/405*/
  #define MAU_TYPE_10BASETXFD  0x000A
  #define MAU_TYPE_10BASETXHD  0x000B
  #define MAU_TYPE_100BASETXFD 0x0010
  #define MAU_TYPE_100BASETXHD 0x0011

  /*
  Multicast BOndary:
  Bit |Value |Meaning
  --------------------------------------------------------------------
  0   | 1    |Block the multicast MAC address 01-0E-CF-00-02-00
     | 0    |Do not block the multicast MAC address 01-0E-CF-00-02-00
  --------------------------------------------------------------------
  ... | 1    |Block the multicast MAC address 01-0E-CF-00-02-xx
     | 0    |Do not block the multicast MAC address 01-0E-CF-00-02-xx
  --------------------------------------------------------------------
  31  | 1    |Block the multicast MAC address 01-0E-CF-00-02-1F
     | 0    |Do not block the multicast MAC address 01-0E-CF-00-02-1F
  */
  #define MULTICAST_BOUNDARY       0x0000  /* Nothing block */

  /*Link port state: for more  value see PN Specification Table 414/415   */
  #define LINK_PORT_STATE_UP       0x01     /* 0x01 Up (ready to pass packets) */
  #define LINK_PORT_STATE_DOWN     0x02     /* 0x02 Down                       */

  /* MediaType */
  #define MEDIA_TYPE_UNKNOW         0x00    /* Unknown             */
  #define MEDIA_TYPE_COPPER         0x01    /* Copper cable        */
  #define MEDIA_TYPE_FIBER_OPTIC    0x02    /* Fiber optic cable   */
  #define MEDIA_TYPE_RADIO          0x03    /* Radio communication */

  #define PN_MRP_DEFAULT_DOMAIN_NAME "default-mrpdomain"
  #define PN_MRP_DEFAULT_DOMAIN_NAME_LEN 17

  #define PN_MRP_DISABLE 0
  #define PN_MRP_CLIENT  1
  #define PN_MRP_MANAGER 2

  APP_WORD APP_FAR PND_CB_Lookup_Data              (APP_DWORD  dwDRContextInd, APP_LPBYTE lpbyDeviceType, APP_LPBYTE lpbyDeviceTypeLen, APP_LPBYTE lpbyOrderId, APP_LPBYTE lpbyOrderIdLen, APP_LPBYTE lpbyHWRevision, APP_LPBYTE lpbyHWRevisionLen, APP_LPBYTE lpbySWRevisionPrefix, APP_LPBYTE lpbySWRevision, APP_LPBYTE lpbySWRevisionLen);

#ifdef RECORD_INTERNAL

  #ifdef RECORD_INTERNAL_PORT_STATISTIC
  APP_WORD APP_FAR PND_CB_Get_PortStatistic        (APP_DWORD dwDRContextInd, APP_BYTE byPort, APP_LPDWORD  lpdwIfInOctets,APP_LPDWORD  lpdwifOutOctets, APP_LPDWORD  lpdwifInDiscards, APP_LPDWORD  lpdwifOutDiscards, APP_LPDWORD  lpdwifInErrors, APP_LPDWORD  lpdwifOutErrors);
  #endif
  #ifdef RECORD_INTERNAL_IO
  APP_WORD APP_FAR PND_CB_Get_Input_data           (APP_DWORD dwARContextInd, APP_DWORD dwDRContextInd, APP_DWORD  dwApi, APP_WORD  wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIOCS, APP_LPBYTE lpbyIOPS, APP_LPBYTE lpbyData,APP_LPWORD lpwLenData);
  APP_WORD APP_FAR PND_CB_Get_Output_data          (APP_DWORD dwARContextInd, APP_DWORD dwDRContextInd, APP_DWORD  dwApi, APP_WORD  wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIOCS, APP_LPBYTE lpbyIOPS, APP_LPBYTE lpbyData,APP_LPWORD lpwLenData, APP_LPWORD lpwSubstitueActive);
  APP_WORD APP_FAR PND_CB_Get_OutputSubsTitute_data(APP_DWORD dwARContextInd, APP_DWORD dwDRContextInd, APP_DWORD  dwApi, APP_WORD  wSlot, APP_WORD wSubSlot, APP_WORD wSubstitueActive, APP_LPWORD lpwSubstitue, APP_LPBYTE lpbyIOCS, APP_LPBYTE lpbyIOPS, APP_LPBYTE lpbyData,APP_LPWORD lpwLenData);

  #endif

  #ifdef RECORD_INTERNAL_IM
  APP_WORD APP_FAR PND_CB_Get_Im0_Data             (APP_DWORD dwARContextInd, APP_DWORD  dwDRContextInd, APP_DWORD  dwAPI, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyOrderId , APP_LPBYTE lpbyOrderIdLen, APP_LPBYTE lpbySerialNumber, APP_LPBYTE lpbySerialNumberLen, APP_LPWORD lpwHardwareRevision, APP_LPBYTE lpbySWRevision,  APP_LPBYTE lpbySWFunctionalEnhancement, APP_LPBYTE lpbySWBugFix, APP_LPBYTE lpbySWInternalChange, APP_LPWORD lpwSWCounter, APP_LPWORD lpwIMSupported, APP_LPBYTE lpbyVendorIdHigh, APP_LPBYTE lpbyVendorIdLow);
  APP_WORD APP_FAR PND_CB_IM0FilterData_Allowed    (APP_DWORD dwDRContextInd, APP_DWORD dwAPI, APP_WORD wSlot, APP_WORD wSubSlot, APP_WORD wBlocType);
  #endif /* RECORD_INTERNAL_IM */

  #ifdef RECORD_INTERNAL_INTERFACE
  APP_WORD APP_FAR PND_CB_Get_StationName          (APP_DWORD dwARContextInd, APP_DWORD dwDRContextInd, APP_LPBYTE lpbyStationName, APP_LPBYTE lpbyStationNameLen);
  APP_WORD APP_FAR PND_CB_Get_Port_Data            (APP_DWORD dwARContextInd, APP_DWORD dwDRContextInd, APP_BYTE byPort, APP_LPBYTE lpbyLengthOwnPortId, APP_LPBYTE lpbyOwnPortId, APP_LPBYTE lpbyNumberOfPeers, APP_LPBYTE lpbyFirstPeer, APP_LPWORD lpwMAUType, APP_LPDWORD lpdwMulticastBoundary, APP_LPWORD lpwLinkState, APP_LPDWORD lpdwMediaType, APP_LPDWORD lpdwLineDelay);
  APP_WORD APP_FAR PND_CB_Get_Peer_Data            (APP_DWORD dwARContextInd, APP_DWORD dwDRContextInd, APP_BYTE byPort, APP_LPBYTE lpbyPeersNumber, APP_LPBYTE lpbyLengthPeerPortID, APP_LPBYTE lpbyPeerPortID, APP_LPBYTE lpbyLengthPeerChassisID, APP_LPBYTE lpbyPeerChassisID, APP_LPDWORD lpdwLineDelay, APP_LPBYTE lpbyPeerMACAddress);
  #endif /* RECORD_INTERNAL_INTERFACE */

#endif /* RECORD_INTERNAL */

  #ifdef LLDP_SUPPORTED
  APP_WORD lldpReadInternal_getPortId              (APP_BYTE byPort, APP_LPBYTE lpbyLengthPortId, APP_LPBYTE lpbyPortId);
  APP_WORD lldpReadInternal_getPortData            (APP_BYTE byPort, APP_LPBYTE lpbyLengthOwnPortId, APP_LPBYTE lpbyOwnPortId);
  APP_WORD lldpReadInternal_getChassiId            (APP_LPBYTE lpbyChassiId, APP_LPBYTE lpbyChassidIdLen);
  #endif /* LLDP_SUPPORTED */
  #ifdef LLDP_RECEIVER
  APP_WORD lldpReadInternal_getPortNbPeers         (APP_BYTE byPort, APP_LPBYTE lpbyNumberOfPeers, APP_LPBYTE lpbyFirstPeers);
  APP_WORD APP_FAR lldpReadInternal_getPeerData    (APP_BYTE byPort, APP_LPBYTE lpbyPeersNumber, APP_LPBYTE lpbyLengthPeerPortID, APP_LPBYTE lpbyPeerPortID, APP_LPBYTE lpbyLengthPeerChassisID, APP_LPBYTE lpbyPeerChassisID, APP_LPDWORD lpdwLineDelay, APP_LPBYTE lpbyPeerMACAddress);
  #endif /* LLDP_RECEIVER */
#endif
