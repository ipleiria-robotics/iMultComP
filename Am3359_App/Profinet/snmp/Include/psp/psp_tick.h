/****************************************************************************
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
#ifndef _PSP_TICK_H
#define _PSP_TICK_H

#include "psp_types.h"
#include <Profinet/snmp/Include/version/ver_psp_tick.h>

#if VER_PSP_TICK_MAJOR != 1
 #error Incompatible VER_PSP_TICK_MAJOR version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define PSP_TICK_RATE    1000u /* Number of ticks per second */
uint32_t psp_get_tick_count ( void );
void psp_tick_init ( void );

#ifdef __cplusplus
}
#endif

#endif /* _PSP_TICK_H */

