/*
+------------------------------------------------------------------------------+
| Woodhead Software & Electronics                                              |
| -------------------------------                                              |
|                                                                              |
|  Copyright 2006 (c)   Woodhead Software & Electronics. All rights reserved.  |
+------------------------------------------------------------------------------+
  Description .......:                                                          
                       
+------------------------------------------------------------------------------+
  $Workfile: ems_api.h $
  $Revision: 12 $
  $Date: 5/03/14 18:03 $
+------------------------------------------------------------------------------+
  Version | Date     | Author | Comments
            mm/dd/yy
  1.0.0     08/26/08   C.D.     New file creation
+------------------------------------------------------------------------------+
*/

#ifndef __EMS_API_H
#define __EMS_API_H

/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/
#include "eip_types.h"
#include "eip_err.h"

/*---------------------------------------------------------------------------*/
/* STRUCTURE DEFINITIONS                                                     */
/*---------------------------------------------------------------------------*/

typedef struct
{
  EIP_WORD    wTransacId;
  EIP_WORD    wFlags;
  EIP_WORD    wClassId;
  EIP_WORD    wCnxnNumber;
  EIP_WORD    wMsgDataSize;
  EIP_LPBYTE  lpbyMsgData;   
}ST_EMSRV_MSG;

/*---------------------------------------------------------------------------*/
/* PUBLIC APIs                                                               */
/*---------------------------------------------------------------------------*/
EIP_BOOL EM_RegisterClass           ( EIP_WORD wClass,           EIP_DWORD * pdwStatus);
EIP_BOOL EM_UnRegisterClass         ( EIP_WORD wClass,           EIP_DWORD * pdwStatus);
EIP_BOOL EM_GetUnsolicitedMsg       ( ST_EMSRV_MSG *pstEMSrvMsg, EIP_DWORD * pdwStatus);
EIP_BOOL EM_ReplyUnsolicitedMsg     ( ST_EMSRV_MSG *pstEMSrvMsg, EIP_DWORD * pdwStatus);
EIP_BOOL EM_GetUnconnectedSendMsg   ( ST_EMSRV_MSG *pstEMSrvMsg, EIP_DWORD * pdwStatus);
EIP_BOOL EM_ReplyUnconnectedSendMsg ( ST_EMSRV_MSG *pstEMSrvMsg, EIP_DWORD * pdwStatus);
EIP_BOOL EM_SendExpMsgRes           ( EIP_WORD wMessageId,
                                      ST_CIP_STATUS stCipStatus,
                                      EIP_WORD wExpMsgSizeRes,
                                      EIP_LPBYTE ExpMsgDataRes,
                                      EIP_DWORD *pdwStatus);

/*---------------------------------------------------------------------------*/
#endif
