/***************************************************************************
 *
 *            Copyright (c) 2017 by HCC Embedded
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
#ifndef _PSP_IP_CONFIG_H
#define _PSP_IP_CONFIG_H
#include "psp_types.h"

#include "../../api/api_ip.h"
#include "../../api/api_ip_tcp.h"
#include "../../version/ver_psp_ip_config.h"
#if ( VER_PSP_IP_CONFIG_MAJOR != 1 ) || ( VER_PSP_IP_CONFIG_MINOR != 1 )
 #error Incompatible PSP_IP_CONFIG version number
#endif



/************************************************************************
* TYPEDEFS
*************************************************************************/

/*****************************************************************************
 * Function wrappers
 ****************************************************************************/


uint32_t psp_ip_set_ifc_config ( uint32_t ifc_hdl
                                , const uint16_t mss
                                , const uint16_t mss_per_window
                                , const uint16_t max_unack_len );

uint32_t psp_ip_get_stats ( t_tcp_stats * p_stats );

uint32_t psp_ip_get_ip_config( const t_ip_ifc_hdl ifc_hdl
                              , t_ip_config * const p_ip_cfg );

uint32_t psp_ip_set_ip_config( const t_ip_ifc_hdl ifc_hdl
                              , t_ip_config * const p_ip_config
                              , const uint8_t flag );

uint32_t psp_ip_get_ifc( const t_ip_addr * const p_dst_ip_addr
                        , t_ip_ifc_hdl * const p_ifc_hdl );

#ifdef __cplusplus
}
#endif

#endif /* ifndef _PSP_IP_CONFIG_H */

