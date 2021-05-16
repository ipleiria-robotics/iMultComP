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
/*  Component           : NetWork Interface                                 */
/* $Workfile:: os_sckt.h                                                    $*/
/* $Revision:: 1                                                            $*/
/* $Date:: 13/04/16 17:17                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*  Socket Abstraction Layer                                                 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef _SOCKET_H
#define _SOCKET_H

#define OS_AFINET      AF_INET
#define OS_INADDR_ANY  INADDR_ANY

APP_DWORD OsSockInit       (APP_VOID);
APP_DWORD OsSockCleanup    (APP_VOID);
APP_DWORD OsSockBind       (APP_LPDWORD lpdwSocket ,LPST_OS_SOCKADDR_IN pSockAdr);
APP_DWORD OsSockListen     (APP_LPDWORD lpdwSockId, APP_WORD wBack);
APP_DWORD OsSockReceiveFrom(APP_LPDWORD lpdwSockId, APP_LPBYTE pbyBuf, APP_LPWORD pwLen, LPST_OS_SOCKADDR_IN pSockAdr);
APP_DWORD OsSockSendTo     (APP_LPDWORD lpdwSockId, APP_LPBYTE pBuf, APP_DWORD len, APP_DWORD flags, LPST_OS_SOCKADDR_IN pSockAdr);
APP_DWORD OsSockOpen       (APP_LPDWORD lpdwSocket, APP_BYTE dwSocketType );
APP_DWORD OsSockSetOption  (APP_LPDWORD lpdwSocket, APP_BYTE dwSocketType );
APP_DWORD OsSockClose      (APP_LPDWORD lpdwSocket, APP_BYTE bySocketType );
APP_DWORD OsSockSelect     (APP_LPDWORD lpdwSockId);

#ifdef WEB_STAT
APP_DWORD OsSockAccept     (APP_LPDWORD lpdwSockId, APP_LPDWORD lpdwNewSockId);
APP_DWORD OsSockSend       (APP_LPDWORD lpdwSockId, APP_LPBYTE lpbyBuf, APP_DWORD dwLen, APP_DWORD dwFlags);
#endif
#endif
