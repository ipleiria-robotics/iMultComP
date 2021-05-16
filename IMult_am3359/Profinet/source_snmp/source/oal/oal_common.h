/***************************************************************************
 *
 *            Copyright (c) 2010-2016 by HCC Embedded
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
#ifndef _OAL_COMMON_H
#define _OAL_COMMON_H

#include "../version/ver_oal.h"
#if VER_OAL_MAJOR != 2 || VER_OAL_MINOR != 2
 #error Incompatible OAL version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum
{
  OAL_SUCCESS = 0
  , OAL_ERR_RESOURCE
  , OAL_ERR_TIMEOUT
  , OAL_ERROR
};

/* 495 S : MISRA-C:2004 6.3: Typedef name has no size indication. */
/*LDRA_INSPECTED 495 S*/
typedef int  oal_ret_t;

#ifdef __cplusplus
}
#endif

#endif /* ifndef _OAL_COMMON_H */

