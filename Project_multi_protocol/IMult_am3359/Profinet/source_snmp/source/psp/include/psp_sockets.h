/***************************************************************************
 *
 *            Copyright (c) 2016-2018 by HCC Embedded
 *
 * This software is copyrighted by and is the sole property of
 * HCC.  All rights, title, ownership, or other interests
 * in the software remain the property of HCC.  This
 * software may only be used in accordance with the corresponding
 * license agreement.  Any unauthorized use, duplication, transmission,
 * distribution, or disclosure of this software is expressly forbidden.
 *
 * This Copyright notice may not be removed or modified without prior
 * written consent of HCC.
 *
 * HCC reserves the right to modify this software without notice.
 *
 * HCC Embedded
 * Budapest 1133
 * Vaci ut 76
 * Hungary
 *
 * Tel:  +36 (1) 450 1302
 * Fax:  +36 (1) 450 1303
 * http: www.hcc-embedded.com
 * email: info@hcc-embedded.com
 *
 ***************************************************************************/
#ifndef _PSP_SOCKET_H
#define _PSP_SOCKET_H
#include "psp_types.h"

#include "../../api/api_ip.h"
#include <ti/ndk/inc/netmain.h>

#include "../../version/ver_psp_sockets.h"
#if ( VER_PSP_SOCKETS_MAJOR != 1 ) || ( VER_PSP_SOCKETS_MINOR != 5 )
 #error Incompatible PSP_SOCKET version number
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Socket supported feutures */
#define PSP_SC_SELECT_SUPP        1U /* Set to 1 if select() is supported. */
#define PSP_SC_POLL_SUPP          1U /* Set to 1 if poll() is supported. */
#define PSP_SC_AF_INET6_SUPP      0U /* Set to 1 if sockets support IPv6 protocol. */
#define PSP_SC_SOCKOPT_SUPP       1U /* Set to 1 if setsockopt()/getsockopt() is supported. */
#define PSP_SC_SO_PRIORITY_SUPP   1U /* Set to 1 if option SO_PRIORITY is supported. */
#define PSP_SC_SO_SNDTIMEO_SUPP   1U /* Set to 1 if option SO_SNDTIMEO is supported. */
#define PSP_SC_SO_RCVTIMEO_SUPP   1U /* Set to 1 if option SO_RCVTIMEO is supported. */
#define PSP_SC_SO_LINGER_SUPP     1U /* Set to 1 if option SO_LINGER is supported. */
#define PSP_SC_SO_NONBLOCK_SUPP   1U /* Set to 1 if option SO_NBIO is supported. */
#define PSP_SC_IP_TTL_OPT_SUPP    0U /* Set to 1 if option IP_TTL_OPT is supported. */
#define PSP_SC_IP_TOS_SUPP        0U /* Set to 1 if option IP_TOS is supported. */
#define PSP_SC_IPV6_ONLY_SUPP     0U /* Set to 1 if option IPV6_ONLY is supported. */
#define PSP_SC_SO_BROADCAST_SUPP  0U /* Set to 1 if option SO_BROADCAST is supported. */
#define PSP_SC_IOCTL_SUPP         0U /* Set to 1 if ioctl() command is supported. */
#define PSP_SC_IO_FIONBIO_SUPP    0U /* Set to 1 if option FIONBIO is supported. */
#define PSP_SC_IO_FIONREAD_SUPP   0U /* Set to 1 if option FIONREAD is supported. */
#define PSP_SC_GETHOSTBYNAME_SUPP 0U /* Set to 1 if function gethostbyname() is supported. */
#define PSP_SC_GETHOSTBYADDR_SUPP 0U /* Set to 1 if function gethostbyaddr() is supported. */
#define PSP_SC_ERRNO_SUPP         1U /* Set to 1 if last error number is supported. */
#define PSP_SC_STATS_SUPP         0U /* Set to 1 if tcp stack statistics are supported. */
#define PSP_SC_ADDR_CONV_SUPP     ( SOCK_INET_NTOA && IP_V4_ENABLE ) /* Set to 1 if platform supports conversion of IP address to/from ASCII string. */

/* Socket defines */
#define PSP_SC_SOCKET_ERROR     INVALID_SOCKET
#define PSP_SC_SOCKET_SUCCESS   0

#define PSP_SC_AF_INET          AF_INET
#define PSP_SC_AF_INET6         AF_INET6

#define PSP_SC_SOCK_STREAM      SOCK_STREAM
#define PSP_SC_SOCK_DGRAM       SOCK_DGRAM

#define PSP_SC_IN_ADDR_ANY      IN_ADDR_ANY


#define PSP_SC_SOL_SOCKET       SOL_SOCKET
#define PSP_SC_IPPROTO          IPPROTO_IP
/* Options */
#define PSP_SC_SO_SNDTIMEO        SO_SNDTIMEO
#define PSP_SC_SO_RCVTIMEO        SO_RCVTIMEO
#define PSP_SC_SO_PRIORITY        SO_PRIORITY
#define PSP_SC_SO_LINGER          SO_LINGER
#define PSP_SC_SO_NONBLOCK        SO_BLOCKING

#define PSP_SC_IP_ADD_MEMBERSHIP  IP_ADD_MEMBERSHIP
#define PSP_SC_IP_DROP_MEMBERSHIP IP_DROP_MEMBERSHIP


/* Socket error codes */
#define PSP_SC_ERR_EBADF          EBADF
#define PSP_SC_ERR_ECONNABORTED   ECONNABORTED
#define PSP_SC_ERR_ECONNRESET     ECONNRESET
#define PSP_SC_ERR_EFAULT         0x7FFFFFFE
#define PSP_SC_ERR_EINVAL         EINVAL
#define PSP_SC_ERR_ENETDOWN       ENETDOWN
#define PSP_SC_ERR_ENOTCONN       ENOTCONN
#define PSP_SC_ERR_ETIMEDOUT      ETIMEDOUT
#define PSP_SC_ERR_EAGAIN         EAGAIN
#define PSP_SC_ERR_ENOMEM         ENOMEM
#define PSP_SC_ERR_EPERM          0x7FFFFFFE
#define PSP_SC_ERR_EALREADY       EALREADY
#define PSP_SC_ERR_ENOTFOUND      0x7FFFFFFE
#define PSP_SC_ERR_EWOULDBLOCK    EWOULDBLOCK
#define PSP_SC_ERR_ENOBUFS        ENOBUFS
#define PSP_SC_ERR_EINPROGRESS    EINPROGRESS

/* poll function flags defines */
#define PSP_SC_POLLIN     POLLIN
#define PSP_SC_POLLPRI    POLLPRI
#define PSP_SC_POLLOUT    POLLOUT
#define PSP_SC_POLLERR    POLLERR
#define PSP_SC_POLLHUP    POLLHUP
#define PSP_SC_POLLNVAL   POLLNVAL
/************************************************************************
* TYPEDEFS
*************************************************************************/
typedef SOCKET  t_psp_socket;

typedef int t_psp_sc_socklen;

/* argument type for psp_sc_setsockopt */
typedef unsigned long t_psp_sc_opt_arg;

/* return type value for  sockets */
typedef int t_psp_sc_ret_val;

/* send/receive buffer length type */
typedef int t_psp_sc_io_len;

/* socket errror type */
typedef int t_psp_sc_error;

/* address structures */
typedef struct sockaddr t_psp_sc_sockaddr;
typedef struct sockaddr_in t_psp_sc_sockaddr_in;

/* poll function argument */
typedef FDPOLLITEM t_psp_sc_pollfd;

/* select function argument */
typedef fd_set t_psp_sc_fdset;
typedef struct timeval t_psp_sc_timeval;

typedef struct linger t_psp_sc_linger;

typedef struct ip_mreq t_psp_sc_ip_mreq;

/*****************************************************************************
 * Function wrappers 
 ****************************************************************************/
#define psp_sc_select(nfds,rfd,wfd,efd,tm) fdSelect((nfds),(rfd),(wfd),(efd),(tm))
#define PSP_SC_FD_ISSET( fd, fdset )     NDK_FD_ISSET( fd, fdset )
#define PSP_SC_FD_SET( fd, fdset )       NDK_FD_SET( fd, fdset )
#define PSP_SC_FD_CLR( fd, fdset )       NDK_FD_CLR( fd, fdset )
#define PSP_SC_FD_ZERO( fdset )          NDK_FD_ZERO( fdset )

#define psp_sc_poll(pollfd,cnt,tm) fdpoll( (pollfd),(cnt),(tm))

#define psp_sc_htons(a)                   htons(a)
#define psp_sc_htonl(a)                   htonl(a)
#define psp_sc_ntohs(a)                   ntohs(a)
#define psp_sc_ntohl(a)                   ntohl(a)

#define psp_sc_bind(s, addr, addrlen)     NDK_bind((s), (addr), (addrlen))
#define psp_sc_connect(s, addr, addrlen)  NDK_connect((s), (addr), (addrlen))
#define psp_sc_listen(s, backlog)         NDK_listen((s), (backlog))
#define psp_sc_accept(s, addr, addrlen)   NDK_accept((s), (addr), (addrlen))
#define psp_sc_recv(s, buf, len, flags)   NDK_recv((s), (buf), (t_psp_sc_io_len)(len), (flags))
#define psp_sc_recvfrom(s, buf, len, flags, addr, asize)  \
        NDK_recvfrom((s), (buf), (t_psp_sc_io_len)(len), (flags), (addr), (asize) )

#define psp_sc_send(s, buf, len, flags)   NDK_send((s), (buf), (t_psp_sc_io_len)(len), (flags))
#define psp_sc_sendto(s, msg, len, flags, to, tolen) \
        NDK_sendto((s), (msg), (t_psp_sc_io_len)(len), (flags), (to), tolen)

#define psp_sc_socket(dom, type, proto)   NDK_socket((dom), (type), proto)
#define psp_sc_close(s)                   NDK_shutdown((s),SHUT_RDWR)
#define psp_sc_setsockopt(s,l,n,o,ol)     NDK_setsockopt((s),(l),(n),(o),(ol))
#define psp_sc_getsockopt(s,l,n,o,ol)     NDK_getsockopt((s),(l),(n),(o),(ol))

int psp_sc_geterrno( t_psp_socket s, int * p_err);

#if PSP_SC_GETHOSTBYNAME_SUPP
 #define psp_sc_gethostbyname(name)       socket_gethostbyname(name)
#endif

#if PSP_SC_GETHOSTBYADDR_SUPP
 #define psp_sc_gethostbyaddr(addr)       socket_gethostbyaddr(addr)
#endif

#if PSP_SC_ADDR_CONV_SUPP
 #define psp_sc_inet_aton(ascii,addr)     socket_inet_aton(ascii,addr)
 #define psp_sc_inet_ntoa(addr)           socket_inet_ntoa(addr)
#endif

#define psp_sc_ioctl(sck,opt,val)         socket_ioctl((sck),(opt),(val))

#define psp_socket_init()    0U
#define psp_socket_start()   0U
#define psp_socket_stop()    0U
#define psp_socket_delete()  0U

#define psp_sc_remote_mss(...)  (PSP_SC_SOCKET_ERROR)

/* This functions are not defined as their functionality is realised by HCC IPStack
 and this wrapper is used only with IP stack
psp_sc_set_ip_config
psp_sc_get_ip_config
psp_sc_set_ifc_config
psp_sc_get_ifc
*/


#ifdef __cplusplus
}
#endif

#endif /* ifndef _PSP_SOCKET_H */

