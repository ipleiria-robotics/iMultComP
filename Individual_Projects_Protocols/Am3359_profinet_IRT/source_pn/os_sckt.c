/*
*---------------------------------------------------------------------------*
*  Copyright (C) 2012 Woodhead Software & Electonics. All rights reserved.  *
*---------------------------------------------------------------------------*
*  This program is protected by international copyright.                    *
*                                                                           *
*  The use of this software including but not limited to its Source Code    *
*  is subject to restrictions as agreed in the license agreement between    *
*  you and Woodhead.                                                        *
*  Copying or distribution is not allowed unless expressly permitted        *
*  according to your license agreement with Woodhead.                       *
*---------------------------------------------------------------------------*
*                                                                           *
*  Project             : STACK PROFINET Device                              *
*  Component           : ABSTRACTION SOCKET LAYER                                                   *
* $Workfile:: os_sckt.c                                                    $*
* $Revision:: 1                                                            $*
* $Date:: 13/04/16 17:17                                                   $*
*                                                                           *
*                                                                           */
/****************************************************************************/
/**
* @file os_sckt.c
* @brief Abstraction of the operating socket API
*
* Abstraction of the operating socket API
*
*****************************************************************************/

/* Stack include */

#include <include/os.h>
#include <include/system.h>
#include <include/sockuser.h>
#include <include/stk_sckt.h>
#include <include/os_sckt.h>


#include <ti/ndk/inc/netmain.h>

#include <ti/ndk/inc/stack/inc/vlanif.h>

#define SOCKET_TRACE(...)

APP_BYTE bySockInit = APP_FALSE;
int         gvlan_dev_index = 0;

/****************************************************************************/
/**
* @ingroup PNIO_Socket_API
* @brief Declares, initializes or instantiates all variables used by the socket API.
*
* This function must be used to declare, initialize or instantiate all variables
* used by the socket API.
* This function is the first Socket API function called by the stack.
*
* @retval OS_OK     API socket initialization succeeds. Stack will be able to
*                   use it without problem.
* @retval OS_NOT_OK An error occurred during initialization. socket will not be
*                   able to work correctly.
*
*****************************************************************************/
APP_DWORD OsSockInit(APP_VOID){

  uint8_t       prio_mapping[8];
  uint16_t      src_index;
  uint16_t      vlan_id;
  uint8_t       def_priority;
  uint8_t       index;

  /* First Token: This is the source interface on which the VLAN device is
   * being created */
  src_index = 1;

  vlan_id = 0;

  def_priority = 0;

  /* By default: We configure the priority mapping to be as follows:-
   *  Priority  | VLAN User Priority
   *  -----------------------------------
   *      0     | 0
   *      1     | 1
   *      2     | 2
   *      3     | 3
   *      4     | 4
   *      5     | 5
   *      6     | 6
   *      7     | 7   */
  for (index = 0; index < 8; index++)
    prio_mapping[index] = index;

  /* Use the VLAN API to create a new VLAN device. */
  gvlan_dev_index = VLANAddDevice (src_index, vlan_id, def_priority, prio_mapping);
  if (gvlan_dev_index < 0)
  {
/*    printf("Error: Unable to create a VLAN Device errcode=%d\n", vlan_dev_index);*/
    return OS_NOT_OK;
  }

/*  printf("Successfully created new VLAN Device %d\n", vlan_dev_index);*/

  return OS_OK;
}

/****************************************************************************/
/**
* @ingroup PNIO_Socket_API
* @brief Deallocates the resources used by the socket API.
*
*
* This function must be used to deallocate the resources used by the socket API.
* This call means the socket API functions will no longer be used by stack.
* In case of stack has to reuse this function it will call the @ref OsSockInit
* function first.
*
*
* @retval OS_OK The return value is not used by stack. However, @b OS_OK must be returned.
*
*****************************************************************************/
APP_DWORD OsSockCleanup(APP_VOID){

  if(0 != VLANDelDevice(gvlan_dev_index))
  {
    return OS_NOT_OK;
  }

  fdCloseSession(TaskSelf());
  bySockInit = APP_FALSE;

  return OS_OK;
}

/****************************************************************************/
/**
* @ingroup PNIO_Socket_API
* @brief Creates a socket that is bound to a specific transport service provider.
*
* This function creates a socket that is bound to a specific transport service
* provider. This function allows stack to get unique ID representing a Socket.
*
*
* @param[in,out] lpdwSocket   Pointer on the future socket handle. This pointer
*                             has to be set with a unique socket API. Stack will
*                             use this handle into the other socket API to identify
*                             the specific socket.
* @param[in]     bySocketType The specific transport service: SOCKET_UDP to open a UDP
*                             socket or SOCKET_TCP to open a TCP one.
*
* @retval OS_OK     The open was performed correctly. lpdwSocket has been
*                   correctly set to a unique ID.
* @retval OS_NOT_OK An error occurred during opening the socket.
*
*****************************************************************************/
APP_DWORD OsSockOpen(APP_LPDWORD lpdwSocket, APP_BYTE bySocketType)
{
  /* APP_HANDLE     Sock; */
  SOCKET Sock ;

  if(APP_FALSE == bySockInit)
  {
    bySockInit = APP_TRUE;
    if ( 0 == fdOpenSession(TaskSelf()))
    {
      OS_ASSERT(APP_FALSE);
    }
  }

  if(APP_NULL == lpdwSocket ){
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;

  }

  switch(bySocketType)
  {
   case SOCKET_UDP:

      Sock = socket(AF_INET, SOCK_DGRAM, 0); /* IPPROTO_UDP ??*/
      break;

   case SOCKET_TCP:

      Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      break;

   default:
     OS_ASSERT(APP_FALSE);
     return OS_NOT_OK;
  }

  if(ERROR  == (int)Sock)
  {
    SOCKET_TRACE("Socket Error Code : %d\n", fdError());
    return OS_NOT_OK;
  }

  (*lpdwSocket) = ((APP_DWORD)Sock);

  return OS_OK;
}

/****************************************************************************/
/**
* @ingroup PNIO_Socket_API
* @brief Sets a socket option.
*
* This function sets a socket option. This function is called just after the @ref OsSockOpen.
*
*
* @param[in]  lpdwSocket   Pointer on the socket handle. This pointer was obtained
*                          by stack using the @ref OsSockOpen function.
* @param[in]  bySocketType The specific transport service: SOCKET_UDP to open a UDP
*                          socket or SOCKET_TCP to open a TCP one.
*
* @retval OS_OK     The set socket was performed correctly. Socket option have
*                   been correctly set.
* @retval OS_NOT_OK An error occurred during setting the socket.
*
*****************************************************************************/
APP_DWORD OsSockSetOption(APP_LPDWORD lpdwSocket, APP_BYTE bySocketType)
{
  APP_DWORD  dwOptval = 1;
  APP_DWORD  dwError  = 0;

  if(APP_NULL == lpdwSocket ){
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  /* On this OS, set socket option is done the same way for TCP or UDP */
  dwError = setsockopt ((SOCKET) (*lpdwSocket) , SOL_SOCKET, SO_REUSEADDR, (char*) &dwOptval, sizeof (dwOptval));

  if(0 != dwError)
  {
    return OS_NOT_OK;
  }


  uint16_t priority = 0;
  dwError = setsockopt((SOCKET) (*lpdwSocket), SOL_SOCKET, SO_PRIORITY, &priority, sizeof(priority));
  if(dwError != 0)
  {
    printf ("Set VLAN prio failed: 0x%x\n", fdError());
    return OS_NOT_OK;
  }
  return OS_OK;
}

#ifdef WEB_STAT

/****************************************************************************/
/**
* @ingroup PNIO_Socket_API
* @brief Places a socket in a state in which it is listening for an incoming
* connection.
*
* This function places a socket in a state in which it is listening for an
* incoming connection. The listen function has to specify the backlog for
* incoming connections.
*
* It is used only if WEB_STAT compilation constant is defined.
*
* @param[in]  lpdwSockId Pointer on the socket handle. This pointer was obtained
*                        by stack using the @ref OsSockOpen function.
* @param[in]  wBack      The maximum length of the queue of pending connections.
*
* @retval OS_OK      The listen socket was performed correctly.
* @retval OS_NOT_OK  An error occurred during the listen socket function call.
*
*****************************************************************************/
APP_DWORD OsSockListen(APP_LPDWORD lpdwSockId,APP_WORD wBack)
{
  APP_DWORD dwStatus = 0;

  if(APP_NULL == lpdwSockId )
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  dwStatus = listen( (*lpdwSockId), wBack);

  if(0 == dwStatus)
  {
    return OS_OK;
  }
  else
  {
    return OS_NOT_OK;
  }

}

/****************************************************************************/
/**
* @ingroup PNIO_Socket_API
* @brief Permits an incoming connection attempt on a socket.
*
* This function permits an incoming connection attempt on a socket. In case of
* success a handle for the socket on which the actual connection is made has
* to be returned.
*
* It is used only if WEB_STAT compilation constant is defined.
*
* @param[in]      lpdwSockId    Pointer on the socket handle. This pointer was obtained
*                               by stack using the @ref OsSockOpen function.
* @param[in,out]  lpdwNewSockId Pointer on the new handle. This pointer must be
*                               set by function with a correct unique id. This
*                               id will be used by stack when it called
*                               @ref OsSockReceiveFrom.
*
* @retval OS_OK      The accept was performed correctly. lpdwNewSockId has
*                    been set with a current id.
* @retval OS_NOT_OK  An error occurred during the accept call function.
*
*****************************************************************************/
APP_DWORD OsSockAccept(APP_LPDWORD lpdwSockId, APP_LPDWORD lpdwNewSockId)
{
  SOCKET     Sock;

  if(APP_NULL == lpdwSockId )
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  if(APP_NULL == lpdwNewSockId)
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }
  Sock = accept( (*lpdwSockId), APP_NULL, 0);

  if(ERROR  == Sock)
  {
    return OS_NOT_OK;
  }
  else
  {
    (*lpdwNewSockId) = Sock;
    return OS_OK;
  }
}

#endif /* WEB_STAT */

/****************************************************************************/
/**
* @ingroup PNIO_Socket_API
* @brief Closes an existing socket.
*
* This function closes an existing socket.
*
*
* @param[in]  lpdwSockId   Pointer on the socket handle. This pointer was obtained
*                          by stack using the @ref OsSockOpen function.
* @param[in]  bySocketType The specific transport service: SOCKET_UDP to close a UDP
                           socket or SOCKET_TCP to close a TCP one.
*
* @retval OS_OK     The close was performed correctly. Stack will no longer be
*                   able to use it.
* @retval OS_NOT_OK An error occurred during the close call function.
*
*****************************************************************************/
APP_DWORD OsSockClose(APP_LPDWORD lpdwSockId, APP_BYTE bySocketType )
{
  APP_DWORD dwStatus = 0;

  if(APP_NULL == lpdwSockId )
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  dwStatus = fdClose( (SOCKET)(*lpdwSockId) );

  if(0 == dwStatus)
  {
    return OS_OK;
  }
  else
  {
    return OS_NOT_OK;
  }
}

/****************************************************************************/
/**
* @ingroup PNIO_Socket_API
* @brief Associates a local address with a socket.
*
* The OsSockBind function associates a local address with a socket.
*
*
* @param[in]  lpdwSocket  Pointer on the socket handle. This pointer was obtained by
*                         stack using the @ref OsSockOpen function.
* @param[in]  lpstSockAdr Pointer on a structure. Contains the information needed to bind
*                         the socket.
*                         The values initialized into the structure are:
*                           - stSockAdr.sin_family
*                           - stSockAdr.sin_port
*                           - stSockAdr.sin_addr.s_addr
*
* @retval OS_OK     The bind function succeeds.
* @retval OS_NOT_OK An error occurred during the Bind call function.
*
*****************************************************************************/
APP_DWORD OsSockBind(APP_LPDWORD lpdwSocket ,LPST_OS_SOCKADDR_IN lpstSockAdr)
{
  APP_DWORD dwStatus = 0;
  struct sockaddr_in stSockAdr;

  if(APP_NULL == lpdwSocket )
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  if(APP_NULL == lpstSockAdr )
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  /**** convert sock-address from APP format into platform format ***/
  stSockAdr.sin_family       = lpstSockAdr->sin_family;
  stSockAdr.sin_port         = lpstSockAdr->sin_port;
  /*stSockAdr.sin_addr.s_addr   = lpstSockAdr->sin_addr.S_un.S_addr;*/
  stSockAdr.sin_addr.s_addr   = HTONL(OS_INADDR_ANY);;

  /**** call platform specific send function  ***/
  dwStatus = bind( (SOCKET)(*lpdwSocket) , (struct sockaddr*)&stSockAdr, sizeof (stSockAdr));

  if(0 == dwStatus)
  {
    return OS_OK;
  }
  else
  {
    dwStatus = fdError();
    return OS_NOT_OK;
  }
}

APP_DWORD gdwsockcnt = 0;
/****************************************************************************/
/**
* @ingroup PNIO_Socket_API
* @brief Determines the status of one socket.
*
* The OsSockSelect function determines the status of one socket. This select
* has not to wait.
*
*
* @param[in]  lpdwSockId Pointer on the socket handle. This pointer was obtained
*                        by stack using the @ref OsSockOpen function.
*
* @retval OS_OK     The select function succeeds. Data are awaiting for this socket.
* @retval OS_NOT_OK An error occurred during the select call function, or no data
*                   found for this socket.
*
*****************************************************************************/
APP_DWORD OsSockSelect(APP_LPDWORD lpdwSockId)
{

  APP_DWORD       dwStatus = 0;
  struct timeval  stTimeout;
  NDK_fd_set          FdReadList;

  if(APP_NULL == lpdwSockId )
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  NDK_FD_ZERO (&FdReadList);
  NDK_FD_SET ( (SOCKET)(*lpdwSockId), &FdReadList);

  stTimeout.tv_usec = 0;    /* waiting time in microseconds*/
  stTimeout.tv_sec  = 0;    /* waiting time in seconds*/
  dwStatus = fdSelect (NDK_FD_SETSIZE, &FdReadList, (NDK_fd_set*) 0, (NDK_fd_set*) 0, &stTimeout);

  if(ERROR == (int)dwStatus)
  {
    return OS_NOT_OK;
  }

  if (!NDK_FD_ISSET ( (SOCKET)(*lpdwSockId), &FdReadList))
  {
    if ((*lpdwSockId) == 0x8021bdc4) {
    gdwsockcnt++;;
    }
    return OS_NOT_OK;
  }

  if ((*lpdwSockId) == 0x8021bdc4) {
    return OS_OK;
  }
  return OS_OK;
}

/****************************************************************************/
/**
* @ingroup PNIO_Socket_API
* @brief Receives a datagram and stores the source address.
*
* The OsSockReceiveFrom function receives a datagram and stores the source address.
*
*
* @param[in]      lpdwSockId  Pointer on the socket handle. This pointer was obtained
*                             by stack using the @ref OsSockOpen function.
* @param[out]     pbyBuf      Buffer to store the incoming data.
* @param[in,out]  lpwLen      The lpbyBuf Buffer length. This value has to be
*                             updated with the size of received data.
* @param[out]     lpstSockAdr Structure to store information about the source address
*                             (sin_family, sin_port, sin_addr.S_un.S_addr).
*
* @return If the OsSockReceiveFrom succeed:
*           - The lpstSockAdr must be correctly set (sin_family, sin_port, sin_addr.S_un.S_addr) .
*           - lpwLen must contain the length of received data.
*
* @retval OS_OK      The OsSockReceiveFrom function succeeds.
* @retval OS_NOT_OK  An error occurred during the OsSockReceiveFrom function call.
*
*****************************************************************************/
APP_DWORD OsSockReceiveFrom(APP_LPDWORD lpdwSockId, APP_LPBYTE pbyBuf, APP_LPWORD lpwLen, LPST_OS_SOCKADDR_IN lpstSockAdr)
{
  int  shSockAdrLen   = 0;
  APP_DWORD  dwDataLen     = 0;
  struct sockaddr_in  stSockAdr = {0};

  if(APP_NULL == lpdwSockId )
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  if(APP_NULL == pbyBuf )
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  if(APP_NULL == lpwLen )
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  if(APP_NULL == lpstSockAdr )
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  shSockAdrLen = sizeof(stSockAdr);

  dwDataLen = recvfrom ( (SOCKET)(*lpdwSockId),           /* Socket handle               */
                       pbyBuf,                          /* pointer to receive buffer  */
                       *lpwLen,                          /* length of receive buffer   */
                       0,
                       (struct sockaddr*)&stSockAdr,    /* remote address (UDP sender)*/
                       (int *)&shSockAdrLen);         /* length of remote address    */

  if( ERROR  == (int)dwDataLen)
  {
    return OS_NOT_OK;
  }

  if(dwDataLen <= (*lpwLen))
  {
    *lpwLen = (APP_WORD)dwDataLen;
    lpstSockAdr->sin_family           =stSockAdr.sin_family;
    lpstSockAdr->sin_port             =stSockAdr.sin_port;
    lpstSockAdr->sin_addr.S_un.S_addr =stSockAdr.sin_addr.s_addr;
    return OS_OK;
  }

  return OS_NOT_OK;
}

/****************************************************************************/
/**
* @ingroup PNIO_Socket_API
* @brief Sends data to a specific destination.
*
* The OsSockSendTo function sends data to a specific destination.
*
*
* @param[in]  lpdwSockId  Pointer on the socket handle. This pointer was obtained by stack using the @ref OsSockOpen function.
* @param[in]  lpbyBuf     A buffer containing the data to be transmitted.
* @param[in]  dwLen       The lentgh (in byte) of lpbyBuf.
* @param[in]  dwFlags     An indicator specifying the way in which the call is made.
* @param[in]  lpstSockAdr Structure used to described parameters necessary to send
*                         data from this socket (sin_family, sin_port, sin_addr.S_un.S_addr)
*
* @retval OS_OK      The OsSockSendTo function succeeds.
* @retval OS_NOT_OK  An error occurred during the OsSockSendTo function call.
*
*****************************************************************************/
APP_DWORD OsSockSendTo(APP_LPDWORD lpdwSockId,  APP_LPBYTE lpbyBuf,  APP_DWORD dwLen, APP_DWORD dwFlags,LPST_OS_SOCKADDR_IN lpstSockAdr)
{
  APP_DWORD           dwStatus = 0;
  struct sockaddr_in  stSockAdr;

  stSockAdr.sin_family       = lpstSockAdr->sin_family;
  stSockAdr.sin_port         = lpstSockAdr->sin_port;
  stSockAdr.sin_addr.s_addr  = lpstSockAdr->sin_addr.S_un.S_addr;

  dwStatus = sendto( (SOCKET)(*lpdwSockId),         /* SOCKET                           */
             lpbyBuf,                        /* LPWSABUF lpBuffers               */
             dwLen,                          /* LPDWORD lpNumberOfBytesSent      */
             dwFlags,                        /* DWORD dwFlags                    */
             (struct sockaddr*)&stSockAdr,   /* const struct sockaddr FAR *lpTo  */
             sizeof (stSockAdr));            /* LSA_INT32 iToLen                 */

  if(ERROR == (int)dwStatus)
  {
    return OS_NOT_OK;
  }
  else
  {
    return OS_OK;
  }
}

#ifdef WEB_STAT

/****************************************************************************/
/**
* @ingroup PNIO_Socket_API
* @brief Sends data on a connected socket.
*
* The OsSockSend function sends data on a connected socket.
*
* It is used only if WEB_STAT compilation constant is defined.
*
* @param[in]  lpdwSockId Pointer on the socket handle. This pointer was obtained
*                        by stack using the @ref OsSockOpen function.
* @param[in]  lpbyBuf    A buffer containing the data to be transmitted.
* @param[in]  dwLen      The lentgh (in byte) of lpbyBuf.
* @param[in]  dwFlags    Indicator specifying the way in which the call is made.
*
* @retval OS_OK      The OsSockSend function succeeds.
* @retval OS_NOT_OK  An error occurred during the OsSockSend function call.
*
*****************************************************************************/
APP_DWORD OsSockSend(APP_LPDWORD lpdwSockId, APP_LPBYTE lpbyBuf, APP_DWORD dwLen, APP_DWORD dwFlags){

  APP_DWORD dwStatus = 0;

    dwStatus = send( (*lpdwSockId), (caddr_t)lpbyBuf, dwLen, dwFlags);

    if(ERROR == dwStatus)
    {
      return OS_NOT_OK;
    }
    else
    {
      return OS_OK;
    }
}
#endif

