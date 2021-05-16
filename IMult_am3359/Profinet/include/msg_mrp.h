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
/*  Component           : MRP                                                */
/* $Workfile:: msg_mrp.h                                                    $*/
/* $Revision:: 1                                                            $*/
/* $Date:: 13/04/16 17:17                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/


#ifndef __MSG_MRP_H__
#define __MSG_MRP_H__

#include "message.h"
#include "mrp_pkg.h"

#ifdef __cplusplus
extern "C"
{
#endif

APP_DWORD APP_FAR ase_mrp_init              (LPST_MSG* ppMsg);
APP_DWORD APP_FAR ase_mrp_exit              (LPST_MSG* ppMsg);
APP_DWORD APP_FAR ase_mrm_start             (LPST_MSG* ppMsg);
APP_DWORD APP_FAR ase_mrm_stop              (LPST_MSG* ppMsg);
APP_DWORD APP_FAR ase_mrm_read              (LPST_MSG* ppMsg);
APP_DWORD APP_FAR ase_mrc_start             (LPST_MSG* ppMsg);
APP_DWORD APP_FAR ase_mrc_stop              (LPST_MSG* ppMsg);
APP_DWORD APP_FAR ase_mrc_read              (LPST_MSG* ppMsg);
APP_DWORD APP_FAR ase_mrp_rcv_pdu           (LPST_MSG* ppMsg);
APP_DWORD APP_FAR ase_mrp_local_link_change (LPST_MSG* ppMsg);
APP_DWORD APP_FAR ase_mrp_timer             (LPST_MSG* ppMsg);

typedef struct _ST_MRM_CONF{

  LPST_MRP_UUID lpstDomainUUID;         /*ID--> if null stack will auto generate an UUID*/
  APP_WORD      wRing_port_1_id;
  APP_WORD      wRing_port_2_id;
  APP_WORD      wVlan_id;
  APP_WORD      wManager_priority;
  APP_WORD      wTopology_change_interval;
  APP_WORD      wTopology_change_repeat_count;
  APP_WORD      wShort_test_interval;
  APP_WORD      wDefault_test_interval;
  APP_WORD      wTest_monitoring_count;
  APP_BOOL      bNon_blocking_MRC_supported;
  APP_WORD      wTest_monitoring_extended_count;
  APP_BYTE      bReact_on_link_change;
  APP_BYTE      wCheck_media_redundancy;
  APP_BYTE      byDomainName[240];
  APP_BYTE      byDomainNameLen;
  APP_LPVOID    lpvHandle;

}ST_MRM_CONF, APP_FAR * LPST_MRM_CONF;

typedef struct _ST_MRC_CONF
{
  LPST_MRP_UUID lpstDomainUUID;         /*ID--> if null stack will auto generate an UUID*/
  APP_WORD      wRing_port_1_id;
  APP_WORD      wRing_port_2_id;
  APP_WORD      wVlan_id;
  APP_WORD      wLink_down_interval;
  APP_WORD      wLink_up_interval;
  APP_WORD      wLink_change_count;
  APP_BOOL      bBlocked_state_supported;
  APP_BYTE      byDomainName[240];
  APP_BYTE      byDomainNameLen;
  APP_LPVOID    lpvHandle;

}ST_MRC_CONF, APP_FAR * LPST_MRC_CONF;


/*
---PDInterfaceMrpDataAdjust:---

( 6) ST_PND_BLOCHEADER stHeader;

( 1) APP_BYTE          Padding;              Make it Uint32Align
( 1) APP_BYTE          Padding;              Make it Uint32Align

(16) ST_CLR_RPC_UUID   stDomainUUID;         ID
( 2) APP_WORD          wMrpRole;             Manager or Client ?
( 1) APP_BYTE          Padding;              Make it Uint32Align
( 1) APP_BYTE          Padding;              Make it Uint32Align
--> 28 ( = 4 * 7)
*/
struct  _ST_MRP_COMMON_DATA{

  ST_MRP_UUID       stDomainUUID;
  APP_WORD          wMrpRole;
  APP_BYTE          byMrpDomainNameLen;
  APP_BYTE          byMrpDomainName[240];
};
typedef struct _ST_MRP_COMMON_DATA (ST_MRP_COMMON_DATA), APP_FAR * LPST_MRP_COMMON_DATA;

/*
---MrpManagerParams---

( 6) ST_PND_BLOCHEADER stHeader,

( 2) APP_WORD          wMrpPrio,
( 2) APP_WORD          wMrpTopChangeT,
( 2) APP_WORD          wMrpTopNrMax,
( 2) APP_WORD          wMrpTstShortT,
( 2) APP_WORD          wMrpTstDefaultT,
( 2) APP_WORD          wMrpTstNrMax,
( 2) APP_WORD          Padding,         Make it Uint32Align
*/
struct  _ST_MRP_MANAGER_PARAM{

  APP_WORD          wMrpPrio;
  APP_WORD          wMrpTopChangeT;
  APP_WORD          wMrpTopNrMax;
  APP_WORD          wMrpTstShortT;
  APP_WORD          wMrpTstDefaultT;
  APP_WORD          wMrpTstNrMax;
  /*APP_WORD          wPadding;*/

};
typedef struct _ST_MRP_MANAGER_PARAM (ST_MRP_MANAGER_PARAM), APP_FAR * LPST_MRP_MANAGER_PARAM;

/*
--MrpClientParams--

( 6) ST_PND_BLOCHEADER stHeader,
( 2) APP_WORD          wMrpLnkDownT,
( 2) APP_WORD          wMrpLnkUpT,
( 2) APP_WORD          wMrpLnkNRmax,
*/
struct  _ST_MRP_CLIENT_PARAM{

  APP_WORD          wMrpLnkDownT;
  APP_WORD          wMrpLnkUpT;
  APP_WORD          wMrpLnkNRmax;

};
typedef struct _ST_MRP_CLIENT_PARAM (ST_MRP_CLIENT_PARAM), APP_FAR * LPST_MRP_CLIENT_PARAM;


struct  _ST_MRP_ALL_DATA{

  ST_MRP_COMMON_DATA             stMrpCommon;
  ST_MRP_MANAGER_PARAM           stMrpMp;
  ST_MRP_CLIENT_PARAM            stMrpCp;
  APP_BYTE                       bCheckMrManager; /* for PDInterfaceMrpDataCheck */

};
typedef struct _ST_MRP_ALL_DATA (ST_MRP_ALL_DATA), APP_FAR * LPST_MRP_ALL_DATA;


struct  _ST_MRP_STORAGE{

  ST_MRP_UUID   stMrpUuid;
  APP_BYTE      byIsFree;
  APP_BYTE      byDomainName[240];
  APP_BYTE      byDomainNameLen;

};
typedef struct _ST_MRP_STORAGE (ST_MRP_STORAGE), APP_FAR * LPST_MRP_STORAGE;


APP_DWORD APP_FAR mrpReadInternal           (LPST_MRP_ALL_DATA lpstMrpData);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif


