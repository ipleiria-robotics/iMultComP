/***************************************************************************
 *
 *            Copyright (c) 2018 by HCC Embedded
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
#include "../../include/psp_sockets.h"

#include "../../../version/ver_psp_sockets.h"
#if ( VER_PSP_SOCKETS_MAJOR != 1 ) || ( VER_PSP_SOCKETS_MINOR != 5 )
 #error Incompatible PSP_SOCKETS version number
#endif

/*************************************************************************
* Function name :
*   int psp_sc_geterrno( int s, int * p_err)
*
* Return :
*   0
*
* Input:
*   s    : socket descriptor (not used)
*
* Output :
*   p_err : pointer to varaible that will receive error number
*
* Description :
*   Get IP configuration of an interface.
*
* Notes :
*   none
**************************************************************************/
int psp_sc_geterrno( t_psp_socket s, int * p_err)
{
  HCC_UNUSED_ARG(s);

  *p_err = fdError();
  return 0;
}

/*************************************************************************
* Function name :
*   uint32_t psp_sc_get_ip_config ( const t_ip_ifc_hdl ifc_hdl
*                                  , t_ip_config * const p_ip_config )
*
* Return :
*   IP_SUCCESS : if configuration could be set
*   or values returned by ip_ifc_check
*
* Input:
*   ifc_hdl     : interface handle
*
* Output :
*   p_ip_config : pointer to the new configuration
*
* Description :
*   Get IP configuration of an interface.
*
* Notes :
*   none
**************************************************************************/
uint32_t psp_sc_get_ip_config( const t_ip_ifc_hdl ifc_hdl
                              , t_ip_config * const p_ip_cfg )
{
  if ( p_ip_cfg != NULL )
  {
    p_ip_cfg->ipc_ipaddr.ipa_version = IPV_IP_V4;
    psp_memset(p_ip_cfg->ipc_ipaddr.ipa_address, 0, IP_V4_ADDR_SIZE );
  }
  return 0;
}


/*************************************************************************
* Function name :
*   uint32_t psp_sc_set_ip_config ( const t_ip_ifc_hdl ifc_hdl
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
uint32_t psp_sc_set_ip_config( const t_ip_ifc_hdl ifc_hdl
                        , t_ip_config * const p_ip_config
                        , const uint8_t flag )

{
  return 0;
}


/*************************************************************************
* Function name :
*   uint32_t psp_sc_get_ifc ( const t_ip_addr * const p_dst_ip_addr
*                            , t_ip_ifc_hdl * const p_ifc_hdl )
*
* Return :
*   IP_SUCCESS : if configuration could be set
*   or values returned by ip_ifc_check
*
* Input:
*   p_dst_ip_addr : pointer to IP address
*
* Output :
*   p_ifc_hdl   : pointer to interface handle
*
* Description :
*   Gets interface number that will be used for communicating with.
*
* Notes :
*   none
**************************************************************************/
uint32_t psp_sc_get_ifc( const t_ip_addr * const p_dst_ip_addr
                           , t_ip_ifc_hdl * const p_ifc_hdl )
{
  return 0;
}
