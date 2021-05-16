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
/* $Workfile:: hwif.h                                                       $*/
/* $Revision:: 12                                                           $*/
/* $Date:: 17/10/16 9:43                                                    $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                            */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#ifndef _HWIF_H
#define _HWIF_H

APP_HANDLE  HW_OpenRead            ( APP_HANDLE lpszInterfaceName ,APP_DWORD   dwInterfaceNumber );
APP_HANDLE  HW_OpenWrite           ( APP_HANDLE lpszInterfaceName ,APP_DWORD   dwInterfaceNumber );
APP_BOOL    HW_Close               ( APP_HANDLE DeviceHandleRead  ,APP_HANDLE  DeviceHandleWrite );
APP_BOOL    HW_SendData            ( APP_LPVOID pData             ,APP_WORD    wLen        ,  APP_HANDLE DeviceHandle );
APP_BOOL    HW_SendDataSpecificPort( APP_LPVOID pData             ,APP_WORD    wLen        ,  APP_DWORD  dwPort,  APP_HANDLE DeviceHandle);
APP_BOOL    HW_ReceiveDataPort     ( APP_LPBYTE pBuffer           ,APP_LPWORD  pLength     ,  APP_LPWORD lpwPort, APP_HANDLE DeviceHandle);
APP_BOOL    HW_GetMacAddr          ( APP_LPBYTE pMacAddr          ,APP_HANDLE  DeviceHandle );
APP_BOOL    HW_GetMacAddrPort      ( APP_LPBYTE pSrcMacAddr       ,APP_BYTE    byPortNumber, APP_HANDLE  DeviceHandle );
APP_BOOL    HW_GetCapabilitiesPort ( APP_BYTE byPortNumber        ,APP_LPWORD  lpwMAUTypeCapabilities, APP_LPBOOL lpbAutoNegSupported);
APP_VOID    HW_SetSeletedDevice    ( APP_DWORD  dwCombo);
APP_DWORD   HW_GetSeletedDevice    ();
APP_DWORD   HW_getInterfaceName    ( APP_LPCHAR lpszInterfaceName, APP_LPCHAR szItfName);
APP_BOOL    HW_CancelRead          ( APP_HANDLE  Handle);

#endif

