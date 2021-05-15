/*
*---------------------------------------------------------------------------*
*  Copyright (C) 2013 Woodhead Software & Electonics. All rights reserved.  *
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
* $Workfile:: ptcp.h                                                      $*
* $Revision:: 5                                                            $*
* $Date:: 17/10/16 9:54                                                    $*
*                                                                           *
*                                                                           */
/****************************************************************************/
/**
* Ptcp managment
*
*  
*****************************************************************************/
#ifndef _PTCPST_H
#define _PTCPST_H

#include "system.h"
#include "uuid_pn.h"

#define MAX_TXPORTGROUP 1
#define MAX_FRAME_DATA  2    /* represent the maximal number of RTC3 frame the PN node could send on each of its port*/

#define MAX_PHASE_ASSIGNMENT 16   /* 16 phase assignment is mandatory*/
#define MAX_NUMBER_ASSIGNMENT 5   /* At least 5 assignment are mandatory*/

typedef struct _ST_PDSYNCDATA{
  ST_CLR_RPC_UUID PTCPSubDomainUUID;
  APP_DWORD     dwReservedIntervalBegin;
  APP_DWORD     dwReservedIntervalEnd;
  APP_DWORD     dwPLLWindow;
  APP_DWORD     dwSyncSendFactor;
  APP_WORD       wSendClockFactor;
  APP_WORD       wPTCPTimeOutFactor;
  APP_WORD       wPTCPTakeOverTimeOutFactor;
  APP_WORD       wPTCPMasterStartupTime;
  APP_WORD       wSyncProperties;
  APP_BYTE       byPTCP_MasterPriority1;
  APP_BYTE       byPTCP_MasterPriority2;
  APP_BYTE       byPTVPLengthSubdomainName;
  APP_BYTE       sPTCPSubDomainName[240];
}ST_PDSYNCDATA, APP_FAR * LPST_PDSYNCDATA;

typedef struct _ST_PDIRGLOBALDATA{
  ST_CLR_RPC_UUID IRDataUUID;
  APP_DWORD dwMaxBridgeDelay;
  APP_DWORD dwNumberofPorts;
  APP_DWORD dwMaxTxPort[WSE_LLDP_MAX_PORT];
  APP_DWORD dwMaxRxPort[WSE_LLDP_MAX_PORT];
  APP_DWORD dwMaxLineRxDelay[WSE_LLDP_MAX_PORT];
  APP_DWORD dwYellowTime[WSE_LLDP_MAX_PORT];
}ST_PDIRGLOBALDATA, APP_FAR * LPST_PDIRGLOBALDATA;

typedef struct _ST_PDIRSUBFRAMEDATA{
  APP_DWORD dwFrameSendOffset;
  APP_WORD  wDataLenght;
  APP_WORD  wreductionRatio;
  APP_WORD  wPhase;
  APP_WORD  wFrameID;
  APP_WORD  wEthertype;
  APP_BYTE  byRxPort;
  APP_BYTE  byFrameDetails;
  APP_BYTE  byNumberOfTxPortGroups;
  APP_BYTE  byTxPortGroupProperties[MAX_TXPORTGROUP];
}ST_PDIRSUBFRAMEDATA, APP_FAR * LPST_PDIRSUBFRAMEDATA;

typedef struct _ST_PDIRFRAMEDATA
{
  APP_DWORD dwIRFrameDataProperties;
  ST_PDIRSUBFRAMEDATA  stPDIrSubFrameFata[MAX_FRAME_DATA];
}ST_PDIRFRAMEDATA, APP_FAR * LPST_PDIRFRAMEDATA;

typedef struct _ST_ASSIGNMENT
{
  APP_DWORD dwRedOrangeBegin[MAX_NUMBER_ASSIGNMENT*2];     /*two times because rx & tx are stored here*/
  APP_DWORD dwOrangeBegin[MAX_NUMBER_ASSIGNMENT*2];     /*two times because rx & tx are stored here*/
  APP_DWORD dwGreenBegin[MAX_NUMBER_ASSIGNMENT*2];     /*two times because rx & tx are stored here*/
  APP_DWORD dwNumberOfPhases;
  APP_WORD  wPhaseAssignmentTX[MAX_PHASE_ASSIGNMENT];
  APP_WORD  wPhaseAssignmentRX[MAX_PHASE_ASSIGNMENT];
}ST_ASSIGNMENT, APP_FAR * LPST_ASSIGNMENT;

typedef struct  _ST_PDIRBEGINENDDATA
{
  APP_WORD wStartOfRedFrameID;
  APP_WORD wEndOfRedFrameID;
  APP_DWORD dwNumberOfPorts;
  APP_DWORD dwNumberOfAssignment[WSE_LLDP_MAX_PORT];
  ST_ASSIGNMENT  st_Assignment[WSE_LLDP_MAX_PORT];
}ST_PDIRBEGINENDDATA, APP_FAR *LPST_PDIRBEGINENDDATA;

typedef struct ST_PDIRDATA{
  ST_PDIRGLOBALDATA      stPDIrGlobaldata;
  ST_PDIRFRAMEDATA       stPDIrFrameData;
  ST_PDIRBEGINENDDATA    stPDIrBeginData;
}ST_PDIRDATA, APP_FAR * LPST_PDIRDATA;
#endif
