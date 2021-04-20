/***************************************************************************
 *
 *            Copyright (c) 2011-2017 by HCC Embedded
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
#ifndef _PSP_RAND_H
#define _PSP_RAND_H

#include "psp_types.h"

#include "../../version/ver_psp_rand.h"
#if VER_PSP_RAND_MAJOR != 1
 #error "VER_PSP_RAND_MAJOR invalid"
#endif

#ifdef __cplusplus
extern "C" {
#endif

uint32_t psp_getrand ( uint32_t val );

#ifdef __cplusplus
}
#endif


#endif /* _PSP_RAND_H */

