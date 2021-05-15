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
#include <stdint.h>
#include <ti/sysbios/knl/Clock.h>
#include "../../../config/config_oal_os.h"


#include "../../../version/ver_psp_tick.h"
#if VER_PSP_TICK_MAJOR != 1  ||  VER_PSP_TICK_MINOR != 1
 #error Incompatible VER_PSP_TICK_MAJOR version number!
#endif




uint32_t psp_get_tick_count ( void )
{
  return Clock_getTicks();
}

void psp_tick_init ( void )
{

} /* psp_tick_init */

