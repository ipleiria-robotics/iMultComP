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
* $Workfile:: pncbdev.h                                                    $*
* $Revision:: 50                                                           $*
* $Date:: 18/10/16 16:51                                                   $*
*                                                                           *
*                                                                           */
/****************************************************************************/
/**
* @file pncbdev.h
* @brief Callback from stack to device user application header.
*
*****************************************************************************/

#ifndef _PNCBDEV_H
#define _PNCBDEV_H

#include "link.h"

/* Remanent Value */
#define PND_Is_Remanent             1
#define PND_Is_Not_Remanent         0

#define PND_AR_CHECK_SUBSTITUE      0
#define PND_AR_CHECK_REPLACE        1

#define PNIO_SSLOT_FREE             1
#define PNIO_SSLOT_CONNECTED        2
#define PNIO_SSLOT_IO_DATA          3

#define PNIO_DHCP_MODE_CLIENT_ID    0
#define PNIO_DHCP_MODE_MAC          1
#define PNIO_DHCP_MODE_NAME         2

#define PNIO_DHCP_CTRL_DISABLE      0
#define PNIO_DHCP_CTRL_RESET        1
#define PNIO_DHCP_CTRL_ENABLE       2

/****************************************************************************/
/**
* @struct ST_PNIO_RW_STATUS
* @brief Status structure
*
* This structure is returned by the @ref PNIO_Record_API "record read/write indication functions".
*
*
*****************************************************************************/
typedef struct
{
  APP_BYTE ErrorCode;   /**< @brief Error code value. */
  APP_BYTE ErrorDecode; /**< @brief Error decode value. */
  APP_BYTE ErrorCode1;  /**< @brief Error code1 value. */
  APP_BYTE ErrorCode2;  /**< @brief Error code2 value. */
} ST_PNIO_RW_STATUS, APP_FAR * LPST_PNIO_RW_STATUS;

/****************************************************************************/
/**
* @struct ST_DIAGNOSIS_COUNTER
* @brief Status structure
*
* This structure is returned by the @ref PNIO_Record_API "record read/write indication functions".
*
*****************************************************************************/
typedef struct
{
  APP_DWORD dwNbArOpen;         /**< @brief Number of time the callback PND_CB_ArOpenedInd has been callled by stack.                                                             */
  APP_DWORD dwNbArClose;        /**< @brief Number of time the callback PND_CB_ArClosedInd has been callled by stack.                                                             */
  APP_DWORD dwNbTimeWdMissed1;  /**< @brief Number of time the watchdog detect "one missing packet". This means one consumption packet has not been received                      */
  APP_DWORD dwNbTimeWdMissed2;  /**< @brief Number of time the watchdog detect "two missing packet". This means that two consecutive consumption packet has not been received     */
  APP_DWORD dwNbTimeWdMissed3;  /**< @brief Number of time the watchdog detect "three missing packet". This means that three consecutive consumption packet has not been received */
} ST_DIAGNOSIS_COUNTER, APP_FAR * LPST_DIAGNOSIS_COUNTER;

APP_WORD APP_FAR PND_CB_DcpDeviceBlinkInd             (APP_DWORD dwDRContextInd);
APP_WORD APP_FAR PND_CB_DcpFactoryResetInd            (APP_DWORD dwDRContextInd, APP_WORD wBlockQualifer);
APP_WORD APP_FAR PND_CB_DcpResetToFactoryInd          (APP_DWORD dwDRContextInd, APP_WORD wBlockQualifer);
APP_WORD APP_FAR PND_CB_DcpDeviceDHCPInd              (APP_DWORD dwDRContextInd, APP_BYTE byDHCPMode, APP_WORD wClienIDLen, APP_LPBYTE lpbyClientID, APP_WORD wBlockQualifier);
APP_WORD APP_FAR PND_CB_DcpDeviceDHCPCtrlInd          (APP_DWORD dwDRContextInd, APP_WORD wControlReq);
APP_WORD APP_FAR PND_CB_DcpDeviceNameInd              (APP_DWORD dwDRContextInd, APP_LPBYTE lpDeviceName, APP_WORD wSizeName, APP_WORD wBlockQualifier);
APP_WORD APP_FAR PND_CB_DcpDeviceIpInd                (APP_DWORD dwDRContextInd, APP_BYTE byType, APP_DWORD dwIp, APP_DWORD dwSubMask, APP_DWORD dwGateway, APP_WORD wBlockQualifier);
APP_WORD APP_FAR PND_CB_DcpDeviceIpFullInd            (APP_DWORD dwDRContextInd, APP_DWORD dwIp, APP_DWORD dwSubMask, APP_DWORD dwGateway, APP_DWORD dwDnsServer1, APP_DWORD dwDnsServer2, APP_DWORD dwDnsServer3, APP_DWORD dwDnsServer4, APP_WORD wBlockQualifier);
APP_WORD APP_FAR PND_CB_Start_Cnf_Pos                 (APP_DWORD dwDRContextInd);
APP_WORD APP_FAR PND_CB_Start_Cnf_Neg                 (APP_DWORD dwDRContextInd);
APP_WORD APP_FAR PND_CB_PlugSubModule_Cnf_Neg         (APP_DWORD dwIRContextInd, APP_LPVOID lpvContext, APP_DWORD  dwApi, APP_WORD   wSlot, APP_WORD   wSubSlot, APP_DWORD dwStatus);
APP_WORD APP_FAR PND_CB_PlugSubModule_Cnf_Pos         (APP_DWORD dwIRContextInd, APP_LPVOID lpvContext, APP_DWORD  dwApi, APP_WORD   wSlot, APP_WORD   wSubSlot);
APP_WORD APP_FAR PND_CB_PullSubModule_Cnf_Neg         (APP_DWORD dwIRContextInd, APP_LPVOID lpvContext, APP_DWORD  dwApi, APP_WORD   wSlot, APP_WORD   wSubSlot, APP_DWORD dwStatus);
APP_WORD APP_FAR PND_CB_PullSubModule_Cnf_Pos         (APP_DWORD dwIRContextInd, APP_LPVOID lpvContext, APP_DWORD  dwApi, APP_WORD   wSlot, APP_WORD   wSubSlot, LPST_DESC_LINK  APP_FAR * lpstArrayOfDescLink);
APP_WORD APP_FAR PND_CB_SubSlotStateChange_ind        (APP_DWORD dwARContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_WORD wNewState);
APP_WORD APP_FAR PND_CB_ArOpenedInd                   (APP_DWORD dwIRContextInd, APP_LPVOID lpstArep, APP_LPDWORD lpdwARContextInd ,APP_WORD wARType, APP_DWORD dwARProperties, APP_LPBYTE lpstARUUID, APP_LPBYTE lpstCMIOUUID, APP_LPBYTE lpbyCMI_Mac, APP_LPBYTE lpbyCMI_Name, APP_WORD wCMI_Name_Len, APP_DWORD dwIPAddr, APP_LPVOID lpstCrInput, APP_LPVOID lpstCrOutput);
APP_WORD APP_FAR PND_CB_ArCheckInd                    (APP_DWORD dwIRContextInd, APP_DWORD dwARContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_DWORD dwRealModuleID, APP_DWORD dwExpectedModuleID, APP_DWORD dwRealSubModuleID, APP_DWORD dwExpectedSubmoduleID, APP_WORD wRealInputLen, APP_WORD wExpectedInputLen, APP_WORD wRealOutputLen, APP_WORD wExpectedOutputLen, APP_LPBYTE lpbyAcceptedMode);
APP_WORD APP_FAR PND_CB_ArClosedInd                   (APP_DWORD dwARContextInd);
APP_WORD APP_FAR PND_CB_ApplicationReady_Cnf_Pos      (APP_DWORD dwARContextInd);
APP_WORD APP_FAR PND_CB_ApplicationReady_Cnf_Neg      (APP_DWORD dwARContextInd, APP_BYTE  byErrCode, APP_BYTE  byErrDecode, APP_BYTE  byErrCode1, APP_BYTE  byErrCode2);
APP_WORD APP_FAR PND_CB_ReadyForRTC3_Cnf_Pos          (APP_DWORD dwARContextInd);
APP_WORD APP_FAR PND_CB_ReadyForRTC3_Cnf_Neg          (APP_DWORD dwARContextInd, APP_BYTE  byErrCode, APP_BYTE  byErrDecode, APP_BYTE  byErrCode1, APP_BYTE  byErrCode2);
APP_WORD APP_FAR PND_CB_ParamEndInd                   (APP_DWORD dwARContextInd);
APP_WORD APP_FAR PND_CB_ApplyConfigurationInd         (APP_DWORD dwDRContextInd);
APP_WORD APP_FAR PND_CB_ReleaseInd                    (APP_DWORD dwARContextInd);
APP_WORD APP_FAR PND_CB_Alarm_Ack                     (APP_DWORD dwARContextInd, APP_WORD wAlarmType, APP_LPVOID lpContext, APP_DWORD dwStatus);
APP_WORD APP_FAR PND_CB_WriteInd                      (APP_DWORD dwARContextInd, APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wIndex, APP_WORD wSlot, APP_WORD wSubSlot, APP_DWORD  dwSizeOfData, APP_LPBYTE lpbyData, LPST_PNIO_RW_STATUS lpStatus, APP_LPWORD lpwAddData1, APP_LPWORD lpwAddData2, APP_LPVOID lpvArepUsed);
APP_WORD APP_FAR PND_CB_ReadInd                       (APP_DWORD dwARContextInd, APP_DWORD dwDRContextInd, APP_DWORD dwApi, APP_WORD wIndex, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPDWORD lpdwSizeOfData, APP_LPBYTE lpbyData, LPST_PNIO_RW_STATUS lpstStatus, APP_LPBYTE lpbyAddData1, APP_LPBYTE lpbyAddData2, APP_LPVOID lpvArepUsed);
APP_WORD APP_FAR PND_CB_Set_Output_Cnf_Pos            (APP_DWORD dwARContextInd, APP_LPVOID lpContext);
APP_WORD APP_FAR PND_CB_Set_Output_Cnf_Neg            (APP_DWORD dwARContextInd, APP_LPVOID lpContext, APP_DWORD dwStatus);
APP_WORD APP_FAR PND_CB_Get_Input_Cnf_Pos             (APP_DWORD dwARContextInd, APP_LPVOID lpContext, APP_LPBYTE lpData, APP_WORD wDataLen, APP_BYTE byIops);
APP_WORD APP_FAR PND_CB_Get_Input_Cnf_Neg             (APP_DWORD dwARContextInd, APP_LPVOID lpContext, APP_DWORD dwStatus);
APP_WORD APP_FAR PND_CB_Set_Output_Apdu_Cnf_Neg       (APP_DWORD dwARContextInd, APP_LPVOID lpContext, APP_DWORD dwStatus);
APP_WORD APP_FAR PND_CB_Set_Output_Apdu_Cnf_Pos       (APP_DWORD dwARContextInd, APP_LPVOID lpContext);
APP_WORD APP_FAR PND_CB_Get_Output_Iocs_Cnf_Pos       (APP_DWORD dwARContextInd, APP_LPVOID lpContext, APP_BYTE byIOCS);
APP_WORD APP_FAR PND_CB_Get_Output_Iocs_Cnf_Neg       (APP_DWORD dwARContextInd, APP_LPVOID lpContext, APP_DWORD dwStatus);
APP_WORD APP_FAR PND_CB_Ctrl_Alarm_Ind                (APP_DWORD dwARContextInd, APP_WORD wAlarmType, APP_DWORD wApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_DWORD dwModuleIdentNumber, APP_DWORD dwSubModuleIdentNumber, APP_WORD wDataLength, APP_LPBYTE lpbydata);
APP_WORD APP_FAR PND_CB_Get_Input_Apdu_Cnf_Neg        (APP_DWORD dwARContextInd, APP_LPVOID lpContext, APP_DWORD dwStatus);
APP_WORD APP_FAR PND_CB_Get_Input_Apdu_Cnf_Pos        (APP_DWORD dwARContextInd, APP_LPVOID lpContext, APP_BYTE byApduStatus);
APP_WORD APP_FAR PND_CB_Set_Input_Iocs_Cnf_Pos        (APP_DWORD dwARContextInd, APP_LPVOID lpContext);
APP_WORD APP_FAR PND_CB_Set_Input_Iocs_Cnf_Neg        (APP_DWORD dwARContextInd, APP_LPVOID lpContext, APP_DWORD dwStatus);
APP_WORD APP_FAR PND_CB_PlugParamEndInd               (APP_DWORD dwARContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot);
APP_WORD APP_FAR PND_CB_PlugApplicationReady_Cnf_Pos  (APP_DWORD dwARContextInd);
APP_WORD APP_FAR PND_CB_PlugApplicationReady_Cnf_Neg  (APP_DWORD dwARContextInd);
APP_WORD APP_FAR PND_CB_GetDiagnosisCounter_Cnf_Pos   (ST_DIAGNOSIS_COUNTER stDiagCounter);
APP_WORD APP_FAR PND_CB_ResetDiagnosisCounter_Cnf_Pos (APP_VOID);
APP_WORD APP_FAR PND_CB_SetParam_Cnf_Pos              ();
APP_WORD APP_FAR PND_CB_AddDiagnosisCnf_Neg           (APP_DWORD dwIRContextInd, APP_LPVOID lpContext, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_WORD  wChannelNumber, APP_DWORD dwDiagnosticID, APP_DWORD dwStatus);
APP_WORD APP_FAR PND_CB_AddDiagnosisCnf_Pos           (APP_DWORD dwIRContextInd, APP_LPVOID lpContext, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_WORD  wChannelNumber, APP_DWORD dwDiagnosticID, APP_BYTE byFireAlarm, APP_LPVOID * lplpAlarmContext);
APP_WORD APP_FAR PND_CB_RemoveDiagnosisCnf_Neg        (APP_DWORD dwIRContextInd, APP_LPVOID lpContext, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_WORD  wChannelNumber, APP_DWORD dwDiagnosticID, APP_DWORD dwStatus);
APP_WORD APP_FAR PND_CB_RemoveDiagnosisCnf_Pos        (APP_DWORD dwIRContextInd, APP_LPVOID lpContext, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_WORD  wChannelNumber, APP_DWORD dwDiagnosticID, APP_BYTE byFireAlarm, APP_LPVOID * lplpAlarmContext);

#ifdef NEW_INPUT_DATA_MODE
APP_WORD APP_FAR PND_CB_NewInputInd                   (APP_DWORD dwARContextInd, APP_DWORD dwDRContextInd);
APP_WORD APP_FAR PND_CB_NewInputAPDUDataStatus        (APP_DWORD dwARContextInd, APP_DWORD dwDRContextInd);
#endif

#ifdef SHARED_MEMORY_MODE
APP_WORD APP_FAR PND_CB_ArOpen_SharedMemoriesDataReq  (APP_DWORD dwARContextInd, APP_WORD wIOCRType, APP_WORD wDataLength, APP_WORD wFrameId, APP_LPDWORD lpdwOffsetStatus , APP_LPDWORD lpdwOffsetOverRun, APP_LPDWORD lpdwOffsetProtect);
APP_WORD APP_FAR PND_CB_SlotReplace_SharedMemoriesItem(APP_DWORD dwIRContextInd, APP_DWORD dwARContextInd, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_DWORD dwModuleID, APP_DWORD dwSubModuleID, APP_WORD wInputLen, APP_WORD wOutputLen,  LPST_DESC_LINK APP_FAR APP_FAR * arrayOflpstOldItemList, LPST_DESC_LINK APP_FAR * arrayOflpstNewItemList);
#endif

#ifdef PN_SR_SUPPORTED
APP_WORD APP_FAR PND_CB_SRArSetUpdated                (APP_BYTE byEventType, APP_DWORD dwARContextIndUpdated, APP_DWORD dwARContextInd,  APP_DWORD dwSrProperties, APP_WORD wRDHTUpdated, APP_WORD wRDHT);
APP_WORD APP_FAR PND_CB_SRArSetSwitchoverInd          (APP_BYTE byEventType, APP_DWORD dwARContextIndUpdated, APP_DWORD dwARContextInd);
APP_WORD APP_FAR PND_CB_SRArSetMTOTExpire             (APP_DWORD dwARContextInd, APP_DWORD dwARContextInd2);

#define SR_NEWSRAR  0
#define SR_SRARLOST 1

#define SR_PRIMARY_SWITCHOVER 0
#define SR_BACKUP_SWITCHOVER  1

#endif


#ifdef PROFIENERGY_INTERFACE
APP_WORD PE_GotoEnergySaving (APP_BYTE bySavingMode);
APP_WORD PE_ExitEnergySaving (APP_BYTE bySavingMode);
#endif

#ifdef PN_PROD_HYBRID

#define PND_AR_HYBRID_MODE_CPU      0
#define PND_AR_HYBRID_MODE_HWACC    1

APP_WORD APP_FAR PND_CB_ArGetProductionModeInd (APP_DWORD dwARContextInd, APP_WORD wARType, APP_DWORD dwARProperties, APP_LPBYTE lpbyXpmMode);

#endif

/* Values for ResetToFactory BlockQualifier */
#define BQ_RESETTOFACTORY_APPLICATION_DATA          1
#define BQ_RESETTOFACTORY_COMMUNICATION_PARAM       2
#define BQ_RESETTOFACTORY_ENGINEERING_PARAM         3
#define BQ_RESETTOFACTORY_ALL_STORED_DATA           4
#define BQ_RESETTOFACTORY_ALL_STORED_TO_FACTORY     8
#define BQ_RESETTOFACTORY_RESET_AND_RESTORE         9

#endif
