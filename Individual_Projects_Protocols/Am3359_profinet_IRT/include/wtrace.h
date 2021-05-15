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
/*  Component           :                                                    */
/* $Workfile:: wtrace.h                                                     $*/
/* $Revision:: 1                                                            $*/
/* $Date:: 13/04/16 17:17                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef __TRACE_H__
#define __TRACE_H__

#ifdef OS_DEBUG

/* printf(__VA_ARGS__) */
#define FSPMDEV_TRACE(...)
#define PROD_FSPM_TRACE(...)
#define CPM_TRACE(...)
#define PPM_TRACE(...)
#define ALPMI_TRACE(...)
#define ALPMR_TRACE(...)
#define APMR_TRACE(...)
#define APMS_TRACE(...)
#define RPC_CLIENT_TRACE(...)
#define RPC_SERVER_TRACE(...)
#define DCP_TRACE(...)
#define OS_TRACE(...)
#define CONFIG_TRACE(...)
#define LMPM_TRACE(...)
#define STTUP_TRACE(...)
#define CALLBACK_TRACE(...)
#define HASH_TRACE(...)
#define ASE_TRACE(...)
#define IO_TRACE(...)
#define SCHEDULE_TRACE(...)
#define LLDP_TRACE(...)
#define LLDP_CB_TRACE(...)
#define LLDP_RX_TRACE(...)
#define LLDP_TX_TRACE(...)
#define CMDEV_TRACE(...)
#define CMRPC_TRACE(...)
#define CMRDR_TRACE(...)
#define CMWRR_TRACE(...)
#define CMPBE_TRACE(...)
#define CMSM_TRACE(...)
#define CMDEV_DA_TRACE(...)
#define CMINA_TRACE(...)
#define CMIO_TRACE(...)
#define CMDMC_TRACE(...)
#define CMSU_TRACE(...)
#define RSMSM_TRACE(...)
#define PLUGSM_TRACE(...)
#define PULLSM_TRACE(...)
#define OWNSM_TRACE(...)
#define PROFIENERGY_TRACE(...)
#define REDUNDANCY_TRACE(...)
#define HWACC_TRACE(...) /*{ char bufftoto[512]={0}; OS_SPRINTF(bufftoto, __VA_ARGS__); UARTPutString(uartInstance, bufftoto);}*/
#define PSM_TRACE(...) /*{ char bufftoto[512]={0}; OS_SPRINTF(bufftoto, __VA_ARGS__); UARTPutString(uartInstance, bufftoto);}*/

#else

#define PROFIENERGY_TRACE(...)
#define FSPMDEV_TRACE(...)
#define CPM_TRACE(...)
#define PPM_TRACE(...)
#define ALPMR_TRACE(...)
#define ALPMI_TRACE(...)
#define APMR_TRACE(...)
#define APMS_TRACE(...)
#define ALM_TRACE(...)
#define PROD_FSPM_TRACE(...)
#define DCP_TRACE(...)
#define OS_TRACE(...)
#define RPC_CLIENT_TRACE(...)
#define RPC_SERVER_TRACE(...)
#define CONFIG_TRACE(...)
#define STTUP_TRACE(...)
#define CALLBACK_TRACE(...)
#define LMPM_TRACE(...)
#define HASH_TRACE(...)
#define ASE_TRACE(...)
#define IO_TRACE(...)
#define LLDP_TRACE(...)
#define LLDP_CB_TRACE(...)
#define LLDP_RX_TRACE(...)
#define LLDP_TX_TRACE(...)
#define CMDEV_TRACE(...)
#define CMRPC_TRACE(...)
#define CMRDR_TRACE(...)
#define CMWRR_TRACE(...)
#define CMPBE_TRACE(...)
#define CMSM_TRACE(...)
#define CMDEV_DA_TRACE(...)
#define CMINA_TRACE(...)
#define CMIO_TRACE(...)
#define CMDMC_TRACE(...)
#define CMSU_TRACE(...)
#define RPC_TRAME_TRACE(...)
#define OWNSM_TRACE(...)
#define PLUGSM_TRACE(...)
#define PULLSM_TRACE(...)
#define RSMSM_TRACE(...)
#define REDUNDANCY_TRACE(...)
#define HWACC_TRACE(...) /*{ char bufftoto[512]={0}; OS_SPRINTF(bufftoto, __VA_ARGS__); UARTPutString(uartInstance, bufftoto);}*/
#define PSM_TRACE(...) /*{ char bufftoto[512]={0}; OS_SPRINTF(bufftoto, __VA_ARGS__);  LOG_printf(uartInstance, bufftoto);}*/
#endif
#endif


