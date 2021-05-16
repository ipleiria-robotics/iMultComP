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
*  Component           : pncbdev header                                     *
* $Workfile:: extcbdev.h                                                   $*
* $Revision:: 16                                                           $*
* $Date:: 7/04/16 9:18                                                     $*
*                                                                           *
*                                                                           */
/****************************************************************************/
/**
* @file extcbdev.h
* @brief Callback from stack to device user application header.
*
*****************************************************************************/

#ifndef _EXTCBDEV_H
#define _EXTCBDEV_H

#ifdef MRP_SUPPORTED
#include "mrp_pkg.h"
#include "msg_mrp.h"
#endif

#ifdef PN_RTC3_SUPPORTED
#include "ptcp.h"
#endif

#include "pncbdev.h"
#include "pnio_pdu.h"

typedef struct _usr_check_peer_data
{
  APP_BYTE          byLenPeerPortID;
  APP_CHAR          szPeerPortID[40];
  APP_BYTE          byLenPeerChassisID;
  APP_CHAR          szPeerChassisID[NAMELEN];
}ST_USR_CHECK_PEER_DATA, APP_FAR * LPST_USR_CHECK_PEER_DATA;


#ifdef MRP_SUPPORTED
APP_WORD APP_FAR PND_CB_ExtConfCheckMRPItf                     (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIsCheckMRPActivate, LPST_MRP_UUID lpstCheckMrpUUID, APP_BYTE byCheckMediaRedundancyManager,  APP_BYTE byCheckMRPDomainUUID);
APP_WORD APP_FAR PND_CB_ExtConfAdjustMRP                       (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIsAdjustMRPActivate               , LPST_MRP_UUID lpstAdjustMrpUUID);
APP_WORD APP_FAR PND_CB_ExtConfAdjustMRPItf                    (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIsAdjustMRPActivate, LPST_MRP_MANAGER_PARAM lpstMrpAdjustManagerParam, LPST_MRP_CLIENT_PARAM lpstMrpAdjustClientParam, LPST_MRP_UUID lpsrMrpAdjustUUID, APP_LPBYTE lpbyMRPAdjustDomainName, APP_BYTE byMRPAdjustLengthDomainName, APP_WORD  wMRPAdjustRole);
APP_WORD APP_FAR PND_CB_ExtConfGetCheckMRPItf                  (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsCheckMRPActivate, LPST_MRP_UUID lpstCheckMrpUUID, APP_LPBYTE lpbyCheckMediaRedundancyManager,  APP_LPBYTE lpbyCheckMRPDomainUUID);
APP_WORD APP_FAR PND_CB_ExtConfGetAdjustMRP                    (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsAdjustMRPActivate               , LPST_MRP_UUID lpstAdjustMrpUUID);
APP_WORD APP_FAR PND_CB_ExtConfGetAdjustMRPItf                 (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsAdjustMRPActivate, LPST_MRP_MANAGER_PARAM lpstMrpAdjustManagerParam, LPST_MRP_CLIENT_PARAM lpstMrpAdjustClientParam, LPST_MRP_UUID lpsrMrpAdjustUUID, APP_LPBYTE lpbyMRPAdjustDomainName, APP_LPBYTE lpbyMRPAdjustLengthDomainName, APP_LPWORD  lpwMRPAdjustRole);
#endif

APP_WORD APP_FAR PND_Write_PortDataCheck_ind                   (APP_DWORD dwARContextInd, APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wIndex, APP_WORD wSlot, APP_WORD wSubSlot, APP_DWORD  dwSizeOfData, APP_LPBYTE lpbyData, LPST_PNIO_RW_STATUS lpStatus, APP_LPWORD lpwbyAddData1, APP_LPWORD lpwbyAddData2);

APP_WORD APP_FAR PND_CB_ExtConfCheckPeer                       (APP_DWORD dwDRContextInd, APP_BYTE byCount, APP_BYTE byIsCheckPeerActive, LPST_USR_CHECK_PEER_DATA lpstCheckPeer, APP_DWORD dwAPI, APP_WORD wSlot, APP_WORD wSubSlot);
APP_WORD APP_FAR PND_CB_ExtConfCheckMAUType                    (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIsCheckMauTypeActive, APP_WORD wCheckMautType);
APP_WORD APP_FAR PND_CB_ExtConfCheckMAUTypeMode                (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIsCheckMauTypeModeActive, APP_WORD wCheckMAUTypeMode);
APP_WORD APP_FAR PND_CB_ExtConfCheckLinkState                  (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byCheckLinkStateActivate, APP_BYTE byCheckLinkState_Link, APP_BYTE byCheckLinkState_Port);
APP_WORD APP_FAR PND_CB_ExtConfCheckLineDelay                  (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byCheckLineDelayeActivate, APP_BYTE byCheckLineDelayFormatIndicator, APP_DWORD dwCheckLineDelayValue);
APP_WORD APP_FAR PND_CB_ExtConfCheckSyncMode                   (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byCheckSyncModeActivate, APP_BYTE byCheckSyncMode_CableDelay, APP_BYTE byCheckSyncMode_SyncMaster);

APP_WORD APP_FAR PND_CB_ExtConfGetCheckPeer                    (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsCheckPeerActive, APP_LPBYTE lpbyCount, LPST_USR_CHECK_PEER_DATA lpstCheckPeer);
APP_WORD APP_FAR PND_CB_ExtConfGetCheckMAUType                 (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsCheckMauTypeActive, APP_LPWORD lpwCheckMautType);
APP_WORD APP_FAR PND_CB_ExtConfGetCheckMAUTypeMode             (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsCheckMauTypeModeActive,APP_LPWORD lpwCheckMAUTypeMode);
APP_WORD APP_FAR PND_CB_ExtConfGetCheckLinkState               (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyCheckLinkStateActivate, APP_LPBYTE lpbyCheckLinkState_Link, APP_LPBYTE lpbyCheckLinkState_Port);
APP_WORD APP_FAR PND_CB_ExtConfGetCheckLineDelay               (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyCheckLineDelayeActivate, APP_LPBYTE lpbyCheckLineDelayFormatIndicator, APP_LPDWORD lpdwCheckLineDelayValue);
APP_WORD APP_FAR PND_CB_ExtConfGetCheckSyncMode                (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyCheckSyncModeActivate, APP_LPBYTE lpbyCheckSyncMode_CableDelay, APP_LPBYTE lpbyCheckSyncMode_SyncMaster);

APP_WORD APP_FAR PND_CB_ExtConfAdjustDomainBoundary            (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIsAdjustDomainBoundaryActivate    , APP_DWORD dwAdjustDomainBoundaryIngress, APP_DWORD dwAdjustDomainBoundaryEgress);
APP_WORD APP_FAR PND_CB_ExtConfAdjustLinkState                 (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIsAdjustLinkStateActivate         , APP_BYTE  byAdjustLinkState_Link, APP_BYTE byAdjustLinkState_Port);
APP_WORD APP_FAR PND_CB_ExtConfAdjustMulticastBoundary         (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIsAdjustMulticastBoundaryActivate , APP_DWORD dwAdjustMulticastBoundary);
APP_WORD APP_FAR PND_CB_ExtConfAdjustMAUType                   (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIsAdjustMAUTypeActivate           , APP_WORD  wAdjustMAUType);
APP_WORD APP_FAR PND_CB_ExtConfAdjustPeerToPeerBoundary        (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIsAdjustPeerToPeerBoundaryActivate, APP_DWORD dwPeerToPeerBoundary);
APP_WORD APP_FAR PND_CB_ExtConfAdjustDCPBoundary               (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIsAdjustDCPBoundaryActivate       , APP_DWORD dwDCPBoundary);
APP_WORD APP_FAR PND_CB_ExtConfAdjustPreambuleLength           (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIsAdjustPreambuleLengthActivate   , APP_WORD  wPreambleLength);
APP_WORD APP_FAR PND_CB_ExtConfAdjustFsuItf                    (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIsAdjustFsuActivate, APP_BYTE byFsuEnable, APP_BYTE byFsuInitiativeDelayMode, APP_WORD  wHelloInterval, APP_WORD  wHelloRetryCount, APP_WORD  wHelloDelay);
APP_WORD APP_FAR PND_CB_ExtConfAdjustItf                       (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byItfAdjustEnable, APP_BYTE byMultiInterfaceNameOfDevice);
#ifdef PN_RTC3_SUPPORTED
APP_WORD APP_FAR PND_CB_ExtConfAdjustIrData                    (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIsAdjustIrDataEnable, ST_PDIRDATA stIrData);
APP_WORD APP_FAR PND_CB_ExtConfAdjustSyncData                  (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIsAdjustSyncDataEnable, ST_PDSYNCDATA stSyncData);
#endif
APP_WORD APP_FAR PND_CB_ExtConfGetAdjustDomainBoundary         (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsAdjustDomainBoundaryActivate    , APP_LPDWORD lpdwAdjustDomainBoundaryIngress, APP_LPDWORD lpdwAdjustDomainBoundaryEgress);
APP_WORD APP_FAR PND_CB_ExtConfGetAdjustLinkState              (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsAdjustLinkStateActivate         , APP_LPBYTE  lpbyAdjustLinkState_Link, APP_LPBYTE lpbyAdjustLinkState_Port);
APP_WORD APP_FAR PND_CB_ExtConfGetAdjustMulticastBoundary      (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsAdjustMulticastBoundaryActivate , APP_LPDWORD lpdwAdjustMulticastBoundary);
APP_WORD APP_FAR PND_CB_ExtConfGetAdjustMAUType                (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsAdjustMAUTypeActivate           , APP_LPWORD  lpwAdjustMAUType);
APP_WORD APP_FAR PND_CB_ExtConfGetAdjustPeerToPeerBoundary     (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsAdjustPeerToPeerBoundaryActivate, APP_LPDWORD lpdwPeerToPeerBoundary);
APP_WORD APP_FAR PND_CB_ExtConfGetAdjustDCPBoundary            (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsAdjustDCPBoundaryActivate       , APP_LPDWORD lpdwDCPBoundary);
APP_WORD APP_FAR PND_CB_ExtConfGetAdjustPreambuleLength        (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsAdjustPreambuleLengthActivate   , APP_LPWORD  lpwPreambleLength);
APP_WORD APP_FAR PND_CB_ExtConfGetAdjustFsuItf                 (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsAdjustFsuActivate, APP_LPBYTE lpbyFsuEnable, APP_LPBYTE lpbyFsuInitiativeDelayMode, APP_LPWORD  lpwHelloInterval, APP_LPWORD  lpwHelloRetryCount, APP_LPWORD  lpwHelloDelay);
APP_WORD APP_FAR PND_CB_ExtConfGetAdjustItf                    (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsAdjustInterfaceActivate, APP_LPBYTE lpbyMultiInterfaceNameOfDevice);
#ifdef PN_RTC3_SUPPORTED
APP_WORD APP_FAR PND_CB_ExtConfGetAdjustIrData                 (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsAdjustIrDataEnable, LPST_PDIRDATA lpstIrData);
APP_WORD APP_FAR PND_CB_ExtConfGetAdjustSyncData               (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyIsAdjustSyncDataEnable, LPST_PDSYNCDATA lpstSyncData);
#endif
APP_WORD APP_FAR PND_Peer_Mismatch_Ind                         (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_DWORD dwPeerState, APP_DWORD dwPeerErrorAppears);
APP_WORD APP_FAR PND_MRP_Mismatch_Ind                          (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_DWORD dwMRPState, APP_DWORD dwErrorAppears);
APP_WORD APP_FAR PND_PeerMAUType_Mismatch_Ind                  (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_DWORD dwErrorAppears, APP_DWORD dwLocalMAUTypeValue, APP_DWORD dwRemoteMAUTypeValue);
APP_WORD APP_FAR PND_PeerSyncDelay_Mismatch_Ind                (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_DWORD dwErrorAppears, APP_DWORD dwLocalLineDelayValue, APP_DWORD dwRemoteLineDelayValue);
APP_WORD APP_FAR PND_PeerSyncPTCPMaster_Mismatch_Ind           (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_DWORD dwErrorAppears, APP_LPBYTE lpbyLocalSyncMaster, APP_LPBYTE lpbyRemoteSyncMaster);
APP_WORD APP_FAR PND_MAUType_Mismatch_Ind                      (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_DWORD dwMAUTypeErrAppears, APP_WORD wExpectedMAUType, APP_WORD wRealMAUType);
APP_WORD APP_FAR PND_Link_Mismatch_Ind                         (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_DWORD dwLinkErrorAppears, APP_BYTE byExpectedLink, APP_BYTE byRealdLink, APP_BYTE byExpectedPortState, APP_BYTE byRealPortState);
APP_WORD APP_FAR PND_LineDelay_Mismatch_Ind                    (APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_DWORD dwLineDelayErrorAppears, APP_DWORD dwExpectedLineDelayValue, APP_DWORD dwRealLineDelayValue);

APP_WORD APP_FAR PND_CB_InformLinkOrMAUTypeChange_Cnf_Pos      (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot);
APP_WORD APP_FAR PND_CB_InformLinkOrMAUTypeChange_Cnf_Neg      (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_WORD wStatus);
APP_WORD APP_FAR PND_CB_InformLineDelayChange_Cnf_Pos          (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot);
APP_WORD APP_FAR PND_CB_InformLineDelayChange_Cnf_Neg          (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_WORD wStatus);
APP_WORD APP_FAR PND_CB_InformSyncStatusChange_Cnf_Pos         (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot);
APP_WORD APP_FAR PND_CB_InformSyncStatusChange_Cnf_Neg         (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_WORD wStatus);
APP_WORD APP_FAR PND_CB_InformPeerMAUTypeChange_Cnf_Pos        (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_BYTE byPortNumber, APP_LPBYTE lpbyMAc);
APP_WORD APP_FAR PND_CB_InformPeerMAUTypeChange_Cnf_Neg        (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_BYTE byPortNumber, APP_LPBYTE lpbyMAc, APP_WORD wStatus);
APP_WORD APP_FAR PND_CB_InformPeerMeasuredDelayChange_Cnf_Pos  (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_BYTE byPortNumber, APP_LPBYTE lpbyMAc);
APP_WORD APP_FAR PND_CB_InformPeerMeasuredDelayChange_Cnf_Neg  (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_BYTE byPortNumber, APP_LPBYTE lpbyMAc, APP_WORD wStatus);
APP_WORD APP_FAR PND_CB_InformPeerPTCPChange_Cnf_Pos           (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_BYTE byPortNumber, APP_LPBYTE lpbyMAc);
APP_WORD APP_FAR PND_CB_InformPeerPTCPChange_Cnf_Neg           (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_BYTE byPortNumber, APP_LPBYTE lpbyMAc, APP_WORD wStatus);
APP_WORD APP_FAR PND_CB_InformPeerPortStatusChange_Cnf_Pos     (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_BYTE byPortNumber, APP_LPBYTE lpbyMAc);
APP_WORD APP_FAR PND_CB_InformPeerPortStatusChange_Cnf_Neg     (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_BYTE byPortNumber, APP_LPBYTE lpbyMAc, APP_WORD wStatus);
#ifdef MRP_SUPPORTED
APP_WORD APP_FAR PND_CB_InformPeerMrpPortStatusChange_Cnf_Pos  (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_BYTE byPortNumber, APP_LPBYTE lpbyMAc);
APP_WORD APP_FAR PND_CB_InformPeerMrpPortStatusChange_Cnf_Neg  (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_BYTE byPortNumber, APP_LPBYTE lpbyMAc, APP_WORD wStatus);
#endif
APP_WORD APP_FAR PND_CB_RemovePeerData_Cnf_Pos                 (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_BYTE byPortNumber, APP_LPBYTE lpbyMAc);
APP_WORD APP_FAR PND_CB_RemovePeerData_Cnf_Neg                 (APP_DWORD dwDRContextInd, APP_LPVOID lpvContext, APP_BYTE byPortNumber, APP_LPBYTE lpbyMAc, APP_WORD wStatus);
APP_WORD APP_FAR PND_CB_RegisterAlias_Cnf_Neg                  (APP_DWORD dwDRContextInd, APP_LPBYTE lpbyAliasName, APP_DWORD dwStatus );
APP_WORD APP_FAR PND_CB_RegisterAlias_Cnf_Pos                  (APP_DWORD dwDRContextInd, APP_LPBYTE lpbyAliasName);
APP_WORD APP_FAR PND_CB_RemoveAlias_Cnf_Neg                    (APP_DWORD dwDRContextInd, APP_LPBYTE lpbyAliasName, APP_DWORD dwStatus );
APP_WORD APP_FAR PND_CB_RemoveAlias_Cnf_Pos                    (APP_DWORD dwDRContextInd, APP_LPBYTE lpbyAliasName);
APP_WORD APP_FAR PND_CB_UpdateAlias_Cnf_Pos                    (APP_DWORD dwDRContextInd, APP_LPBYTE lpbyAliasName);
APP_WORD APP_FAR PND_CB_UpdateAlias_Cnf_Neg                    (APP_DWORD dwDRContextInd, APP_LPBYTE lpbyAliasName, APP_DWORD dwStatus);

/*
 User help: values possible for Link stat: link (see specification for more details
        0x00         Reserved
        0x01         Up (ready to pass packets)
        0x02         Down
        0x03         Testing (in some test mode)
        0x04         Unknown (status cannot be determined)
        0x05         Dormant
        0x06         NotPresent
        0x07         LowerLayerDown
        0x08 – 0xFF  Reserved

 User help: values possible for Link state: port (see specification for more details
        0x00 Unknown
        0x01 Disabled / Discarding
        0x02 Blocking
        0x03 Listening
        0x04 Learning
        0x05 Forwarding
        0x06 Broken
        0x07 – 0xFF Reserved

 User help: values possible for MAUType (see specification for more details

        0x0000  Reserved for MediaType “Copper cable” and “Fiber optic cable”
                Used for MediaType “Radio communication”  Default for radio communication
        0x0005  10BaseT
        0x000A  10BaseTXHD
        0x000B  10BaseTXFD
        0x000C  10BaseFLHD
        0x000D  10BaseFLFD
        0x000F  100BaseTXHD
        0x0010  Default (MediaType Copper) 100BaseTXFD
        0x0011  100BaseFXHD
        0x0012  Default (MediaType Fiber optic) 100BaseFXFD
        0x0015  1000BaseXHD
        0x0016  1000BaseXFD
        0x0017  1000BaseLXHD
        0x0018  1000BaseLXFD
        0x0019  1000BaseSXHD
        0x001A  1000BaseSXFD
        0x001D  1000BaseTHD
        0x001E  1000BaseTFD
        0x001F  10GbaseX
        0x0020  10GbaseLX4
        0x0021  10GbaseR
        0x0022  10GbaseER
        0x0023  10GbaseLR
        0x0024  10GbaseSR
        0x0025  10GbaseW
        0x0026  10GbaseEW
        0x0027  10GbaseLW
        0x0028  10GbaseSW
        0x0029  10GbaseCX4
        0x002A  2BaseTL
        0x002B  10PassTS
        0x002C  100BaseBX10D
        0x002D  100BaseBX10U
        0x002E  100BaseLX10
        0x002F  1000BaseBX10D
        0x0030  1000BaseBX10U
        0x0031  1000BaseLX10
        0x0032  1000BasePX10D
        0x0033  1000BasePX10U
        0x0034  1000BasePX20D
        0x0035  1000BasePX20U
        0x0036  10GbaseT or 100BasePXFD
        0x0037  10GBaseLRM
        0x0038  1000BaseKX
        0x0039  1000BaseKX4
        0x003A  1000BaseKR
        0x003B  10G1GBasePRXD1
        0x003C  10G1GBasePRXD2
        0x003D  10G1GBasePRXD3
        0x003E  10G1GBasePRXU1
        0x003F  10G1GBasePRXU2
        0x0040  10G1GBasePRXU3
        0x0041  10GBasePRD1
        0x0042  10GBasePRD2
        0x0043  10GBasePRD3
        0x0044  10GBasePRU1
        0x0045  10GBasePRU3
        0x0046  40GbaseKR4
        0x0047  40GbaseCR4
        0x0048  40GbaseSR4
        0x0049  40GbaseFR
        0x004A  40GbaseLR4
        0x004B  100GbaseCR10
        0x004C  100GbaseSR10
        0x004D  100GbaseLR4
        0x004E  100GbaseER4
        IANA dependent 100BasePXFD
 */


#endif
