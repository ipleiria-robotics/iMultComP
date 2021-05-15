/*---------------------------------------------------------------------------*/
/*  Copyright (C) 2007 Woodhead Software & Electonics. All rights reserved.  */
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
/*  Project             : STACK PROFINET Device                              */
/*  Component           :                                                    */
/* $Workfile:: stackdev.h                                                   $*/
/* $Revision:: 56                                                           $*/
/* $Date:: 15/03/16 10:58                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef __STACKDEV_H__
#define __STACKDEV_H__


#include <Profinet/include/RT/platform_pn.h>
#include "link.h"

#define UNUSED_ARG(p) p=p

#define PN_OUTPUT_DATA  1
#define PN_INPUT_DATA   2

#define RPC_EPM_DEVICE_TYPE_MAX_LEN 25
#define RPC_EPM_ORDER_ID_MAX_LEN    20
#define RPC_EPM_HWREV_MAX_LEN       5
#define RPC_EPM_SWREV_MAX_LEN       9
#define RPC_EPM_SERIALNUM_MAX_LEN   16

typedef struct _ST_PARAM_DEVICE
{
  /*Im0_Data*/
  /* insert +1 for null caracter if printf, sprintf is used */
  APP_BYTE byOrderId[RPC_EPM_ORDER_ID_MAX_LEN+1];
  APP_BYTE byOrderIdLen;

  APP_BYTE bySerialNumber[RPC_EPM_SERIALNUM_MAX_LEN+1];
  APP_BYTE bySerialNumberLen;

  APP_BYTE byHWRevision[RPC_EPM_HWREV_MAX_LEN+1];
  APP_BYTE byHWRevisionLen;

  APP_WORD wHardwareRevision;

  APP_BYTE bySWRevision[RPC_EPM_SWREV_MAX_LEN+1];
  APP_BYTE bySWRevisionLen;

  APP_BYTE byDeviceType[RPC_EPM_DEVICE_TYPE_MAX_LEN+1];
  APP_BYTE byDeviceTypeLen;

  APP_BYTE bySWFunctionalEnhancement;
  APP_BYTE bySWBugFix;
  APP_BYTE bySWInternalChange;
  APP_WORD wSWCounter;
  APP_BYTE bySWRevisionPrefix;

}ST_PARAM, APP_FAR * LPST_ST_PARAM;

typedef struct{

  APP_LPVOID lpstIOXSMngtIn;
  APP_LPVOID lpstIOXSMngtOut;

  APP_LPBYTE lpbyInputIOPS;
  APP_LPBYTE lpbyInputIOCS;

  APP_BYTE   byInputIOPS;
  APP_BYTE   byInputIOCS;

  APP_DWORD  dwOffsetInAdvcIOPS;
  APP_DWORD  dwOffsetInAdvcIOCS;
  APP_DWORD  dwOffsetInAdvcNew;
  APP_DWORD  dwOffsetInData;

} ST_SS_SMINFO, APP_FAR *LPST_SS_SMINFO;

/****************************************************************************/
/**
* @struct ST_PND_SUB_SLOT_CONF
* @brief Subslot configuration structure.
*
* This structure is used to store subslot configuration.
*
*
*****************************************************************************/
typedef struct{
  APP_DWORD     dwApi;         /**< @brief Module/submodule Localization.                     */
  APP_WORD      wSlot;         /**< @brief Module/submodule Localization.                     */
  APP_WORD      wSubSlot;      /**< @brief Module/submodule Localization.                     */
  APP_DWORD     dwModuleId;    /**< @brief Module identification number.                      */
  APP_DWORD     dwSubModuleId; /**< @brief Submodule identification number.                   */
  APP_WORD      wSizeInput;    /**< @brief Size of input.                                     */
  APP_WORD      wSizeOutput;   /**< @brief Size of output.                                    */
  APP_BYTE      byIO;          /**< @brief Module type (PN_INPUT_DATA and/or PN_OUTPUT_DATA). */

#ifdef SHARED_MEMORY_MODE
  ST_DESC_LINK  stDescLinkConfArray[MAX_ARD_PER_DEVICE*MAX_INSTANCE_PER_DEVICE]; /**< @brief List of items to be attached to the submodule.
                                                                                    * Only used by stack if @ref SHARED_MEMORY_MODE is defined in other case this field is ignored.
                                                                                    * See @ref PND_SM_AddInputItem and @ref PND_SM_AddOutputItem function for more details on items. */
#endif

}ST_PND_SUB_SLOT_CONF,APP_FAR *LPST_PND_SUB_SLOT_CONF;

#define PN_USI_CHANNEL_DIAG_DATA           0x8000
#define PN_USI_MANUFACTURER_DIAG_DATA      0x8001
#define PN_USI_EXT_CHANNEL_DIAG_DATA       0x8002
#define PN_USI_QUALIFIED_CHANNEL_DIAG_DATA 0x8003

#define PN_CHANPROP_FAULT                  0x00
#define PN_CHANPROP_MAINTENANCE_REQUIRED   0x01
#define PN_CHANPROP_MAINTENANCE_DEMANDED   0x02
#define PN_CHANPROP_MAINTENANCE_QUALIFIED  0x03

#define PN_CHANPROP_APPEAR                 0x01
#define PN_CHANPROP_DIAPPEAR               0x02

#define PN_CHANPROP_DIR_MANUFACTURER       0x00
#define PN_CHANPROP_DIR_INPUT              0x01
#define PN_CHANPROP_DIR_OUTPUT             0x02
#define PN_CHANPROP_DIR_INOUT              0x03

typedef struct _ST_DIAG_DATA
{
  APP_WORD wUserStructureIdentifier;         /* This values define the level of current diagnostic registered. Allowed values are:
                                                 0x8000 = ChannelDiagnosisData   (recommanded) : Only field ChannelNumber, ChannelProperties (_XXXX will be used by stack) and ChannelErrorType.
                                                 0x8002 = ExtChannelDiagnosisData              : In addition of field from ChannelDiagnosisData, the ExtChannelErrorType and ExtChannelAddValue field will be use by stack.
                                                 0x8003 = QualifiedChannelDiagnosisData        : In addition of field from ExtChannelDiagnosisData, the QualifiedChannelQualifier field will be use by stack.
                                             */
  APP_BYTE  byChannelProperties_Type;        /* 0x00 = Shall be used if the field ChannelNumber contains the value 0x8000, or if other values are not appropriate
                                                0x01 = 1 Bit: The data length of this channel is 1 Bit
                                                0x02 = 2 Bit: The data length of this channel is 2 Bit
                                                (... see specification )
                                                0x07 = 64 Bit: The data length of this channel is 64 Bit
                                             */
  APP_BYTE  byChannelProperties_Maintenance; /* 0x00 : Fault
                                                0x01 : Maintenance required
                                                0x02 : Maintenance demanded
                                                0x03 : QualifiedChannelQualifier (dwQualifiedChannelQualifier) is used to determine error level. Shall only be used with wUserStructureIdentifier set to 0x8003
                                             */
  APP_BYTE  byChannelProperties_Direction;   /* 0x00 : Manufacturer specific
                                                0x01 : Input Channel
                                                0x02 : Output Channel
                                                0x03 : Intput/Output Channel
                                             */
  APP_BYTE  byChannelProperties_Accumulative;/* 0x01 : Accumulative diagnostis from more than one channel
                                                0x00 : Default value
                                             */
  APP_WORD  wChannelErrorType;               /* See specification "Coding of the field ChannelErrorType" for all possible values. Below some exemple:
                                                0x0001 : Short circuit
                                                0x0002 : Undervoltage
                                                0x0003 : Overvoltage
                                                0x0003 : Overload
                                                (...)
                                                0x0010 : Default for "parameter missing"
                                                0x0011 : Default for "power supply fault"
                                                (...)
                                             */
  APP_WORD  wExtChannelErrorType;            /* Thie values depends on wChannelErrorType. See specificationfor all possible values.
                                                Only used if wUserStructureIdentifier is set to 0x8002 or 0x8003
                                             */
  APP_DWORD dwExtChannelAddValue;            /* Thie values depends on wExtChannelErrorType. See specificationfor all possible values.
                                                Only used if wUserStructureIdentifier is set to 0x8002 or 0x8003
                                             */
  APP_DWORD dwQualifiedChannelQualifier;     /* Thie values depends define a level of error. It shall be set to 0 if byChannelProperties_Maintenance is not set to
                                                0x03. See specificationfor all possible values.Only used if wUserStructureIdentifier is set to 0x8003
                                             */
}ST_DIAG_DATA, APP_FAR * LPST_DIAG_DATA;


/************************ define ************************/
/*data APDU status flag values*/
#define APDU_DATA_FLAG_VALUE           0x04
#define APDU_AR_STATE_FLAG_VALUE       0x01
#define APDU_PROVIDER_DATA_FLAG_VALUE  0x10
#define APDU_PROBLEME_IND_FLAG_VALUE   0x20

/* APDU set/not set flag */
#define APDU_DATA_VALIDE                 1
#define APDU_AR_STATE_PRIMARY            1
#define APDU_PROVIDER_RUN                1
#define APDU_NO_PROBLEME                 1

#define APDU_DATA_INVALID                0
#define APDU_AR_STATE_BACKUP             0
#define APDU_PROVIDER_STOP               0
#define APDU_HAVE_PROBLEME               0

/********* IP type: provided by DHCP or static **/
#define PND_IP_STATIC                    0x00
#define PND_IP_DHCP                      0x80
#define PND_IP_DHCP_MAC                  0x81
#define PND_IP_DHCP_NAME                 0x82
#define PND_IP_DHCP_ID                   0x83

/********* Initiative delay accepted            **/
#define PND_INITIAVE_DELAY_MODE_OFF       0x00       /* IO-Device does not issue a DCP-Hello-ReqPDU                                */
#define PND_INITIAVE_DELAY_MODE_ON        0x01       /* IO-Device does issue a DCP-Hello-ReqPDU                                    */
#define PND_INITIAVE_DELAY_MODE_ON_DELAY  0x02       /* IO-Device does issue a DCP-Hello-ReqPDU after LinkUP but after a delay.    */

#define PND_HELLO_INTERVAL_0030MS           30
#define PND_HELLO_INTERVAL_0050MS           50
#define PND_HELLO_INTERVAL_0100MS          100
#define PND_HELLO_INTERVAL_0300MS          300
#define PND_HELLO_INTERVAL_0500MS          500
#define PND_HELLO_INTERVAL_1000MS         1000

#define PND_HELLO_DELAY_OFF                 0
#define PND_HELLO_DELAY_0050MS              50
#define PND_HELLO_DELAY_0100MS             100
#define PND_HELLO_DELAY_0500MS             500
#define PND_HELLO_DELAY_1000MS            1000

#define PND_HELLO_MAX_RETRY_ALLOWED       0x0F      /*Due to specification no more than0x0F retry are allowed*/

/********************* Function prototype *****************/
APP_WORD APP_FAR PND_Init                         (APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_Exit                         (APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_Start                        (APP_LPVOID lpPhyDev, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_Stop                         (APP_LPVOID lpPhyDev, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_AddDevice                    (APP_LPVOID APP_FAR *lpPhyDev, APP_DWORD dwDRContextInd, APP_LPCHAR lpszInterfaceName, APP_WORD wPort, APP_LPCHAR lpbyDeviceName, APP_LPCHAR lpbyDeviceType, APP_WORD wDeviceID, APP_WORD wVendorID, APP_BYTE byTypeIp, APP_DWORD dwIpAddress, APP_DWORD dwSubNetMask, APP_DWORD dwGateway, APP_WORD wReserved1, APP_WORD wReserved2, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_AddDeviceExt                 (APP_LPVOID APP_FAR *lpPhyDev, APP_DWORD dwDRContextInd, APP_LPCHAR lpszInterfaceName, APP_WORD wPort, APP_LPCHAR lpbyDeviceName, APP_LPCHAR lpbyDeviceType, APP_WORD wDeviceID, APP_WORD wVendorID, APP_BYTE byTypeIp, APP_DWORD dwIpAddress, APP_DWORD dwSubNetMask, APP_DWORD dwGateway, LPST_ST_PARAM lpstParam, APP_WORD wReserved1, APP_WORD wReserved2, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_DelDevice                    (APP_LPVOID lpPhyDev, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_SetParam                     (APP_LPVOID lpstDrep, ST_PARAM stParam, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_AddInstance                  (APP_LPVOID lpPhyDev, APP_LPVOID APP_FAR *lppLogDev, APP_DWORD dwIRContextInd, APP_WORD wInstance, APP_DWORD dwNbSSElement, LPST_PND_SUB_SLOT_CONF arrayOfSubSlot, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_PlugSubModule                (APP_LPVOID lpLogDev, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_DWORD  dwModuleId, APP_DWORD  dwSubModuleId, APP_WORD wSizeIn, APP_WORD wSizeOut, PST_DESC_LINK  pstDescLink, APP_LPVOID lpvContext, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_PullSubModule                (APP_LPVOID lpLogDev, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPVOID lpvContext, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_SetSubModuleState            (APP_LPVOID lpLogDev, APP_DWORD dwApi ,APP_WORD wSlot, APP_WORD wSubSlot, APP_DWORD dwState, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_GetSubModuleState            (APP_LPVOID lpLogDev, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPDWORD lpdwState, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_Alarm_Process                (APP_LPVOID lpLogDev, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyAlarmData, APP_WORD wAlarmDataLen, APP_WORD wUserStructureCode, APP_LPVOID lpContext,APP_WORD wReserved1, APP_WORD wReserved2, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_Alarm_Diagnosis              (APP_LPVOID lpLogDev, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPBYTE lpbyAlarmData, APP_WORD wAlarmDataLen, APP_WORD wUserStructureCode, APP_LPVOID lpContext,APP_WORD wReserved1, APP_WORD wReserved2, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_Alarm                        (APP_LPVOID lpLogDev, APP_DWORD dwApi, APP_BYTE byPriority, APP_WORD wAlarmType, APP_WORD wSlot, APP_WORD wSubSlot, APP_WORD wAlarmSpecifier, APP_LPBYTE lpbyAlarmData, APP_WORD wAlarmDataLen, APP_WORD wUserStructureCode, APP_LPVOID lpContext, APP_WORD wReserved1, APP_LPWORD lpwStatus);

APP_WORD APP_FAR PND_ApplicationReady             (APP_LPVOID lpstArep, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_PlugApplicationReady         (APP_LPVOID lpstArep, APP_DWORD dwApi ,APP_WORD wSlot, APP_WORD wSubSlot, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_ArAbort                      (APP_LPVOID lpstArep, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_Set_Output_Apdu_Status       (APP_LPVOID lpstArep, APP_LPVOID lpContext, APP_BYTE byData_Flag, APP_BYTE byAR_State_Flag, APP_BYTE byProvider_State_Flag, APP_BYTE byProblem_Indicator_Flag, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_Get_Input_Apdu_Status        (APP_LPVOID lpstArep, APP_LPVOID lpContext, APP_LPWORD lpwStatus);

APP_WORD APP_FAR PND_Set_Output                   (APP_LPVOID lpstDrep, APP_LPVOID lpContext, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIOPS, APP_LPVOID lpvData, APP_WORD wDataSize, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_Set_Input_Iocs               (APP_LPVOID lpstDrep, APP_LPVOID lpContext, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byIOCS, APP_WORD wIOCSLength, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_Get_Output_Iocs              (APP_LPVOID lpstDrep, APP_LPVOID lpContext, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_Get_Input                    (APP_LPVOID lpstDrep, APP_LPVOID lpContext, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_DHCP_Offer_Req               (APP_LPVOID lpstDrep, APP_DWORD dwIp, APP_DWORD dwSubNetMask, APP_DWORD dwGateway, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_DCP_Hello_Req                (APP_LPVOID lpstDrep, APP_WORD wPort, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_Configure_Dcp_Hello          (APP_LPVOID lpstDrep, APP_BYTE byInitiaveValues, APP_WORD wHelloStartUpDelay, APP_WORD wHelloInterval, APP_WORD wRetriesCount, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_RegisterAlias_Req            (APP_LPVOID lpstDrep, APP_LPBYTE lpbyAliasName, APP_BYTE byPortNumber, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_RemoveAlias_Req              (APP_LPVOID lpstDrep, APP_LPBYTE lpbyAliasName, APP_BYTE byPortNumber, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_UpdateAlias_Req              (APP_LPVOID lpstDrep, APP_LPBYTE lpbyNewAliasName, APP_LPBYTE lpbyOldAliasName, APP_BYTE byPortNumber, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_InformLinkOrMAUTypeChange    (APP_LPVOID lpstDrep, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_WORD wMAUType, APP_BYTE byLinkState, APP_BYTE byPortState, APP_LPVOID lpvContext, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_InformPeerMAUTypeChange      (APP_LPVOID lpstDrep, APP_LPBYTE lpbyMacAddr, APP_BYTE byPortNumber, APP_WORD wMAUTyp, APP_LPVOID lpvContexte, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_RemovePeerDataChange         (APP_LPVOID lpstDrep, APP_LPBYTE lpbyMacAddr, APP_BYTE byPortNumber, APP_LPVOID lpvContext, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_AddDiagnostic                (APP_LPVOID lpstIrep, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_WORD  wChannelNumber, APP_DWORD dwDiagnosticID, LPST_DIAG_DATA lpstDiagData, APP_LPVOID lpContext, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_RemoveDiagnostic             (APP_LPVOID lpstIrep, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_WORD  wChannelNumber, APP_DWORD dwDiagnosticID, APP_LPVOID lpContext, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_InformPeerMrpPortStatusChange (APP_LPVOID lpstDrep, APP_LPBYTE lpbyMacAddr, APP_BYTE byPortNumber, APP_LPBYTE byPnMrpUUID, APP_WORD wPnMrrtPortStatus, APP_LPVOID lpvContext, APP_LPWORD lpwStatus); // P+F change

#ifdef PN_RTC3_SUPPORTED
APP_WORD APP_FAR PND_InformPTCPSyncStatus         (APP_LPVOID lpstDrep, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE bySyncStatus, APP_LPBYTE byPTCPMaster, APP_LPVOID lpvContext, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_InformLineDelayChange        (APP_LPVOID lpstDrep, APP_DWORD dwApi, APP_WORD wSlot, APP_WORD wSubSlot, APP_BYTE byLineDelayFormatIndicator, APP_DWORD dwLineDelayValue, APP_LPVOID lpvContext, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_InformPeerMeasuredDelayChange(APP_LPVOID lpstDrep, APP_LPBYTE lpbyMacAddr, APP_BYTE byPortNumber, APP_DWORD dwPeerRxDelayLocal, APP_DWORD dwPeerRxDelayRemote, APP_DWORD dwPeerTxDelayLocal, APP_DWORD dwPeerTxDelayRemote, APP_DWORD dwPeerCableDelay, APP_LPVOID lpvContext, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_InformPeerPTCPChange         (APP_LPVOID lpstDrep, APP_LPBYTE lpbyMacAddr, APP_BYTE byPortNumber, APP_LPBYTE lpbyMasterSourceMacAddress, APP_LPBYTE lpbySubdomainUuid, APP_LPBYTE lpbyIrDataUuid, APP_LPVOID lpvContext, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_InformPeerPortStatusChange   (APP_LPVOID lpstDrep, APP_LPBYTE lpbyMacAddr, APP_BYTE byPort, APP_LPWORD lpwPNPortStatusRt2, APP_LPWORD lpwPNPortStatusRt3, APP_LPVOID lpvContext, APP_LPWORD lpwStatus);
#endif

APP_WORD APP_FAR PND_GetDiagnosisCounter_Req      (APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_ResetDiagnosisCounter_Req    (APP_LPWORD lpwStatus);
APP_VOID APP_FAR PND_Init_Alloc_Debug_Elt  ();

APP_WORD APP_FAR PND_ReadResponsPos               (APP_LPVOID lpstArep, APP_DWORD dwSize, APP_LPBYTE lpbyData, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_ReadResponsNeg               (APP_LPVOID lpstArep, APP_BYTE byErrorCode1, APP_BYTE byErrorCode2, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_WriteResponsePos             (APP_LPVOID lpstArep, APP_LPWORD lpwStatus);
APP_WORD APP_FAR PND_WriteResponseNeg             (APP_LPVOID lpstArep, APP_BYTE byErrorCode1, APP_BYTE byErrorCode2, APP_LPWORD lpwStatus);

#define PND_IOCS_OK          0x80
#define PND_IOCS_NOT_OK      0x00

#define PND_IOPS_OK          0x80
#define PND_IOPS_NOT_OK      0x00

#define IOXS_GOOD            0x80
#define IOXS_BAD_CONTROLLER  0x60
#define IOXS_BAD_DEVICE      0x40

#endif/*End of stackdev_H */
