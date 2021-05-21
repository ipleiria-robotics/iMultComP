/*
+------------------------------------------------------------------------------+
| Woodhead Software & Electronics                                              |
| -------------------------------                                              |
|                                                                              |
|  Copyright 2006 (c)   Woodhead Software & Electronics. All rights reserved.  |
+------------------------------------------------------------------------------+
  Description .......:                                                          
                       
+------------------------------------------------------------------------------+
  $Workfile: obj_api.h $
  $Revision: 3 $
  $Date: 21/01/11 14:54 $
+------------------------------------------------------------------------------+
  Version | Date     | Author | Comments
            mm/dd/yy
  1.0.0     16/02/10   Jean-Yves Beaugrand
+------------------------------------------------------------------------------+
*/

#ifndef __OBJ_API_H
#define __OBJ_API_H

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
  EIP_BYTE  * lpResFrame;
  EIP_WORD  wResFrameSize;
}ST_EIP_EM_RES,  EIP_NEAR * P_ST_EIP_EM_RES, EIP_FAR * LP_ST_EIP_EM_RES;

typedef struct
{ 
  EIP_WORD  wClassID;
  EIP_BYTE  byService;
  EIP_DWORD dwInstance;
  EIP_WORD  wAttribut;
  EPATH     EPath;
  EIP_WORD  wPathSize;
  EIP_BYTE  * lpData;
  EIP_WORD  wDataSize;
}ST_EIP_EM_REQ, EIP_NEAR * P_ST_EIP_EM_REQ, EIP_FAR * LP_ST_EIP_EM_REQ;

typedef EIP_BOOL ( *LP_OBJ_CALL_BACK) (EIP_WORD wTransacID, LP_ST_EIP_EM_REQ pstEIP_EmReq, LP_ST_EIP_EM_RES pstEIP_EmRes);

/*---------------------------------------------------------------------------*/
/* PUBLIC APIs                                                               */
/*---------------------------------------------------------------------------*/
EIP_BOOL OBJ_CreateObject (EIP_WORD wClassID);

EIP_BOOL OBJ_DeleteObject (EIP_WORD wClassID);

EIP_BOOL OBJ_DeleteAllObject (EIP_VOID);

EIP_BOOL OBJ_AddClassService (EIP_WORD wClassID, EIP_BYTE byService, LP_OBJ_CALL_BACK ObjCallBack);
EIP_BOOL OBJ_AddClassAttribute (EIP_WORD wClassID, EIP_WORD wAttribute);
EIP_BOOL OBJ_AddInstance (EIP_WORD wClassID, EIP_DWORD dwInstance);
EIP_BOOL OBJ_AddInstanceService (EIP_WORD wClassID, EIP_BYTE byService, LP_OBJ_CALL_BACK ObjCallBack);
EIP_BOOL OBJ_AddInstanceAttribute (EIP_WORD wClassID, EIP_WORD wAttribute);
EIP_BOOL OBJ_SendExpMsgRes(EIP_WORD wTransacID, LP_ST_EIP_EM_RES pstEIP_EmRes);

/*---------------------------------------------------------------------------*/
#endif
