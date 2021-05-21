/*
+------------------------------------------------------------------------------+
| Woodhead Software & Electronics                                              |
| -------------------------------                                              |
|                                                                              |
|  Copyright 2006 (c)   Woodhead Software & Electronics. All rights reserved.  |
+------------------------------------------------------------------------------+
  Description .......:                                                          
                       
+------------------------------------------------------------------------------+
  $Workfile: eip_api.h $
  $Revision: 70 $
  $Date: 9/08/17 13:50 $
+------------------------------------------------------------------------------+
  Version | Date     | Author | Comments
            mm/dd/yy
  1.0.0     08/03/06   J.B.     Initial stack sources 
+------------------------------------------------------------------------------+
*/

#ifndef __EIP_API_H
#define __EIP_API_H

/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/
#include "eip_types.h"
#include "eip_err.h"

/*---------------------------------------------------------------------------*/
/* Constant Definitions                                                      */
/*---------------------------------------------------------------------------*/
/* Connection Type*/
#define CONNECTION_EIP_NULL           0
#define CONNECTION_MULTICAST      1
#define CONNECTION_POINT_TO_POINT 2

/* Production Trigger*/
#define TRIGGER_CYCLIC              0
#define TRIGGER_COS                 1
#define TRIGGER_APPLICATION_OBJECT  2
#define TRIGGER_NOT_DEFINED         7

/* Transport Class*/
#define CLASS_0     0
#define CLASS_1     1
#define CLASS_2     2
#define CLASS_3     3

/* Max Header of Data Item (SequenceCount (Only Class 1) + [0 (Modeless) or 4 (32-Bit Header)]) => 2 + 4 = 6*/
#define CIP_DATA_ITEM_HEADER 6

/* Object CM, filters*/
#define CM_FILTER_CNXN_CLASS_0  0x00010001
#define CM_FILTER_CNXN_CLASS_1  0x00010002

#define EIP_CAPABILITY_BIT_SAFETY         0
#define EIP_CAPABILITY_BIT_ACD            1
#define EIP_CAPABILITY_BIT_QUICK_CONNECT  2
#define EIP_CAPABILITY_BIT_MCAST_TTL      3

#define EIP_CAPABILITY_SAFETY (1 << EIP_CAPABILITY_BIT_SAFETY)
#define EIP_CAPABILITY_ACD (1 << EIP_CAPABILITY_BIT_ACD)
#define EIP_CAPABILITY_QUICK_CONNECT (1 << EIP_CAPABILITY_BIT_QUICK_CONNECT)
#define EIP_CAPABILITY_MCAST_TTL (1 << EIP_CAPABILITY_BIT_MCAST_TTL)

#define EIP_CAPABILITY_IS_ENABLED(Capabilities, Bit) (EIP_BOOL)(((Capabilities) >> (Bit)) & 1)

/*---------------------------------------------------------------------------*/
/* STRUCTURE DEFINITIONS                                                     */
/*---------------------------------------------------------------------------*/
typedef struct
{
  /*! Explicit message timeout (in seconds) */
  EIP_WORD   wExpMsgTimeOut;
  /*! Stack capabilities, see EIP_CAPABILITY_XXXX for all options that can be set */
  EIP_DWORD  dwCapabilities;
} ST_EIP_INIT, EIP_NEAR * PST_EIP_INIT, EIP_FAR * LPST_EIP_INIT;
typedef struct
{
  EIP_DWORD  dwLocalIP;
  EIP_DWORD  dwSubNetMask;
  EIP_DWORD  dwGateway;
} ST_EIP_ONLINE, EIP_NEAR * PST_EIP_ONLINE, EIP_FAR * LPST_EIP_ONLINE;

typedef struct
{
  UDINT O_to_T_Connection_ID ;
  UDINT T_to_O_Connection_ID ;
  UINT  Connection_Serial_Number ;
  UINT  Originator_Vendor_ID ;
  UDINT Originator_Serial_Number ;
  UDINT O_to_T_API ;
  UDINT T_to_O_API ;
  USINT Application_Reply_Size ;
  USINT Reserved ;
} ST_FWOPEN_RESPONSE;

typedef struct
{
  /* Input*/
  EIP_WORD            wTransacNum;
  EIP_BYTE           *byFwOpenReq;
  /* Output*/
  ST_FWOPEN_RESPONSE  stFwdOpenResponse;
  EIP_LPVOID          lpvIODataRef_OT;
  EIP_LPVOID          lpvIODataRef_TO;

  EIP_LPVOID          lpvProd;
  EIP_WORD            wConnectionID;
  
  EIP_WORD            wDataSize_OT;
  EIP_WORD            wDataSize_TO;
} ST_IOCNXN_PARAMS;

typedef struct
{
  EIP_LPVOID  lpvDataIn;
  EIP_LPVOID  lpvDataOut;
  EIP_LPVOID  lpvCtrlTable;
  EIP_LPVOID  lpvHealthTable;
  EIP_LPVOID  lpvStatusIn;
  EIP_LPVOID  lpvStatusOut;

  EIP_LPVOID  lpvUpdateIn;
  EIP_LPVOID  lpvUpdateOut;
  EIP_LPBYTE  lpbyHandshake[2];
} ST_SHM_PARAMS,  * PST_SHM_PARAMS,  * LPST_SHM_PARAMS;

typedef struct
{ 
  EIP_BYTE byState;
  EIP_BYTE byInstanceType;
  EIP_BYTE byTransportClassTrigger;
 
  EIP_WORD  wOriCnxnSerialNumber;
  EIP_WORD  wOriVendorID;
  EIP_DWORD dwOriSerialNumber;

  EIP_WORD  wTargetCnxnType;
  EIP_DWORD dwRpiOT;
  EIP_DWORD dwRpiTO;
  EIP_WORD  wSizeOT;
  EIP_WORD  wSizeTO;

  EIP_DWORD dwSockSendIpForeign;
  EIP_WORD  wSockSendPortForeign;
  EIP_DWORD dwSockSendIpLocal;
  EIP_WORD  wSockSendPortLocal;

  EIP_DWORD dwSockRecvIpForeign;
  EIP_WORD  wSockRecvPortForeign;
  EIP_DWORD dwSockRecvIpLocal;
  EIP_WORD  wSockRecvPortLocal;
} ST_CNXN_INFO,  * PST_CNXN_INFO,  * LPST_CNXN_INFO;

/*---------------------------------------------------------------------------*/
/* PUBLIC APIs                                                               */
/*---------------------------------------------------------------------------*/

EIP_BOOL EIP_Init      (ST_EIP_INIT *   pstEipInit);
EIP_BOOL EIP_Exit      (EIP_VOID);
EIP_BOOL EIP_Configure (EIP_BYTE * pbyBuffer, ST_SHM_PARAMS * pstSHMParams);
EIP_VOID EIP_Process   (EIP_VOID);
EIP_BOOL EIP_Start     (EIP_VOID);
EIP_BOOL EIP_Stop      (EIP_VOID);
EIP_VOID EIP_GetVersion(EIP_BYTE * pMajor, EIP_BYTE * pMinor, EIP_BYTE * pBuild, EIP_BYTE * pSpecial);

EIP_BOOL EIP_Online    (ST_EIP_ONLINE * pstEipOnline);
EIP_BOOL EIP_Offline   (EIP_VOID);

EIP_BOOL EIP_GetCapability(EIP_BYTE byBit);

EIP_BOOL CM_CreateIoConnection( ST_IOCNXN_PARAMS * pstIoCnxnParams,ST_CIP_STATUS *pstCipStatus);

EIP_BOOL CM_ProduceIoData(EIP_LPVOID lpvProd, EIP_LPBYTE lpBuffer, EIP_WORD wSizeToCopy, EIP_LPSHORT lpnStatus);

EIP_BOOL CM_CloseIoConnection(  EIP_WORD        Connection_Serial_Number,
                                EIP_WORD        Originator_Vendor_ID,
                                EIP_DWORD       Originator_Serial_Number,
                                LPST_CIP_STATUS pstCipStatus            );

EIP_BOOL CM_SetUserCID(EIP_WORD wConnectionID, EIP_DWORD dwUserCID);

EIP_BOOL CN_GetConnectionInfo(EIP_WORD wInstanceNumber, ST_CNXN_INFO * pstCnxnInfo);

/* Ethernet object */
EIP_BOOL ETH_AddInstance(EIP_WORD wInstance);

#endif /*__EIP_API_H*/
