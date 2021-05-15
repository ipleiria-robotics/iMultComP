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
#include "../../include/psp_types.h"
#include "../../include/psp_string.h"
#include "../../include/psp_ip_config.h"

#include "../../../version/ver_psp_ip_config.h"
#if ( VER_PSP_IP_CONFIG_MAJOR != 1 ) || ( VER_PSP_IP_CONFIG_MINOR != 1 )
 #error Incompatible PSP_IP_CONFIG version number
#endif

/*************************************************************************
* Function name :
*   t_ip_ret psp_ip_set_ifc_config ( const t_ip_ifc_hdl ifc_hdl
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
uint32_t psp_ip_set_ifc_config ( uint32_t ifc_hdl
                                , const uint16_t mss
                                , const uint16_t mss_per_window
                                , const uint16_t max_unack_len )
{
  HCC_UNUSED_ARG(ifc_hdl);
  HCC_UNUSED_ARG(mss);
  HCC_UNUSED_ARG(mss_per_window);
  HCC_UNUSED_ARG(max_unack_len);
  /* Implement here platform specififc configuration of TCP stack parameters */
  return 1U;
} /* psp_ip_set_ifc_config */


/*************************************************************************
* Function name :
*   uint32_t psp_ip_get_stats ( t_tcp_stats * p_stats )
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
uint32_t psp_ip_get_stats ( t_tcp_stats * p_stats )
{
  HCC_UNUSED_ARG(p_stats);
  /* Implement here platform specififc obtaining of IP TCP statistics */
  return 1U;
}

/*************************************************************************
* Function name :
*   t_ip_ret psp_ip_get_ip_config ( const t_ip_ifc_hdl ifc_hdl
*                                  , t_ip_config * const p_ip_config )
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
uint32_t psp_ip_get_ip_config( const t_ip_ifc_hdl ifc_hdl
                              , t_ip_config * const p_ip_cfg )
{
  HCC_UNUSED_ARG(ifc_hdl);
  HCC_UNUSED_ARG(p_ip_cfg);
  /* Implement here platform specififc obtaining of IP address configuration
     of given address */
  return 1U;
}


/*************************************************************************
* Function name :
*   t_ip_ret psp_ip_set_ip_config ( const t_ip_ifc_hdl ifc_hdl
*                                  , t_ip_config * const p_ip_config
*                                  , const uint8_t flag )
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
uint32_t psp_ip_set_ip_config( const t_ip_ifc_hdl ifc_hdl
                              , t_ip_config * const p_ip_config
                              , const uint8_t flag )
{
  HCC_UNUSED_ARG(ifc_hdl);
  HCC_UNUSED_ARG(p_ip_config);
  HCC_UNUSED_ARG(flag);
  /* Implement here platform specififc Interface IP configuration */
  return 1U;
}

/*************************************************************************
* Function name :
*   t_ip_ret psp_ip_get_ifc ( const t_ip_addr * const p_dst_ip_addr
*                              , t_ip_ifc_hdl * const p_route_hdl )
*
* Return :
*   values returned by ip_route_get_hdl_nm.
*
* Input :
*   p_dst_ip_addr : pointer to destination IP address
*
* Output :
*   p_route_hdl  : where to write the route handle
*
* Description :
*   Search for an interface based on the destination IP address.
*
* Notes:
*   none
**************************************************************************/
uint32_t psp_ip_get_ifc( const t_ip_addr * const p_dst_ip_addr
                        , t_ip_ifc_hdl * const p_ifc_hdl )
{
  HCC_UNUSED_ARG(p_dst_ip_addr);
  /* Implement here platform specififc search for an interface assigned with
     given address */
  if ( p_ifc_hdl != NULL )
  {
    *p_ifc_hdl = 0U;
  }
  return 0U;
}
