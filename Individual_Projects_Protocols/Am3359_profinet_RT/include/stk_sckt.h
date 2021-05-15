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
/*  Component           : Mutex manager                                     */
/* $Workfile:: stk_sckt.h                                                    $*/
/* $Revision:: 8                                                            $*/
/* $Date:: 29/07/14 15:50                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*     abstraction layer for Mutex                                          */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef __STK_SCKT_H__
#define __STK_SCKT_H__

/* Specific defines for pool management*/
#define MAXNUM_SOCKETS     (12)
#define PND_SCKT_FREE     0
#define PND_SCKT_USED     1

typedef struct _ST_APP_IP_ADDR
{
  union
  {
    struct { APP_BYTE  s_b1,s_b2,s_b3,s_b4; } S_un_b;
    struct { APP_WORD  s_w1,s_w2; } S_un_w;
    APP_DWORD S_addr;
  } S_un;
} ST_APP_IP_ADDR ;

typedef struct _ST_OS_SOCKADDR_IN
{
  APP_WORD        sin_family;
  APP_WORD        sin_port;
  ST_APP_IP_ADDR  sin_addr;
  APP_BYTE        sin_zero[8];
}ST_OS_SOCKADDR_IN, APP_FAR * LPST_OS_SOCKADDR_IN;


APP_DWORD StackSockInit       (APP_VOID);
APP_DWORD StackSockCleanup    (APP_VOID);
APP_DWORD StackSockOpen       (APP_LPDWORD  SockId, APP_BYTE bySocketType);
APP_DWORD StackSockClose      (APP_DWORD SockId, APP_BYTE bySocketType);
APP_DWORD StackSockBind       (APP_DWORD SockId, LPST_OS_SOCKADDR_IN pSockAdr, APP_DWORD pSockAdrLen);
APP_DWORD StackSockReceiveFrom(APP_DWORD dwSockId,APP_LPBYTE pbyBuf,APP_LPWORD pwLen,LPST_OS_SOCKADDR_IN pSockAdr);
APP_DWORD StackSockSendTo     (APP_DWORD SockId,  APP_LPBYTE pBuf,  APP_DWORD len, APP_DWORD flags,LPST_OS_SOCKADDR_IN pSockAdr,  APP_DWORD SockAdrLen);

#ifdef WEB_STAT
APP_DWORD StackSockSend       (APP_DWORD dwSockId,  APP_LPBYTE lpBuf,  APP_DWORD dwLen, APP_DWORD dwFlags);
APP_DWORD StackSockListen     (APP_DWORD dwSockId,APP_WORD wBack);
APP_DWORD StackSockAccept     (APP_DWORD dwSockId,APP_WORD wBack);
#endif

/* Use to inform if Socket is UDP or TCP one*/
#define SOCKET_UDP 0
#define SOCKET_TCP 1

#endif
