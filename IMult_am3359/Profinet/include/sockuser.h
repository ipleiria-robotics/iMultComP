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
/*  Component           : Hardware Interface                                 */
/* $Workfile:: sockuser.h                                                   $*/
/* $Revision:: 1                                                            $*/
/* $Date:: 13/04/16 17:17                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*  Network Abstraction Layer                                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#ifndef _SOCKUSER_H
#define _SOCKUSER_H

#include "stk_sckt.h"

/*
#define EWOULDBLOCK             0
#define EINPROGRESS             1
#define EALREADY                2
#define ENOTSOCK                3
#define EDESTADDRREQ            4
#define EMSGSIZE                5
#define EPROTOTYPE              6
#define ENOPROTOOPT             7
#define EPROTONOSUPPORT         8
#define ESOCKTNOSUPPORT         9
#define EOPNOTSUPP              10
#define EPFNOSUPPORT            11
#define EAFNOSUPPORT            12
#define EADDRINUSE              13
#define EADDRNOTAVAIL           14
#define ENETDOWN                15
#define ENETUNREACH             16
#define ENETRESET               17
#define ECONNABORTED            18
#define ECONNRESET              19
#define ENOBUFS                 20
#define EISCONN                 21
#define ENOTCONN                22
#define ESHUTDOWN               23
#define ETOOMANYREFS            24
#define ETIMEDOUT               25
#define ECONNREFUSED            26
#define ELOOP                   27
#define ENAMETOOLONG            28
#define EHOSTDOWN               29
#define EHOSTUNREACH            30
#define ENOTEMPTY               31
#define EPROCLIM                32
#define EUSERS                  33
#define EDQUOT                  34
#define ESTALE                  35
#define EREMOTE                 36

#define EINVAL            37
#define EFAULT            38
*/

/* Specific defines*/

/*#define FREE            0
#define USED            1
*/

#define ntohs     NDK_ntohs
#define ntohl     NDK_ntohl
#define htons     NDK_htons
#define htonl     NDK_htonl

typedef struct
{
  APP_DWORD used;        /* occupation tag: socket is used/free*/
  APP_DWORD PendingCnt;  /* USED: receive request pending, FREE: no requ. pending*/
  APP_DWORD SockHndlRd;  /* Socket Handle, returned by the operating system*/
  APP_DWORD ThreadId;    /* ID of the socket - service thread*/
  APP_WORD  Port;        /* UDP Port where the socket listen */
} SOCK_PROP;

typedef struct
{
   SOCK_PROP  Sock[MAXNUM_SOCKETS];
}  SOCK_MANAGER;

#endif
