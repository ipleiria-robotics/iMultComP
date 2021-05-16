/***************************************************************************
 *
 *            Copyright (c) 2012-2016 by HCC Embedded
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
#ifndef _CONFIG_OAL_OS_H
#define _CONFIG_OAL_OS_H

/* OAL user definition file for no OS */
#include "../version/ver_oal_os.h"
#if VER_OAL_SYSBIOS_MAJOR != 1 || VER_OAL_SYSBIOS_MINOR != 3
 #error Incompatible OAL_SYSBIOS version number!
#endif
#include "../version/ver_oal.h"
#if VER_OAL_MAJOR != 2 || VER_OAL_MINOR < 2
 #error Incompatible OAL version number!
#endif

/* priorities */
#define OAL_HIGHEST_PRIORITY 14
#define OAL_HIGH_PRIORITY    10
#define OAL_NORMAL_PRIORITY  7
#define OAL_LOW_PRIORITY     4
#define OAL_LOWEST_PRIORITY  2

/* Event flag to use for user tasks invoking internal functions. */
#define OAL_EVENT_FLAG       0x100

/* tick rate in ms */
#define OAL_TICK_RATE        1

/* no. of max. interrupt sources */
#define OAL_ISR_COUNT        6


#endif /* ifndef _CONFIG_OAL_OS_H */


