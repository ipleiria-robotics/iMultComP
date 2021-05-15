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
 * This a fake IP config file used by Socket Adaptor
 ***************************************************************************/

#ifndef _CONFIG_IP_H
#define _CONFIG_IP_H

#include "../version/ver_ip.h"
#if ( VER_IP_MAJOR != 6 )
 #error Incompatible IP version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif


/************ general options ***************/
#define IP_MULTICAST_ENABLE    1      /* enable multicasting support */
#define IP_TCP_ENABLE          1      /* enable TCP support */
#define IP_UDP_ENABLE          1      /* enable UDP support */

#define IP_MAX_TASK            16u    /* maximum no. tasks */

#define IP_MAX_FQDN_SIZE       32u    /* max. FQDN size (if DHCP and/or DNS is enabled) */

#define IP_V4_ENABLE           1      /* set to 1 to enable IP protocol version 4 */
#define IP_V6_ENABLE           0      /* set to 1 to enable IP protocol version 6 */

/********** default buffer counts ***********/
#define IP_DEF_MIN_BUF_COUNT   1u     /* default min. no. minimum size buffers */
#define IP_DEF_MTU_BUF_COUNT   1u     /* default min. no. MTU size buffers */

#ifdef __cplusplus
}
#endif

#endif /* ifndef _CONFIG_IP_H */

