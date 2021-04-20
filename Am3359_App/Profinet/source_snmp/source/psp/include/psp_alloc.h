/***************************************************************************
 *
 *            Copyright (c) 2010-2017 by HCC Embedded
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
#ifndef _PSP_ALLOC_H_
#define _PSP_ALLOC_H_

#include "../../version/ver_psp_alloc.h"
#if VER_PSP_ALLOC_MAJOR != 1 || VER_PSP_ALLOC_MINOR != 1
 #error Incompatible PSP_ALLOC version number!
#endif

#include <stdlib.h>
#include "psp_types.h"
#define psp_malloc( l ) malloc( (size_t)l )
#define psp_free( p )   free( p )

#endif

