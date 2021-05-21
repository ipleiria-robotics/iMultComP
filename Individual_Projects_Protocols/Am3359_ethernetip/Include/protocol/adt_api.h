/*
+------------------------------------------------------------------------------+
| Woodhead Software & Electronics                                              |
| -------------------------------                                              |
|                                                                              |
|  Copyright 2006 (c)   Woodhead Software & Electronics. All rights reserved.  |
+------------------------------------------------------------------------------+
  Description .......:                                                          
                       
+------------------------------------------------------------------------------+
  $Workfile: adt_api.h $
  $Revision: 14 $
  $Date: 5/05/11 10:43 $
+------------------------------------------------------------------------------+
  Version | Date     | Author | Comments
            mm/dd/yy
  1.0.0     08/26/08   C.D.     New file creation
+------------------------------------------------------------------------------+
*/

#ifndef __ADT_API_H
#define __ADT_API_H

/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/
#include "eip_types.h"
#include "eip_err.h"

/*---------------------------------------------------------------------------*/
/* Constant Definitions                                                      */
/*---------------------------------------------------------------------------*/
// ----------------------------------------------------------------------------
// ASSEMBLY OBJECT TYPES
// ----------------------------------------------------------------------------
#define LINK_IO_ASM_TYPE_MASK     0x00FF

#define LINK_IO_ASM_TYPE_CONFIG   0x0001 
#define LINK_IO_ASM_TYPE_TO       0x0002
#define LINK_IO_ASM_TYPE_OT       0x0003

#define LINK_IO_ASM_TYPE_OT_EO    0x0013
#define LINK_IO_ASM_TYPE_OT_IO    0x0023
#define LINK_IO_ASM_TYPE_OT_LO    0x0033

#define LINK_IO_ASM_CNXN_VARIABLE 0x4000
#define LINK_IO_ASM_USER_MANAGED  0x8000

// ----------------------------------------------------------------------------
// ASSEMBLY OBJECT REAL-TYPE FORMATS
// ----------------------------------------------------------------------------
#define RTF_NONE                0xFFFF
#define RTF_32_BIT_HEADER       0
#define RTF_ZERO_LENGTH_DATA    1
#define RTF_MODELESS            2
#define RTF_HEARTBEAT           3
#define RTF_SAFETY              5

/* Stream number used by the stack to open the implicits messages (min 1) */
#define EIP_SCANNER_STREAM_NUMBER_IO    1
/* Stream number used by the stack for unconnected exp msg */
#define EIP_SCANNER_STREAM_NUMBER_UN_EM 2
/* Stream number used by the stack for connected exp msg */
#define EIP_SCANNER_STREAM_NUMBER_CO_EM 3

/*---------------------------------------------------------------------------*/
/* STRUCTURE DEFINITIONS                                                     */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* PUBLIC APIs                                                               */
/*---------------------------------------------------------------------------*/
EIP_LPVOID  ADT_CreateAssemblyObject(EIP_WORD wInstance, EIP_WORD wAssemblyType, EIP_WORD wRealTimeFormat, EIP_WORD wDataSize);
EIP_BOOL    ADT_DeleteAssemblyObject(EIP_WORD wInstance);
EIP_VOID    ADT_ChangeIndex         (EIP_LPVOID lpvIODataRef, EIP_WORD wIndex);

/*---------------------------------------------------------------------------*/
#endif
