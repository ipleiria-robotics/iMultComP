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
/* $Workfile:: dua.h                                                        $*/
/* $Revision:: 2                                                            $*/
/* $Date:: 19/10/16 11:51                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* D e s c r i p t i o n :                                                   */
/* dua Managment                                                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#ifndef _DUA_H
#define _DUA_H

#include "pnio_pdu.h"
#include "extcbdev.h"
/************************ default value for stack device initialization ************************/

/* profinet device configuration */
#define PND_NAME_FROM_CONFIG              "molex-devti"
#define PND_NAME_SIZE_FROM_CONFIG         11

/* network configuration */
#define PND_ITF_IP_ADRESSE                "128.127.60.111"
#define PND_INTERFACE_SIZE                256

/*device  configuration */
#ifndef PN_RTC3_SUPPORTED
#ifdef AM335X_FAMILY_BUILD
#define PND_DEVICE_ID                     0x301 /* For GSDML 0x310 : RTC3(AM335X)  0x301 : RTC2*/
#elif defined (AM437X_FAMILY_BUILD)
#define PND_DEVICE_ID                     0x303 /* For GSDML 0x313 : RTC3(AM437X)  0x303 : RTC2*/
#elif defined (AM57XX_FAMILY_BUILD)
#define PND_DEVICE_ID                     0x305 /* For GSDML 0x315 : RTC3(AM57XX)  0x305 : RTC2*/
#elif defined (SOC_K2G)
#define PND_DEVICE_ID                     0x305 /* For GSDML 0x315 : RTC3(AM57XX)  0x305 : RTC2*/
#endif
#else
#ifdef AM335X_FAMILY_BUILD
#define PND_DEVICE_ID                     0x310 /* For GSDML 0x310 : RTC3(AM335X)  0x301 : RTC2*/
#elif defined (AM437X_FAMILY_BUILD)
#define PND_DEVICE_ID                     0x313 /* For GSDML 0x313 : RTC3(AM437X)  0x303 : RTC2*/
#elif defined (AM57XX_FAMILY_BUILD)
#define PND_DEVICE_ID                     0x315 /* For GSDML 0x315 : RTC3(AM57XX)  0x305 : RTC2*/
#elif defined (SOC_K2G)
#define PND_DEVICE_ID                     0x315 /* For GSDML 0x315 : RTC3(AM57XX)  0x305 : RTC2*/
#endif
#endif
#define PND_VENDOR_ID                     0x127
#define PND_INSTANCE_ID                   0x1
#define PND_DEVICE_TYPE_FROM_CONFIG       "BRAD-COMM"
#define PND_DEVICE_TYPE_SIZE_FROM_CONFIG  9

#define PND_DUA_VERSION_MAJOR             0x01
#define PND_DUA_VERSION_MINOR             0x00
#define PND_DUA_VERSION_INTERNAL          0x03
#define PND_DUA_VERSION_BUGFIX            0x00

#define PN_NB_ITEM_SUPPORTED              (PN_CONFIGURATION_SIZE*6)

#define MIB_STRING_LEN                    256

/************************ structure ************************/
#include "os_pck.h"

#define IM_Tag_Function_LEN 32
#define IM_Tag_Location_LEN 22
/*
Declaration of IM1 structure is as described in Profinet IO specification V2.1
Visible string elements shall be filled with blanks if shorter than defined length
*/
struct _ST_IM1{
  APP_WORD wBlocType;
  APP_WORD wBlocLen;
  APP_BYTE byVersionHigh;
  APP_BYTE byVersionLow;
  APP_BYTE IM_Tag_Function[IM_Tag_Function_LEN];
  APP_BYTE IM_Tag_Location[IM_Tag_Location_LEN];
}STRUCT_ALIGNEMENT;
typedef struct _ST_IM1 PACK_ALIGNEMENT(ST_IM1), APP_FAR * LPST_IM1;


#define IM_Date_LEN 16
/*
Declaration of IM2 structure is as described in Profinet IO specification V2.1
Visible string elements shall be filled with blanks if shorter than defined length
*/
struct _ST_IM2{
  APP_WORD wBlocType;
  APP_WORD wBlocLen;
  APP_BYTE byVersionHigh;
  APP_BYTE byVersionLow;
  APP_BYTE IM_Date[IM_Date_LEN];
}STRUCT_ALIGNEMENT;
typedef struct _ST_IM2 PACK_ALIGNEMENT(ST_IM2), APP_FAR * LPST_IM2;


#define IM_Descriptor_LEN 54
/*
Declaration of IM3 structure is as described in Profinet IO specification V2.1
Visible string elements shall be filled with blanks if shorter than defined length
*/
struct _ST_IM3{
  APP_WORD wBlocType;
  APP_WORD wBlocLen;
  APP_BYTE byVersionHigh;
  APP_BYTE byVersionLow;
  APP_BYTE IM_Descriptor[IM_Descriptor_LEN];
}STRUCT_ALIGNEMENT;
typedef struct _ST_IM3 PACK_ALIGNEMENT(ST_IM3), APP_FAR * LPST_IM3;

#define IM_Signature_LEN 54
/*
Declaration of IM4 structure is as described in Profinet IO specification V2.1
Visible string elements shall be filled with blanks if shorter than defined length
*/
struct _ST_IM4{
  APP_WORD wBlocType;
  APP_WORD wBlocLen;
  APP_BYTE byVersionHigh;
  APP_BYTE byVersionLow;
  APP_BYTE IM_Signature[IM_Signature_LEN];
}STRUCT_ALIGNEMENT;
typedef struct _ST_IM4 PACK_ALIGNEMENT(ST_IM4), APP_FAR * LPST_IM4;

#include "os_unpck.h"

typedef struct _st_pnd_ar_information{

  APP_LPVOID lpstArep;
  APP_DWORD  dwDUAState;
  APP_BYTE   byNumArep;
  APP_BYTE   byContexte;
  APP_BYTE   byIsIOSarDa;
  APP_BYTE   byIsIOOwner;
  APP_BYTE   byConfIsAccepted;

}ST_PND_AR_INFO,APP_FAR *LPST_PND_AR_INFO;

/* This is a DUA structure, not a Stack one                                 */
/* This is structure is giving as an exemple of flash storage               */
/* Stack device Profinet IO didn't care about his format                    */
typedef struct _st_pnd_port_remanent_data
{
  APP_DWORD               dwApi;
  APP_WORD                wSlot;
  APP_WORD                wSubSlot;

  /* Related to PND_CB_ExtConfCheckPeer and PND_CB_ExtConfGetCheckPeer (Nb: use of stack strcuture to limit the duplication)*/
  ST_USR_CHECK_PEER_DATA  stCheckPeers;
  APP_BYTE  byIsCheckPeerActivate;

  /* Related to PND_CB_ExtConfCheckMAUTypeMode and PND_CB_ExtConfGetCheckMAUTypeMode */
  APP_BYTE  byIsCheckMAUTypeModeActive;
  APP_WORD  wCheckMAUTypeMode;

  /* Related to PND_CB_ExtConfCheckMAUType and PND_CB_ExtConfGetCheckMAUType */
  APP_BYTE  byIsCheckMAUTypeActive;
  APP_WORD  wCheckMAUtType;

  /* Related to PND_CB_ExtConfCheckLinkState and PND_CB_ExtConfGetCheckLinkState */
  APP_BYTE  byCheckLinkStateActivate;
  APP_BYTE  byCheckLinkState_Link;
  APP_BYTE  byCheckLinkState_Port;

  /* Related to PND_CB_ExtConfAdjustMAUType and PND_CB_ExtConfGetAdjustMAUType*/
  APP_BYTE  byIsAdjustMAUTypeActivate;
  APP_WORD  wAdjustMAUType;

  /* Related to PND_CB_ExtConfAdjustPeerToPeerBoundary and PND_CB_ExtConfGetAdjustPeerToPeerBoundary*/
  APP_BYTE  byIsAdjustPeerToPeerBoundaryActivate;
  APP_DWORD dwPeerToPeerBoundary;

  /* Related to PND_CB_ExtConfAdjustDCPBoundary and PND_CB_ExtConfGetAdjustDCPBoundary*/
  APP_BYTE  byIsAdjustDCPBoundaryActivate;
  APP_DWORD dwDCPBoundary;

  /* Related to PND_CB_ExtConfAdjustPreambuleLength and PND_CB_ExtConfGetAdjustPreambuleLength*/
  APP_BYTE  byIsAdjustPreambuleLengthActivate;
  APP_WORD  wPreambleLength;

  #ifdef MRP_SUPPORTED
  /* Related to PND_CB_ExtConfAdjustMRP and PND_CB_ExtConfGetAdjustMRP */
  APP_BYTE  byIsAdjustMRPActivate;
  ST_MRP_UUID stAdjustMrpUUID;
  #endif

  #ifdef PN_RTC3_SUPPORTED
  /* Related to PND_CB_ExtConfCheckLineDelay and PND_CB_ExtConfGetCheckLineDelay */
  APP_BYTE  byIsCheckLineDelayeActivate;
  APP_BYTE  byCheckLineDelayFormatIndicator;
  APP_DWORD dwCheckLineDelayValue;

  /* Related to PND_CB_ExtConfCheckSyncMode and PND_CB_ExtConfGetCheckSyncMode */
  APP_BYTE  byCheckSyncModeActivate;
  APP_BYTE  byCheckSyncMode_CableDelay;
  APP_BYTE  byCheckSyncMode_SyncMaster;

  /* Related to PND_CB_ExtConfAdjustDomainBoundary and PND_CB_ExtConfGetAdjustDomainBoundary */
  APP_BYTE  byIsAdjustDomainBoundaryActivate;
  APP_DWORD dwAdjustDomainBoundaryIngress;
  APP_DWORD dwAdjustDomainBoundaryEgress;

  /* Related to PND_CB_ExtConfAdjustLinkState and PND_CB_ExtConfGetAdjustLinkState */
  APP_BYTE  byIsAdjustLinkStateActivate;
  APP_BYTE  byAdjustLinkState_Link;
  APP_BYTE  byAdjustLinkState_Port;

  /* Related to PND_CB_ExtConfAdjustMulticastBoundary and PND_CB_ExtConfGetAdjustMulticastBoundary */
  APP_BYTE  byIsAdjustMulticastBoundaryActivate;
  APP_DWORD dwAdjustMulticastBoundary;
  #endif

}ST_PND_PORT_REMANENT_DATA, APP_FAR *LPST_PND_PORT_REMANENT_DATA;

/* This is a DUA structure, not a Stack one                                 */
/* This is structure is giving as an exemple of flash storage               */
/* Stack device Profinet IO didn't care about his format                    */
typedef struct _st_pnd_remanent_data
{
  APP_DWORD dwVersion;
  APP_DWORD dwRemDataLen;
  APP_BYTE  wIpMode;                            /* PND_IP_STATIC or PND_IP_DHCP                                                            */
  APP_DWORD dwIpAddress;                        /* Adresse IP of the device                                                                */
  APP_DWORD dwSubNetMask;                       /* Subnet mask                                                                             */
  APP_DWORD dwGateway;                          /* Gateway                                                                                 */

  APP_CHAR sName[NAMELEN+1];                      /* name of the device: the name size must be lower than NAMELEN (240)                      */
  APP_WORD wNameLen;

  /* Related to PND_CB_ExtConfAdjustFsuItf and  PND_CB_ExtConfGetAdjustFsuItf */
  APP_BYTE  byIsAdjustFsuActivate;
  APP_BYTE  byFsuEnable;
  APP_BYTE  byFsuInitiativeDelayMode;
  APP_WORD  wHelloInterval;
  APP_WORD  wHelloRetryCount;
  APP_WORD  wHelloDelay;

  /* Related to PND_CB_ExtConfAdjustItf and PND_CB_ExtConfGetAdjustItf */
  APP_BYTE byItfAdjustEnable;
  APP_BYTE byMultiInterfaceNameOfDevice;

  /* Related to PND_CB_ReadInd and PND_CB_WriteInd: wIndex == INDEX_SUBSLOT_IM1*/
  APP_BYTE IM1_Tag_Function[IM_Tag_Function_LEN];
  APP_BYTE IM1_Tag_Location[IM_Tag_Location_LEN];

  /* Related to PND_CB_ReadInd and PND_CB_WriteInd: wIndex == INDEX_SUBSLOT_IM1*/
  APP_BYTE IM2_Date[IM_Date_LEN];

  /* Related to PND_CB_ReadInd and PND_CB_WriteInd: wIndex == INDEX_SUBSLOT_IM1*/
  APP_BYTE IM3_Descriptor[IM_Descriptor_LEN];

  /* Related to PND_CB_ReadInd and PND_CB_WriteInd: wIndex == INDEX_SUBSLOT_IM1*/
  APP_BYTE IM4_Signature[IM_Signature_LEN];

  #ifdef MRP_SUPPORTED
  /* Related to PND_CB_ExtConfCheckMRPItf and PND_CB_ExtConfGetCheckMRPItf */
  ST_MRP_UUID stCheckMrpUUID;
  APP_BYTE    byIsCheckMRPActivate;
  APP_BYTE    byCheckMediaRedundancyManager;
  APP_BYTE    byCheckMRPDomainUUID;

  /* Related to PND_CB_ExtConfGetAdjustMRPItf and PND_CB_ExtConfAdjustMRPItf */
  ST_MRP_MANAGER_PARAM stMrpAdjustManagerParam;
  ST_MRP_CLIENT_PARAM  stMrpAdjustClientParam;
  ST_MRP_UUID          srMrpAdjustUUID;
  APP_BYTE             byIsAdjustMRPActivate;
  APP_BYTE             byMRPAdjustDomainName[NAMELEN];
  APP_BYTE             byMRPAdjustLengthDomainName;
  APP_WORD             wMRPAdjustRole;
  #endif

#ifdef PN_RTC3_SUPPORTED
  /* Related to PND_CB_ExtConfGetAdjustIrGlobalData and PND_CB_ExtConfGetAdjustIrGlobalData */
  APP_BYTE  byIsAdjustIrDataEnable;
  ST_PDIRDATA stAdjustIrData;

  /* Related to PND_CB_ExtConfGetAdjustSyncGlobalData and PND_CB_ExtConfGetAdjustSyncGlobalData */
  APP_BYTE  byIsAdjustSyncDataEnable;
  ST_PDSYNCDATA stAdjustSyncData;
#endif

  /* Related to PND_CB_ExtConfAdjustFsuItf and PND_CB_ExtConfGetAdjustFsuItf */
  ST_PND_PORT_REMANENT_DATA  stPortRemData[PN_CONFIGURATION_PORT_SIZE];

}ST_PND_REMANENT_DATA, APP_FAR *LPST_PND_REMANENT_DATA;


/* This is a DUA structure, not a Stack one                                 */
/* This is structure is giving as an exemple of configuration storage       */
/* Stack device Profinet IO didn't care about his format                    */
/* Only ST_PND_SUB_SLOT_CONF is a stack structure and should not be changed */
typedef struct _device_configuration
{
  APP_DWORD dwNbMaxSlot;                        /* number of data into the  lpstArrayOfSSlot                                               */

  ST_PND_SUB_SLOT_CONF *ArrayOfstSSlot;         /* Array of subslot for this configuration                                                 */

  APP_LPCHAR lpsInterfaceName;                  /* Interface Name: stack will use this parameter in the HW_OpenRead/HW_OpenWrite function. */

  APP_WORD wDeviceID;                           /* Device ID                                                                               */
  APP_WORD wVendorID;                           /* Vendor ID                                                                               */
  APP_WORD wInstance;                           /* Instance use for this device ID (for now should be 1                                    */
  APP_WORD wPort;                              /* Interface Number: this number is use as parameter for this HW_SetSeletedDevice function. */

  APP_CHAR lpbyDeviceType[NAMELEN];             /* Type of the device       */   

  ST_PND_REMANENT_DATA stRemData;

}ST_PND_CONF,APP_FAR *LPST_PND_CONF;

/************************ function ************************/
APP_DWORD ConfigurationLoad      (LPST_PND_CONF     pndConfiguration);
APP_DWORD InitProfinetStackDevice(APP_LPVOID  *lppPhysicalDevice,  APP_LPVOID     *lppLogivalDevice, LPST_PND_CONF lpstConf, APP_LPBYTE lpbyContexteD, APP_LPBYTE lpbyContexteI);
APP_DWORD PullConfiguration      (APP_LPVOID  lpLogivalDevice,     LPST_PND_CONF lpstConf, APP_LPBYTE lpbyContext);

APP_DWORD GetInputDataFromNetwork (LPST_PND_AR_INFO lpStDuaInfo, LPST_PND_CONF lpstConf);
APP_DWORD SetOutputDataToNetwork  (LPST_PND_AR_INFO lpStDuaInfo, LPST_PND_CONF lpstConf);
APP_WORD  UserSendAlm             (APP_LPVOID lpLogicalDevice);
APP_WORD  UserPlugPull            (APP_LPVOID lpLogicalDevice, APP_BYTE byHasToPlug, LPST_PND_CONF lpstPndConfiguration);
APP_WORD  UserWriteProcessing     (APP_DWORD dwARContextInd, APP_DWORD dwApi, APP_WORD wIndex, APP_WORD wSlot, APP_WORD wSubSlot, APP_DWORD  dwSizeOfData, APP_LPBYTE lpbyData, LPST_PNIO_RW_STATUS lpStatus, APP_LPWORD lpwbyAddData1, APP_LPWORD lpwAddData2);
APP_WORD  UserReadProcessing      (APP_DWORD dwARContextInd, APP_DWORD dwApi, APP_WORD wIndex, APP_WORD wSlot, APP_WORD wSubSlot, APP_LPDWORD  lpdwSizeOfData, APP_LPBYTE lpbyData, LPST_PNIO_RW_STATUS lpStatus, APP_LPBYTE lpbyAddData1, APP_LPBYTE lpbyAddData2);
APP_DWORD ReleaseDUABuffer        (LPST_PND_CONF lpstPndConfiguration);


/* some define used by the DUA manage the current connexion state */
#define PND_DUA_STATUS_WFC         0  /* waiting for connection               */
#define PND_DUA_STATUS_WF_APP_RDY  1  /* DUA have to send Application ready   */
#define PND_DUA_STATUS_WF_RDY_CNF  2  /* DUA hwait Application ready callback */
#define PND_DUA_STATUS_FIRST_DATA  3  /* Stack send his first true data       */
#define PND_DUA_STATUS_CONNECTED   4  /* Stack is connected                   */

/************************ Macro ************************/
/* Put the data for the givin slot into this buffer. Buffer is init with the size of PND_SIZEOF_INPUT_DATA */
#define PND_DUA_GET_INPUT_DATA(wSlot ,byData)  OS_MEMSET(byData, gwCurrentData, PND_SIZEOF_OUTPUT_DATA)

/* This function is call by the default DUA. Data send to this function are the output of the data*/
#define PND_DUA_SET_OUTPUT_DATA(wSlot ,byData)  /*printf("Slot %d -- Data %02x \n", wSlot, byData[0] ) Nothing, because printing this informations is to long */

APP_VOID  InitializeIMRecord(APP_VOID);
APP_DWORD BuildParamStack(LPST_ST_PARAM lpstParam);

/************************ structure ************************/

/* the context use to recover information when we update data*/
typedef struct _context_input_output
{
    APP_DWORD API;
    APP_WORD  Slot;
    APP_WORD  SubSlot;
    APP_BYTE  byRequestFinish;

}ST_CONTEXT_IN_OUT,APP_FAR *LPST_CONTEXT_IN_OUT;



#endif

