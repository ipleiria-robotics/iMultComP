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
#ifndef _PSP_STRING_H_
#define _PSP_STRING_H_

#include <string.h>
#include "psp_types.h"

#include "../../version/ver_psp_string.h"
#if VER_PSP_STRING_MAJOR != 1 || VER_PSP_STRING_MINOR != 5
 #error Incompatible PSP_STRING version number!
#endif

#define psp_memcpy( d, s, l )    memcpy( ( d ), ( s ), (size_t)( l ) )
#define psp_memmove( d, s, l )   memmove( ( d ), ( s ), (size_t)( l ) )
#define psp_memset( d, c, l )    memset( ( d ), ( c ), (size_t)( l ) )
#define psp_memcmp( s1, s2, l )  memcmp( ( s1 ), ( s2 ), (size_t)( l ) )
#define psp_strnlen( s, l )      strnlen( ( s ), (size_t)( l ) )
#define psp_strncat( d, s, l )   strncat( ( d ), ( s ), (size_t)( l ) )
#define psp_strncpy( d, s, l )   strncpy( ( d ), ( s ), (size_t)( l ) )
#define psp_strncmp( s1, s2, l ) strncmp( ( s1 ), ( s2 ), (size_t)( l ) )

#endif /* ifndef _PSP_STRING_H_ */

