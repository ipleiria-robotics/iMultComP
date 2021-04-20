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
/* $Workfile:: tlv.h                                                        $*/
/* $Revision:: 6                                                            $*/
/* $Date:: 29/07/14 15:51                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*  - LLDP TLV generator & checker                                           */
/*---------------------------------------------------------------------------*/
#ifndef _TLV_H
#define _TLV_H

#ifdef LLDP_RECEIVER

#define TLV_ORG_PN       1
#define TLV_ORG_8023     2
#define TLV_ORG_UNKNOWN  255

typedef struct _ST_LLDP_TLV {

  APP_BYTE    byType;
  APP_WORD    wLength;
  APP_LPBYTE  lpbyInfo;
} ST_LLDP_TLV, APP_FAR * LPST_LLDP_TLV;
#define DISCARD_TLV(a) OS_MEMSET(a, 0, sizeof(ST_LLDP_TLV))

extern APP_WORD (*Validate_Tlv[128])(LPST_LLDP_TLV lpstTlv);
APP_WORD InitializeTLVFunctionValidators(APP_VOID);
APP_WORD Validate_Length_Max_256        (LPST_LLDP_TLV lpstTlv);
APP_WORD Validate_Length_Max_255        (LPST_LLDP_TLV lpstTlv);
APP_WORD Validate_End_Of_LLDPDU_TLV     (LPST_LLDP_TLV lpstTlv);
APP_WORD Validate_Chassis_Id_TLV        (LPST_LLDP_TLV lpstTlv);
APP_WORD Validate_Port_Id_TLV           (LPST_LLDP_TLV lpstTlv);
APP_WORD Validate_Ttl_TLV               (LPST_LLDP_TLV lpstTlv);
APP_WORD Validate_Port_Description_TLV  (LPST_LLDP_TLV lpstTlv);
APP_WORD Validate_System_Name_TLV       (LPST_LLDP_TLV lpstTlv);
APP_WORD Validate_System_Description_TLV(LPST_LLDP_TLV lpstTlv);
APP_WORD Validate_System_Capability_TLV (LPST_LLDP_TLV lpstTlv);
APP_WORD Validate_Mgmt_Addr_TLV         (LPST_LLDP_TLV lpstTlv);
APP_WORD Validate_Organization_TLV      (LPST_LLDP_TLV lpstTlv);
APP_WORD ReadPortDescriptionIDTLV       (LPST_LLDP_TLV lpstTlv, APP_LPBYTE *lpbyPortDescription);
APP_WORD ReadChassisIDTLV               (LPST_LLDP_TLV lpstTlv, APP_LPBYTE lpbyChassisIdSubType, APP_LPBYTE lpbyChassisId);
APP_WORD ReadSystemNameTLV              (LPST_LLDP_TLV lpstTlv, APP_LPBYTE *lpbySystemName);
APP_WORD ReadSystemDescTLV              (LPST_LLDP_TLV lpstTlv, APP_LPBYTE *lpbySystemDesc);
APP_WORD ReadSystemCapTLV               (LPST_LLDP_TLV lpstTlv, APP_LPWORD lpwSystemCap,  APP_LPWORD lpwSystemEnabledCap);
APP_WORD IsTlvOUI_PN_8023               (LPST_LLDP_TLV lpstTlv, APP_LPBYTE lpbyOrganizationOUI);
APP_WORD ReadAddrMgtTLV                 (LPST_LLDP_TLV lpstTlv, APP_LPBYTE lpbyMgtAddrSubTypeLen, APP_LPBYTE lpbyMgtAddrSubType, APP_LPBYTE lpbyMgtAddr, APP_LPBYTE lpbyMgtAddrItfSubType, APP_LPDWORD lpdwMgtAddrItfNumber, APP_LPBYTE lpbyMgtAddrOidLength, APP_LPBYTE sMgtAddrOid);
APP_WORD ReadPortIDTLV                  (LPST_LLDP_TLV lpstTlv, APP_LPBYTE lpbyPortIdSubType, APP_LPBYTE lpbyPortId);
APP_WORD ReadTTLTLV                     (LPST_LLDP_TLV lpstTlv, APP_LPWORD lpwRxTTL);
#endif

#endif
