/*
+------------------------------------------------------------------------------+
| Woodhead Software & Electronics                                              |
| -------------------------------                                              |
|                                                                              |
|  Copyright 2006 (c)   Woodhead Software & Electronics. All rights reserved.  |
+------------------------------------------------------------------------------+
  Description .......:                                                          
                       
+------------------------------------------------------------------------------+
  $Workfile: user_api.h $
  $Revision: 49 $
  $Date: 24/09/14 14:08 $
+------------------------------------------------------------------------------+
  Version | Date     | Author	| Comments
			mm/dd/yy
  1.0.0     12/17/04   E.L.     Initial stack sources
  1.0.1     04/05/05   E.L.     Add async mode for Exp. Msg Server
                                  struct ST_USER_EM_UNR modified
+-------------------------------------------------------------------------------+
*/

#ifndef __USER_API_H
#define __USER_API_H


/*---------------------------------------------------------------------------*/
/* Constant Definitions                                                      */
/*---------------------------------------------------------------------------*/

// Events
#define EVT_EMSRV_NEWREQ        0x0200
#define EVT_EMSRV_NEWUNCONN     0x0300

#define EVT_CNXN_NONE        		0x0400
#define EVT_CNXN_CONNECTED   		0x0401
#define EVT_CNXN_TIMEOUT     		0x0402

#define EVT_SHM_IN_UPDATED      0x0500

#define EVT_SHM_OUT_ACK         0x0600

// CM User Managed
#define CM_NOT_USER_MANAGED     0x00000000
#define CM_DON_T_AUTO_PRODUCE   0x00000001
#define CM_CLOSE_NOTIFICATION   0x00000002
#define CM_RECEIVE_FW_OPEN      0x00000004
#define CM_RECEIVE_FW_CLOSE     0x00000008
#define CM_NOTIFY_ALL_PACKET    0x00000010

/*---------------------------------------------------------------------------*/
/* Functions prototypes                                                      */
/*---------------------------------------------------------------------------*/

EIP_BOOL USER_Init (EIP_VOID);
EIP_BOOL USER_Exit(EIP_VOID);
EIP_VOID USER_Event(EIP_WORD wEventID);
EIP_VOID USER_GetVersion(EIP_BYTE * pMajor, EIP_BYTE * pMinor, EIP_BYTE * pBuild, EIP_BYTE * pSpecial);

EIP_BOOL USER_ID_GetAttributeValue(EIP_WORD wAttributNumber, LPCLASS_IDENTITY pstIdentityObj, LPST_CIP_STATUS pstCipStatus);
EIP_BOOL USER_ID_ResetObject(EIP_BYTE byTypeOfReset, LPST_CIP_STATUS pstCipStatus);

EIP_BOOL USER_TCP_GetAttributeValue(EIP_WORD wAttributNumber, LPCLASS_TCP_IP pstTcpIpObj, LPST_CIP_STATUS pstCipStatus);
EIP_BOOL USER_TCP_SetAttributeValue(EIP_WORD wAttributNumber, LPCLASS_TCP_IP pstTcpIpObj, LPST_CIP_STATUS pstCipStatus);

EIP_BOOL USER_ETH_GetAttributeValue(EIP_WORD wInstance, EIP_WORD wAttributNumber, LPCLASS_ETHERNET_LINK pstEthernetObj, EIP_BOOL bClear, LPST_CIP_STATUS pstCipStatus);
EIP_BOOL USER_ETH_SetAttributeValue(EIP_WORD wInstance, EIP_WORD wAttributNumber, LPCLASS_ETHERNET_LINK pstEthernetObj, LPST_CIP_STATUS pstCipStatus);

EIP_BOOL USER_QoS_GetAttributeValue(CLASS_QOS * lpQosObj, EIP_WORD * pwQosEnable, LPST_CIP_STATUS pstCipStatus);
EIP_BOOL USER_QoS_SetAttributeValue(EIP_WORD wAttributID, CLASS_QOS * lpQosObj,  ST_CIP_STATUS * pstCipStatus);

EIP_VOID USER_CM_ConnectionClosed (EIP_WORD wConnectionID);

EIP_DWORD USER_CM_HowCnxnIsUserManaged (EIP_LPVOID lpFwdOpenRequest, EIP_BOOL bIsLarge);

EIP_BOOL SO_Open_Session();
EIP_BOOL SO_Close_Session();

/*---------------------------------------------------------------------------*/
#endif //__USER_API_H
