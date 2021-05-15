/***************************************************************************
 *
 *            Copyright (c) 2011-2016 by HCC Embedded
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
#ifndef _CONFIG_OAL_H
#define _CONFIG_OAL_H

#include "../version/ver_oal.h"
#if VER_OAL_MAJOR != 2 || VER_OAL_MINOR != 2
 #error Incompatible OAL version number!
#endif


#define OAL_TASK_SUPPORTED        1
#define OAL_TASK_GET_ID_SUPPORTED 1
#define OAL_TASK_SLEEP_SUPPORTED  1

#define OAL_MUTEX_SUPPORTED       1

#define OAL_EVENT_SUPPORTED       1

#define OAL_ISR_SUPPORTED         1


#endif /* ifndef _CONFIG_OAL_H */

