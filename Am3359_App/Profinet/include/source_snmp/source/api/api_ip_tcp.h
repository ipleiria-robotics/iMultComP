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
 * This a fake TCP API file. It used by Socket Adaptor to have access to IP
 * TCP structures and TCP functions declarations.
 ***************************************************************************/

#ifndef _API_IP_TCP_H
#define _API_IP_TCP_H


/************************************************************************
*  Include section
*************************************************************************/
#include "../psp/include/psp_types.h"
#include "api_ip.h"

#include "../version/ver_ip_tcp.h"
#if ( VER_IP_TCP_MAJOR != 6 ) || ( VER_IP_TCP_MINOR != 6 )
 #error Incompatible TCP version number!
#endif
#include "../version/ver_psp_types.h"
#if VER_PSP_TYPES_MAJOR != 1
 #error Incompatible PSP_TYPES version number!
#endif
#include "../version/ver_ip.h"
#if VER_IP_MAJOR != 6
 #error Incompatible IP version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************
*  Defines section
*************************************************************************/

/* Invalid TCP port handle */
#define TCP_INVALID_PORT_HDL UINT32_MAX

/* Invalid TCP connection handle */
#define TCP_INVALID_CONN_HDL UINT32_MAX


/************************************************************************
* TYPEDEFS
*************************************************************************/
typedef uint32_t  t_tcp_port_hdl; /* TCP port handle */
typedef uint32_t  t_tcp_conn_hdl; /* TCP connection handle */


/*
** TCP statistics.
** This structure is the output for tcp_scad_get_stats function
*/
typedef struct
{
  uint32_t  ts_transmitted;    /* no. transmitted packets */
  uint32_t  ts_retransmitted;  /* no. retransmitted packets */
  uint32_t  ts_waitack;        /* no. packets waiting for acknowledgment */
} t_tcp_stats;



/************************************************************************
* EXTERNAL VARIABLES
*************************************************************************/


/************************************************************************
* Function Prototype Section
*************************************************************************/


/*************************************************************************
* Function name :
*   t_ip_ret tcp_scad_get_buf ( const t_tcp_conn_hdl tcp_conn_hdl
*                         , const uint16_t req_len
*                         , t_ip_get_buf_tn * const p_get_buf_tn
*                         , t_ip_get_buf * const p_get_buf )
*
* Return :
*   IP_SUCCESS           : buffer obtained successfully
*   IP_ERR_INVALID_SIZE  : invalid size requested
*   IP_ERR_NO_CONNECTION : connection handle is invalid
*   IP_ERR_INVALID_HDL   : invalid connection handle
*   or error codes from tcp_check_conn_hdl and ip_ifc_get_buf
*
* Input :
*   tcp_conn_hdl : TCP connection handle
*   req_len      : requested length
*   p_get_buf_tn : pointer to structure holding timeout and/or
*                  notification
*
* Output:
*   p_get_buf : where to write the properties of the buffer
*
* Description :
*   Tries to get a TCP protocol buffer.
*
* Notes:
*   none
**************************************************************************/
t_ip_ret tcp_scad_get_buf ( const t_tcp_conn_hdl tcp_conn_hdl
                      , const uint16_t req_len
                      , t_ip_get_buf_tn * const p_get_buf_tn
                      , t_ip_get_buf * const p_get_buf );


/*************************************************************************
* Function name :
*   void tcp_scad_release_buf ( uint8_t * const p_buf )
*
* Return :
*   none
*
* Input :
*   p_buf : pointer to the buffer
*
* Output:
*   none
*
* Description :
*   Release an IP buffer based on the buffer pointer.
*
* Notes:
*   This function only works for buffers obtained with
*   tcp_scad_get_buf or buffer pointers returned by tcp_scad_receive.
**************************************************************************/
void tcp_scad_release_buf ( uint8_t * const p_buf );


/*************************************************************************
* Function name :
*   t_ip_ret tcp_scad_open ( const uint16_t port_num
*                      , const uint16_t max_conn_cnt
*                      , t_ip_ntf * const p_ntf
*                      , t_tcp_port_hdl * const p_tcp_port_hdl )
*
* Return :
*   IP_SUCCESS or IP_ERR_PORT_OPEN
*
* Input :
*   port_num : port number to open
*   max_conn : maximum number of connections
*   p_ntf    : pointer to notification
*
* Output:
*   p_tcp_port_hdl : where to write TCP port handle
*
* Description :
*   Open port to accept connections to.
*
* Notes:
*   none
**************************************************************************/
t_ip_ret tcp_scad_open ( const uint16_t port_num
                   , const uint16_t max_conn_cnt
                   , t_ip_ntf * const p_ntf
                   , t_tcp_port_hdl * const p_tcp_port_hdl );


/*************************************************************************
* Function name :
*   t_ip_ret tcp_scad_close ( const t_tcp_port_hdl tcp_port_hdl )
*
* Return :
*   IP_SUCCESS : successful execution
*   values returned by tcp_check_port_hdl.
*
* Input :
*   tcp_port_hdl : TCP port handle
*
* Output:
*   none
*
* Description :
*   Close an opened port.
*
* Notes:
*   none
**************************************************************************/
t_ip_ret tcp_scad_close ( const t_tcp_port_hdl tcp_port_hdl );


/*************************************************************************
* Function name :
*   t_ip_ret tcp_scad_accept ( const t_tcp_port_hdl tcp_port_hdl
*                        , const uint32_t timeout
*                        , t_ip_ntf * const p_ntf
*                        , t_ip_port * const p_ip_port
*                        , t_tcp_conn_hdl * const p_tcp_conn_hdl )
*
* Return :
*   IP_SUCCESS           : connection accepted
*   IP_ERR_NO_CONNECTION : no connection was accepted
*   or values returned by ip_clr_get_event or tcp_check_port_hdl
*
* Input :
*   tcp_port_handle : port handle
*   timeout         : time to wait for a connection (ignored in
*                     non-OS mode)
*   p_ntf           : pointer to the notification
*
* Output:
*   p_ip_port      : where to write remote node IP address and port
*   p_tcp_conn_hdl : where to write connection handle
*
* Description :
*   Accept a connection from a remote node on a port previously
*   opened with tcp_scad_open function.
*
* Notes:
*   none
**************************************************************************/
t_ip_ret tcp_scad_accept ( const t_tcp_port_hdl tcp_port_hdl
                     , const uint32_t timeout
                     , t_ip_ntf * const p_ntf
                     , t_ip_port * const p_ip_port
                     , t_tcp_conn_hdl * const p_tcp_conn_hdl );


/*************************************************************************
* Function name :
*   t_ip_ret tcp_scad_connect ( const uint16_t src_ip_port
*                         , const uint32_t dst_ip_addr
*                         , const uint16_t dst_ip_port
*                         , const uint32_t timeout
*                         , t_ip_ntf * const p_ntf
*                         , t_tcp_conn_hdl * const p_tcp_conn_hdl )
*
* Return :
*   IP_SUCCESS           : if connection is established (never returned
*                          in non-OS mode)
*   IP_ERR_NO_CONNECTION : if connection couldn't be established during
*                          timeout period (only in OS mode)
*   IP_CONN_PENDING      : if connection procedure was successfully
*                          started but the connection wes not yet
*                          established (in non-OS mode)
*   or values returned by ip_route_get_hdl_nm, tcp_scad_get_free_conn,
*   ip_clr_get_event or tcp_scad_check_local_port.
*
* Input :
*   src_ip_port : source IP port number
*   p_dst_ip_addr : pointer to destination IP address structure
*   dst_ip_port : destination IP port number
*   p_ntf     : pointer to notification function
*   timeout   : time to wait for the connection establishment (ignored
*               in non-OS mode)
*
* Output :
*   p_tcp_conn_hdl : where to write the connection handle
*
* Description :
*   Initiate a connection to a remote port.
*
* Notes :
*   none
**************************************************************************/
t_ip_ret tcp_scad_connect ( const uint16_t src_ip_port
                      , const t_ip_addr * const p_dst_ip_addr
                      , const uint16_t dst_ip_port
                      , const uint32_t timeout
                      , t_ip_ntf * const p_ntf
                      , t_tcp_conn_hdl * const p_tcp_conn_hdl );


/*************************************************************************
* Function name :
*   t_ip_ret tcp_scad_disconnect ( const t_tcp_conn_hdl tcp_conn_hdl )
*
* Return :
*   IP_SUCCESS           : if connection was disconnected
*   IP_ERR_NO_CONNECTION : no connection
*   or values returned by tcp_check_conn_hdl
*
* Input :
*   tcp_conn_hdl : TCP connection handle
*
* Output:
*   none
*
* Description :
*   Disconnect a connection. This function can be called whenever
*   the connection needs to be stopped or should always be called
*   when another function returned IP_DISCONNECT_WAIT.
*
* Notes:
*   none
**************************************************************************/
t_ip_ret tcp_scad_disconnect ( const t_tcp_conn_hdl tcp_conn_hdl );


/*************************************************************************
* Function name :
*   t_ip_ret tcp_scad_connection_state ( const t_tcp_conn_hdl tcp_conn_hdl )
*
* Return :
*   IP_SUCCESS           : if connection is established
*   IP_CONN_PENDING      : connection establishment is pending
*   IP_ERR_NO_CONNECTION : if the connection is not active
*   IP_DISCONNECT_WAIT   : waiting for disconnection from the user
*   or values returned by tcp_check_conn_hdl.
*
* Input :
*   tcp_conn_hdl : TCP connection handle
*
* Output:
*   none
*
* Description :
*   Get the state of a connection
*
* Notes:
*   none
**************************************************************************/
t_ip_ret tcp_scad_connection_state ( const t_tcp_conn_hdl tcp_conn_hdl );


/*************************************************************************
* Function name :
*   t_ip_ret tcp_scad_rx_ready ( const t_tcp_conn_hdl tcp_conn_hdl
*                          , uint32_t * const p_len )
*
* Return :
*   IP_SUCCESS : there is unread data.
*   or values returned by tcp_rx_check.
*
* Input :
*   tcp_conn_hdl : TCP connection handle
*
* Output:
*   p_len : where to write the amount of pending data. It can
*           be set to NULL if the information is not needed.
*
* Description :
*   Check if there is any unread data on a connection.
*
* Notes:
*   none
**************************************************************************/
t_ip_ret tcp_scad_rx_ready ( const t_tcp_conn_hdl tcp_conn_hdl
                       , uint32_t * const p_len );


/*************************************************************************
* Function name :
*   t_ip_ret tcp_scad_receive ( const t_tcp_conn_hdl tcp_conn_hdl
*                         , const uint32_t timeout
*                         , uint8_t * * const p_buf
*                         , uint16_t * const p_buf_len )
*
* Return :
*   IP_SUCCESS   : data available and is the last IP fragment
*   IP_MORE_DATA : data available and is not the last IP fragment
*   or values returned by tcp_scad_rx_check, ip_clr_get_event
*   and tcp_scad_ipbuf_queue_get.
*
* Input :
*   tcp_conn_hdl : TCP connection handle
*   timeout      : maximum time to wait for data to be available
*                  in non-OS mode this field is ignored and will
*                  always be non-blocking.
*
* Output:
*   p_buf     : where to write the pointer of the received data
*   p_buf_len : where to write the length of the received data
*
* Description :
*   Receieve data on a connection.
*
* Notes:
*   After the received data is processed the buffer must be released
*   using tcp_scad_release_buf function.
**************************************************************************/
t_ip_ret tcp_scad_receive ( const t_tcp_conn_hdl tcp_conn_hdl
                      , const uint32_t timeout
                      , uint8_t * * const p_buf
                      , uint16_t * const p_buf_len );


/*************************************************************************
* Function name :
*   t_ip_ret tcp_scad_tx_ready ( const t_tcp_conn_hdl tcp_conn_hdl
*                          , const uint32_t timeout )
*
* Return :
*   IP_SUCCESS : TX queue is empty
*   or values returned by tcp_tx_check or ip_clr_get_event.
*
* Input :
*   tcp_conn_hdl : TCP connection handle
*   timeout      : maximum time to wait if transmit is not ready,
*                  only used in non-polled mode.
*
* Output :
*   none
*
* Description :
*   Check if all data has been transmitted on the specified
*   connection. In non-polled mode timeout can be used to wait
*   for a predefined time for empty TX queues.
*
* Notes :
*   none
**************************************************************************/
t_ip_ret tcp_scad_tx_ready ( const t_tcp_conn_hdl tcp_conn_hdl
                       , const uint32_t timeout );


/*************************************************************************
* Function name :
*   t_ip_ret tcp_scad_send ( const t_tcp_conn_hdl tcp_conn_hdl
*                      , uint8_t * const p_buf
*                      , const uint16_t buf_len )
*
* Return :
*   IP_SUCCESS           : packet accepted for transfer
*   IP_DISCONNECT_WAIT   : waiting for disconnection from the user
*   IP_ERR_INVALID_SIZE  : if buffer length exceeds MSS
*   IP_ERR_NO_CONNECTION : connection is not established
*   or values returned by tcp_check_conn_hdl
*
* Input :
*   tcp_conn_hdl : connection handle
*   p_buf        : pointer to the buffer to transmit
*   buf_len      : length of data o send
*
* Output :
*   none
*
* Description :
*   Sends TCP data to remote node.
*
* Notes :
*   p_buf must point to a buffer previously obtained with
*   tcp_scad_get_buf, if the function returns an error the buffer
*   must be released with tcp_scad_release_buf.
**************************************************************************/
t_ip_ret tcp_scad_send ( const t_tcp_conn_hdl tcp_conn_hdl
                   , uint8_t * const p_buf
                   , const uint16_t buf_len );


/*************************************************************************
* Function name :
*   t_ip_ret tcp_scad_set_ip_opt ( const t_tcp_conn_hdl tcp_conn_hdl
*                            , const t_ip_opt * const p_ip_opt )
*
* Return :
*   IP_SUCCESS           : packet accepted for transfer
*   IP_ERR_NO_CONNECTION : connection is not established
*   or values returned by tcp_check_conn_hdl.
*
* Input :
*   p_tcp_conn_hdl : pointer to connection handle
*   p_ip_opt       : pointer to IP options
*
* Output:
*   none
*
* Description :
*   Set IP options to use for all outgoing packets in a connection.
*
* Notes:
*   none
**************************************************************************/
t_ip_ret tcp_scad_set_ip_opt ( const t_tcp_conn_hdl tcp_conn_hdl
                         , const t_ip_opt * const p_ip_opt );


/*************************************************************************
* Function name :
*   t_ip_ret tcp_scad_get_remote_mss ( const t_tcp_conn_hdl tcp_conn_hdl
*                                , uint16_t * const p_mss )
*
* Return :
*   IP_SUCCESS           : packet accepted for transfer
*   IP_ERR_NO_CONNECTION : connection is not established
*   or values returned by tcp_check_conn_hdl.
*
* Input :
*   p_tcp_conn_hdl : pointer to connection handle
*
* Output:
*   p_mss : where to write the MSS size
*
* Description :
*   Return the MSS size of the remote node.
*
* Notes:
*   none
**************************************************************************/
t_ip_ret tcp_scad_get_remote_mss ( const t_tcp_conn_hdl tcp_conn_hdl
                             , uint16_t * const p_mss );


/*************************************************************************
* Function name :
*   t_ip_ret tcp_scad_set_ifc_config ( const t_ip_ifc_hdl ifc_hdl
*                                , const uint16_t mss
*                                , const uint16_t mss_per_window
*                                , const uint16_t max_unack_len )
*
* Return :
*   values returned by ip_ifc_check
*
* Input :
*   ifc_hdl        : interface handle
*   mss            : MSS
*   mss_per_window : no. segments per window
*   max_unack_len  : max unacknowledged length
*
* Output :
*   none
*
* Description :
*   Set TCP configuration for an interface.
*
* Notes :
*   none
**************************************************************************/
t_ip_ret tcp_scad_set_ifc_config ( const t_ip_ifc_hdl ifc_hdl
                             , const uint16_t mss
                             , const uint16_t mss_per_window
                             , const uint16_t max_unack_len );


/*************************************************************************
* Function name :
*   void tcp_scad_get_stats ( t_tcp_stats * p_stats )
*
* Return :
*   none
*
* Input :
*   none
*
* Output:
*   p_stats : where to write statistics
*
* Description :
*   Get statistics.
*
* Notes:
*   none
**************************************************************************/
void tcp_scad_get_stats ( t_tcp_stats * p_stats );


#ifdef __cplusplus
}
#endif

#endif /* ifndef _API_IP_TCP_H */

