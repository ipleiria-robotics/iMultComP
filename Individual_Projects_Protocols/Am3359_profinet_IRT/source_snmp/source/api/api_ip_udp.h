/***************************************************************************
 *
 *            Copyright (c) 2011-2018 by HCC Embedded
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
 * This a fake UDP API file. It used by Socket Adaptor to have access to IP
 * UDP structures and UDP functions declarations.
 ***************************************************************************/

#ifndef _API_IP_UDP_H
#define _API_IP_UDP_H


/************************************************************************
*  Include section
*************************************************************************/
#include "../psp/include/psp_types.h"
#include "api_ip.h"

#include "../version/ver_ip_udp.h"
#if ( VER_IP_UDP_MAJOR != 7 ) || ( VER_IP_UDP_MINOR != 1 )
 #error Incompatible UDP version number!
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

/* Invalid UDP handle */
#define UDP_INVALID_HDL UINT32_MAX


/************************************************************************
* TYPEDEFS
*************************************************************************/
typedef uint32_t  t_udp_hdl; /* UDP handle */


/************************************************************************
* EXTERNAL VARIABLES
*************************************************************************/


/************************************************************************
* Function Prototype Section
*************************************************************************/


/*************************************************************************
* Function name :
*  t_ip_ret udp_scad_get_buf ( const t_ip_route_hdl route_hdl
*                        , const uint16_t req_len
*                        , t_ip_get_buf_tn * const p_get_buf_tn
*                        , t_ip_get_buf * const p_get_buf )
*
* Return :
*   values returned by ip_route_check_hdl or ip_pool_get_ipbuf.
*
* Input :
*   route_hdl    : route handle
*   req_len      : requested length
*   p_get_buf_tn : pointer to structure holding timeout and/or
*                  notification
*
* Output:
*   p_get_buf : where to write the properties of the buffer
*
* Description :
*   Tries to get a UDP protocol buffer.
*
* Notes:
*   none
**************************************************************************/
t_ip_ret udp_scad_get_buf ( const t_ip_route_hdl route_hdl
                      , const uint16_t req_len
                      , t_ip_get_buf_tn * const p_get_buf_tn
                      , t_ip_get_buf * const p_get_buf );


/*************************************************************************
* Function name :
*  void udp_scad_release_buf ( uint8_t * const p_buf )
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
*   udp_scad_get_buf or buffer pointers returned by udp_scad_receive.
**************************************************************************/
void udp_scad_release_buf ( uint8_t * const p_buf );


/*************************************************************************
* Function name :
*   t_ip_ret udp_scad_open ( const uint16_t port_num
*                      , t_ip_ntf * const p_ntf
*                      , t_ip_addr * const p_ip_addr
*                      , t_udp_hdl * const p_udp_hdl )
*
* Return :
*   IP_SUCCESS           : on success
*   IP_ERR_PORT_OPENED   : if already opened
*   IP_ERR_NO_MORE_ENTRY : if no more entry (UDP_SERVER_PORTS exceeded)
*
* Input :
*   port_num  : number of UDP port to be opened
*   p_ntf     : pointer to the notification structure
*   p_ip_addr : pointer to the IP address or NULL if all IP addresses accepted
*
* Output :
*   p_udp_hdl : where to write the udp handle
*
* Description :
*   Open a port for receiving.
*
* Notes :
*   none
**************************************************************************/
t_ip_ret udp_scad_open ( const uint16_t port_num
                        , t_ip_addr * const p_ip_addr
                        , t_ip_ntf * const p_ntf
                        , t_udp_hdl * const p_udp_hdl );


/*************************************************************************
* Function name :
*   t_ip_ret udp_scad_close ( const t_udp_hdl udp_hdl )
*
* Return :
*   IP_SUCCESS : on success
*   or values returned by udp_check_hdl
*
* Input :
*   udp_hdl : UDP handler
*
* Output :
*   none
*
* Description :
*   called by user application to close a UDP port.
*
* Notes :
*   none
**************************************************************************/
t_ip_ret udp_scad_close ( const t_udp_hdl udp_hdl );


/*************************************************************************
* Function name :
*   t_ip_ret udp_scad_rx_ready ( const t_udp_hdl udp_hdl
*                          , uint32_t * const p_len )
*
* Return :
*   IP_SUCCESS     : on success
*   IP_ERR_NO_DATA : there is no data available
*   or values returned by udp_check_hdl
*
* Input :
*   udp_hdl : udp handle
*
* Output :
*   p_len : where to write the amount of pending data. It can
*           be set to NULL if the information is not needed.
*
* Description :
*   Check if there is data available on an opened UDP port
*
* Notes :
*   none
**************************************************************************/
t_ip_ret udp_scad_rx_ready ( const t_udp_hdl udp_hdl
                       , uint32_t * const p_len );


/*************************************************************************
* Function name :
*   t_ip_ret udp_scad_receive ( const t_udp_hdl udp_hdl
*                         , const uint32_t timeout
*                         , t_ip_route_hdl * const p_route_hdl
*                         , t_ip_port * const p_ip_port
*                         , uint8_t * * const pp_buf
*                         , uint16_t * const p_buf_len )
*
* Return :
*   IP_SUCCESS         : data available and is the last IP fragment
*   IP_MORE_DATA       : data available and is not the last IP fragment
*   IP_ERR_NO_DATA     : there is no data available
*   IP_ERR_INVALID_HDL : invalid handle
*   or values returned by udp_check_hdl, ip_clr_get_event and
*   ipbuf_queue_get
*
* Input :
*   udp_hdl : udp handle
*   timeout : time to wait for a packet
*
* Output :
*   p_route_hdl : where to write the route handle (optional)
*   p_ip_port   : IP address and UDP port of the sender
*   pp_buf      : where to write the pointer of the received data
*   p_buf_len   : where to write the received data length
*
* Description :
*   called by user application to retrieve the data on a port opened
*   earlier with udp_scad_open.
*
* Notes:
*   After the received data is processed the buffer must be released
*   using udp_scad_release_buf function.
**************************************************************************/
t_ip_ret udp_scad_receive ( const t_udp_hdl udp_hdl
                      , const uint32_t timeout
                      , t_ip_route_hdl * const p_route_hdl
                      , t_ip_port * const p_ip_port
                      , uint8_t * * const pp_buf
                      , uint16_t * const p_buf_len );


/*************************************************************************
* Function name :
*   t_ip_ret udp_scad_send ( const t_ip_route_hdl route_hdl
*                      , const uint16_t src_port_num
*                      , const t_ip_addr * const p_dst_ip_addr
*                      , const uint16_t dst_port_num
*                      , uint8_t * const p_buf
*                      , const uint16_t buf_len
*                      , const t_ip_opt * const p_ip_opt )
*
* Return :
*   values returned by ip_send_pkt
*
* Input :
*   route_hdl     : route handle
*   src_port_num  : source port number
*   p_dst_ip_addr : pointer to destination IP address
*   dst_port_num  : destination port number
*   p_buf         : pointer to the buffer to send
*   buf_len       : length of the data
*   p_ip_opt      : pointer to IP options (default values are
*                  used if set to NULL)
*
* Output :
*   none
*
* Description :
*   Sends UDP data to remote host.
*
* Notes :
*   p_buf must point to a buffer previously obtained with
*   udp_scad_get_buf, if the function returns an error the buffer
*   must be released with udp_scad_release_buf.
**************************************************************************/
t_ip_ret udp_scad_send ( const t_ip_route_hdl route_hdl
                   , const uint16_t src_port_num
                   , const t_ip_addr * const p_dst_ip_addr
                   , const uint16_t dst_port_num
                   , uint8_t * const p_buf
                   , const uint16_t buf_len
                   , const t_ip_opt * const p_ip_opt );


#ifdef __cplusplus
}
#endif

#endif /* ifndef _API_IP_UDP_H */

