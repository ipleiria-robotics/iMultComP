/*
+------------------------------------------------------------------------------+
| Woodhead Software & Electronics                                              |
| -------------------------------                                              |
|                                                                              |
|  Copyright 2006 (c)   Woodhead Software & Electronics. All rights reserved.  |
+------------------------------------------------------------------------------+
  Description .......:                                                          
                       
+------------------------------------------------------------------------------+
  $Workfile: io_api.h $
  $Revision: 24 $
  $Date: 24/09/14 14:08 $
+------------------------------------------------------------------------------+
  Version | Date     | Author | Comments
            mm/dd/yy
  1.0.0     08/26/08   C.D.     New file creation
+------------------------------------------------------------------------------+
*/

#ifndef __IO_API_H
#define __IO_API_H

/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/
#include "eip_types.h"
#include "eip_err.h"

/*---------------------------------------------------------------------------*/
/* Definitions                                                               */
/*---------------------------------------------------------------------------*/
#define IO_NB_PRODUCTION   (CIP_ORIGINATOR_IO + CIP_TARGET_IO)
#define IO_NB_CONSUMPTION  (CIP_ORIGINATOR_IO + CIP_TARGET_IO)

#define INDEX_FIRST_IO              1
#define INDEX_FIRST_IO_TARGET       (CIP_ORIGINATOR_IO + 1)

/*---------------------------------------------------------------------------*/
/* PUBLIC APIs                                                               */
/*---------------------------------------------------------------------------*/
EIP_BOOL IO_SetModeRun(EIP_VOID);
EIP_BOOL IO_SetModeIdle(EIP_VOID);

EIP_WORD IO_ReadData(EIP_LPVOID lpvIODataRef, EIP_LPBYTE lpBuffer, EIP_WORD wSizeToCopy, EIP_LPSHORT lpnStatus);
EIP_WORD IO_WriteData(EIP_LPVOID lpvIODataRef, EIP_LPBYTE lpBuffer, EIP_WORD wSizeToCopy, EIP_LPSHORT lpnStatus);
EIP_SHORT IO_GetStatus(EIP_LPVOID lpvIODataRef);
EIP_SHORT IO_GetCipStatus (EIP_LPVOID lpvIODataRef, ST_CIP_STATUS * pstCipStatus);

EIP_DWORD IO_GetLastCID(EIP_LPVOID lpvIODataRef);
EIP_DWORD IO_GetLastUserCID(EIP_LPVOID lpvIODataRef);

/*---------------------------------------------------------------------------*/
#endif
