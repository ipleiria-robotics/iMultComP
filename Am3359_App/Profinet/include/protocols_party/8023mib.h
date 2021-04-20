/*---------------------------------------------------------------------------*/
/*  Copyright (C) 2006 Woodhead Software & Electonics. All rights reserved.  */
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
/*  Project             : STACK PROFINET Controller                          */
/*  Component           : LLDP                                               */
/* $Workfile:: 8023mib.h                                                    $*/
/* $Revision:: 5                                                            $*/
/* $Date:: 17/10/16 9:32                                                    $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*  - LLDP 802.3 MIB header                                                  */
/*---------------------------------------------------------------------------*/
#ifndef _8023_MIB_H
#define _8023_MIB_H


#include "lldpt.h"

typedef struct ST_LLDP_LOCAL_8023_MIB{

  APP_WORD w8023OperationMauType;
  APP_BYTE by8023TxEnabled;
  APP_BOOL b8023AutoNegSupported;
  APP_BOOL b8023AutoNegEnabled;
  APP_WORD w8023AutoNegAdvertisedCap;

}ST_LLDP_LOCAL_8023_MIB, APP_FAR * LPST_LLDP_LOCAL_8023_MIB;


typedef struct _LLDP_REMOTE_8023_MIB{

  APP_WORD w8023RemOperationMauType;
  APP_BOOL b8023RemAutoNegSupported;
  APP_BOOL b8023RemAutoNegEnabled;
  APP_BYTE s8023RemAutoNegAdvertisedCap[2];


}ST_LLDP_REMOTE_8023_MIB, APP_FAR * LPST_LLDP_REMOTE_8023_MIB;


/* 8023 Tx Enabled Masks */
#define LLDP_8023_MACPHYCONFSTATUS_MASK           1
#define LLDP_8023_POWERVIAMDI_MASK                2   /* Not supported Yet */
#define LLDP_8023_LINKAGGREGATION_MASK            4   /* Not supported Yet */
#define LLDP_8023_MACFRAMESIZE_MASK               8   /* Not supported Yet */


#define LLDP_8023_AutoNeg_Supported_Bit           1
#define LLDP_8023_AutoNeg_Enabled_Bit             2


#define WSE_LLDP_8023_MAUTYPE                     0x0010
#define WSE_LLDP_8023_ADVERTISED_100TXFD          0x6C00

APP_WORD Initialise8023LocalMIB(LPST_LLDP_INIT_PORT lpstLldpPort, APP_LPWORD lpwStatus);

#endif  /* END OF HEADER FILE */
