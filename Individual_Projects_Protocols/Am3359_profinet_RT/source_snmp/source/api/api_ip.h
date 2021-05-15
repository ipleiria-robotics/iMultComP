/***************************************************************************
 *
 *            Copyright (c) 2017-2018 by HCC Embedded
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
/***************************************************************************
 * This a fake IP API file. It used by Socket Adaptor to have access to IP
 * structures.
 ***************************************************************************/


#ifndef _API_IP_H
#define _API_IP_H


/************************************************************************
*  Include section
*************************************************************************/
#include "../psp/include/psp_types.h"
#include "../config/config_ip.h"

#include "../version/ver_ip.h"
#if ( VER_IP_MAJOR != 6 ) || ( VER_IP_MINOR != 11 )
 #error Incompatible IP version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************
*  Defines section
*************************************************************************/

/* error codes */
#define IP_SUCCESS                0
#define IP_MORE_DATA              1
#define IP_CONN_PENDING           2
#define IP_DISCONNECT_WAIT        3
#define IP_ERROR                  4
#define IP_ERR_OS                 5
#define IP_ERR_INIT               6
#define IP_ERR_NWDRV              7
#define IP_ERR_NOT_READY          8
#define IP_ERR_NOT_CONFIGURED     9
#define IP_ERR_NO_DATA            10
#define IP_ERR_NO_MORE_ENTRY      11
#define IP_ERR_NO_CONNECTION      12
#define IP_ERR_NO_BUFFER          13
#define IP_ERR_INVALID_PARAM      14
#define IP_ERR_INVALID_HDL        15
#define IP_ERR_INVALID_FRAME      16
#define IP_ERR_INVALID_PROTOCOL   17
#define IP_ERR_INVALID_SIZE       18
#define IP_ERR_INVALID_REQUEST    19
#define IP_ERR_INVALID_STATE      20
#define IP_ERR_INVALID_CONFIG     21
#define IP_ERR_INVALID_BUFFER     22
#define IP_ERR_INVALID_POOL       23
#define IP_ERR_POOL_BUSY          24
#define IP_ERR_ROUTE              25
#define IP_ERR_LINK_DOWN          26
#define IP_ERR_PORT_OPENED        27
#define IP_ERR_BAD_CHECKSUM       28
#define IP_ERR_DUP_FRAGMENT       29
#define IP_ERR_TASK_NOT_FOUND     30
#define IP_ERR_TIMEOUT            31
#define IP_PKT_SENT               32

/********** IP address size **********/
#define IP_V4_ADDR_SIZE           4u
#define IP_V6_ADDR_SIZE           16u

/* Invalid handles */
#define IP_INVALID_IFC_HDL        UINT8_MAX
#define IP_INVALID_ROUTE_HDL      UINT32_MAX

/* set par_req_len to IP_MTU_SIZE to get the max. buffer */
/* size when ..._get_buf is called */
#define IP_MTU_SIZE               0u

/* minimum buffer size */
#define IP_MIN_BUF_SIZE           64u

/* route states */
#define IP_STATE_STOPPED          0u
#define IP_STATE_LINK_DOWN        1u
#define IP_STATE_LINK_UP          2u
#define IP_STATE_OFFLINE          3u
#define IP_STATE_LINK_MASK        3u

#define IP_V4_STATE_CONFIGURED    4u
#define IP_V6_STATE_CONFIGURED    8u

/* timeout values */
#define IP_WAIT_NONE              0u
#define IP_WAIT_FOREVER           UINT32_MAX

/* notifications */
#define IP_NTF_BUF_AVAIL          0x00000001u
#define IP_NTF_TX_RDY             0x00000002u
#define IP_NTF_TX_ERR             0x00000004u
#define IP_NTF_RX_RDY             0x00000008u
#define IP_NTF_CON_ESTD           0x00000010u
#define IP_NTF_CON_CLOSE          0x00000020u
#define IP_NTF_CON_ABORT          0x00000040u
#define IP_NTF_DCON_WAIT          0x00000080u
#define IP_NTF_PORT_CLOSE         0x00000100u
#define IP_NTF_PORT_ABORT         0x00000200u

#define IP_NTF_CONFIG_CHANGE      0x00010000u

/* IP configuration options */
#define IP_IFC_OPT_DHCP_ENABLE    1u      /* enable DHCP for this interface */
#define IP_IFC_OPT_DEF_GATEWAY    2u      /* set this interface to be the default gateway */
#define IP_V6_IFC_OPT_DHCP_ENABLE 4u      /* enable DHCP_V6 for this interface */
#define IP_V6_IFC_OPT_DEF_GATEWAY 8u      /* set this interface to be the default gateway */

/* flags indicating which configuratioon should be set */
#define IP_CONFIGURE_IP_V4        1u    /* IP v4 configuration*/
#define IP_CONFIGURE_IP_V6        2u    /* IP v6 configuration */

#if IP_V6_ENABLE
 #define IP_ADDR_MAX_LENGTH       IP_V6_ADDR_SIZE
#elif IP_V4_ENABLE
 #define IP_ADDR_MAX_LENGTH       IP_V4_ADDR_SIZE
#else
 #define IP_ADDR_MAX_LENGTH       4U
#endif

#if IP_V6_ENABLE
 #define IP_V6_MULTICAST_ADDRESS  { 0xFF, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02} /* multicast address of DHCP all Relay Agents and servers */
#endif

/* IP port option */
#define IP_PORT_AUTO_ASSIGN       0u /* automatic port assigment */


/************************************************************************
* TYPEDEFS
*************************************************************************/

/* IP return type */
typedef int  t_ip_ret;

/* IP interface handle */
typedef uint8_t  t_ip_ifc_hdl;

/* IP route handle */
typedef t_ip_ifc_hdl  t_ip_route_hdl;

/* IP route get handle (for ip_route_get) */
typedef uint16_t  t_ip_route_get_hdl;


/* IP packet options */
typedef struct
{
  uint8_t  ipo_ttl;  /* TTL field of IP header */
  uint8_t  ipo_tos;  /* TOS field of IP header */
}
t_ip_opt;


/* IP version */
typedef enum
{
  IPV_IP_NONE = 0u
  , IPV_IP_V4        /* IP version 4 */
  , IPV_IP_V6        /* IP version 6 */
}
t_ip_ver;


/* IP address type */
typedef struct
{
  uint8_t   ipa_address[IP_ADDR_MAX_LENGTH]; /* holds IP address*/
  t_ip_ver  ipa_version;                     /* IP address version */
} t_ip_addr;


/* IP route entry */
typedef struct
{
  uint8_t         ipr_state;     /* route entry state (IP_STATE_...) */
  t_ip_ifc_hdl    ipr_ifc_hdl;   /* interface handle */
  t_ip_addr       ipr_nwaddr;    /* network address */
#if IP_V4_ENABLE
  uint8_t         ipr_netmask[IP_V4_ADDR_SIZE];   /* net mask */
  uint8_t         ipr_gateway[IP_V4_ADDR_SIZE];   /* gateway address */
#endif
  t_ip_route_hdl  ipr_route_hdl; /* route handle */
}
t_ip_route;


/* IP configuration */
typedef struct
{
  uint8_t  ipc_state;       /* state of the IP config (IP_STATE_...) */
                            /* (only used in ip_get_config) */
#if IP_V4_ENABLE
  t_ip_addr  ipc_ipaddr;    /* IP v4 address */

  uint8_t    ipc_netmask[IP_V4_ADDR_SIZE];  /* netmask */
  uint8_t    ipc_gateway[IP_V4_ADDR_SIZE];  /* gateway address */

  uint8_t    ipc_dns1_addr[IP_V4_ADDR_SIZE]; /* 1st DNS server address */
  uint8_t    ipc_dns2_addr[IP_V4_ADDR_SIZE]; /* 2nd DNS server address */
#endif

#if IP_V6_ENABLE
  t_ip_addr  ipc_ip_v6_addr;                  /* IP v6 address */

  uint8_t    ipc_ip_v6_dns1[IP_V6_ADDR_SIZE]; /* IP v6 DNS 1'st address */
  uint8_t    ipc_ip_v6_dns2[IP_V6_ADDR_SIZE]; /* IP v6 DNS 2'nd address */
#endif
} t_ip_config;


/* IP port descriptor */
typedef struct
{
  t_ip_addr  ipp_ip_addr;
  uint16_t   ipp_port;
}
t_ip_port;


/* configured notification type */
typedef void ( * t_ip_config_ntf_fn )( const t_ip_ifc_hdl ifc_hdl
                                      , const uint32_t ntf
                                      , const t_ip_config * const p_ip_config
                                      , const char_t * const p_fqdn );


/* Notification function type */
typedef void ( * t_ip_ntf_fn )( const uint32_t param, const uint32_t ntf );


/* Notification descriptor */
typedef struct s_ip_ntf
{
  /* User variables */
  t_ip_ntf_fn  ntf_fn;            /* notification function */
  uint32_t     ntf_param;         /* parameter sent with the notification */

  /* Internal variable(s) */
  struct s_ip_ntf * p_ntf_next;   /* pointer to the next entry */
}
t_ip_ntf;


/* timeout/notification structure for get buffer functions */
typedef struct
{
  uint32_t   igb_timeout;  /* timeout */
  t_ip_ntf * p_igb_ntf;    /* pointer to notification */
}
t_ip_get_buf_tn;


/* pointer and length of the buffer obtained with get buffer functions */
typedef struct
{
  uint8_t * p_igb_buf;     /* pointer to the buffer */
  uint16_t  igb_buf_len;   /* length of the allocated buffer */
}
t_ip_get_buf;


/************************************************************************
* EXTERNAL VARIABLES
*************************************************************************/


/************************************************************************
* Function Prototype Section
*************************************************************************/

/*************************************************************************
* Function name :
*   t_ip_ret ip_enter_task ( void )
*
* Return :
*   IP_SUCCESS           : on success
*   IP_ERR_NO_MORE_ENTRY : table is full (IP_MAX_TASK exceeded)
*
* Input :
*   none
*
* Output :
*   none
*
* Description :
*   Allows the caller task to call IP stack functions.
*
* Notes :
*   none
**************************************************************************/
t_ip_ret ip_enter_task ( void );


/*************************************************************************
* Function name :
*   t_ip_ret ip_exit_task ( void )
*
* Return :
*   values returned by ip_get_event_entry_nm
*
* Input :
*   none
*
* Output :
*   none
*
* Description :
*   Exits the caller task, this means that the caller task
*   is no longer able to call any functions in the IP stack.
*
* Notes :
*   none
**************************************************************************/
t_ip_ret ip_exit_task ( void );

/*************************************************************************
* Function name :
*   t_ip_ret ip_route_get_hdl ( const t_ip_addr * const p_dst_ip_addr
*                              , t_ip_route_hdl * const p_route_hdl )
*
* Return :
*   IP_SUCCESS           : on success
*   IP_ERR_INVALID_PARAM : if p_route_hdl is NULL
*
* Input :
*   p_dst_ip_addr : pointer to destination IP address
*
* Output :
*   p_route_hdl  : where to write the route handle
*
* Description :
*   Search the route handle based on the destination IP address.
*
* Notes:
*   none
**************************************************************************/
t_ip_ret ip_route_get_hdl ( const t_ip_addr * const p_dst_ip_addr
                           , t_ip_route_hdl * const p_route_hdl );

/*************************************************************************
* Function name :
*   t_ip_ret ip_get_config ( const t_ip_ifc_hdl ifc_hdl
*                           , t_ip_config * const p_ip_config )
*
* Return :
*   IP_SUCCESS : if configuration could be obtained
*   or values returned by ip_ifc_check
*
* Input:
*   ifc_hdl : interface handle
*
* Output :
*   p_ip_config : where to write the current configuration
*
* Description :
*   get IP configuration of an interface.
*
* Notes :
*   none
**************************************************************************/
t_ip_ret ip_get_config ( const t_ip_ifc_hdl ifc_hdl
                        , t_ip_config * const p_ip_config );

/*************************************************************************
* Function name :
*   t_ip_ret ip_set_config ( const t_ip_ifc_hdl ifc_hdl
*                           , t_ip_config * const p_ip_config
*                           , const uint8_t flag )
*
* Return :
*   IP_SUCCESS : if configuration could be set
*   or values returned by ip_ifc_check
*
* Input:
*   ifc_hdl     : interface handle
*   p_ip_config : pointer to the new configuration
*   flag        : flag indicating which configuration should be set:
*                 IP_CONFIGURE_IP_V4 - IP_V4
*                 IP_CONFIGURE_IP_V6 - IP_V6
*
* Output :
*   none
*
* Description :
*   Set IP configuration of an interface.
*
* Notes :
*   none
**************************************************************************/
t_ip_ret ip_set_config ( const t_ip_ifc_hdl ifc_hdl
                        , t_ip_config * const p_ip_config
                        , const uint8_t flag );

/*************************************************************************
* Function name :
*   t_ip_ret ip_route_get_config ( const t_ip_route_hdl route_hdl
*                                 , t_ip_config * const p_ip_cfg );
*
* Return :
*   values returned by ip_route_check_hdl and ip_get_config.
*
* Input :
*   route_hdl : route handle
*
* Output :
*   p_ip_cfg : where to write the IP configuration
*
* Description :
*   Get the IP configuration from route handle.
*
* Notes:
*   none
**************************************************************************/
t_ip_ret ip_route_get_config ( const t_ip_route_hdl route_hdl
                              , t_ip_config * const p_ip_cfg );


#ifdef __cplusplus
}
#endif

#endif /* ifndef _API_IP_H */

