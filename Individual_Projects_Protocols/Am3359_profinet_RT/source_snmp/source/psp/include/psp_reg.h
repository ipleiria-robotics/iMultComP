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
#ifndef _PSP_REG_H
#define _PSP_REG_H

#include "psp_types.h"

#include "../../version/ver_psp_reg.h"
#if VER_PSP_REG_MAJOR != 1 || VER_PSP_REG_MINOR != 0
 #error Incompatible PSP_REG version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif

uint16_t psp_rev16 ( uint16_t v );
uint32_t psp_rev32 ( uint32_t v );

#define psp_rreg8( base, offset )         ( *(volatile uint8_t *)( base + offset ) )
#define psp_wreg8( base, offset, value )  ( *(volatile uint8_t *)( base + offset ) = ( value ) )
#define psp_rreg16( base, offset )        ( *(volatile uint16_t *)( base + offset ) )
#define psp_wreg16( base, offset, value ) ( *(volatile uint16_t *)( base + offset ) = ( value ) )
#define psp_rreg32( base, offset )        ( *(volatile uint32_t *)( base + offset ) )
#define psp_wreg32( base, offset, value ) ( *(volatile uint32_t *)( base + offset ) = ( value ) )

#ifdef __cplusplus
}
#endif

#endif /* ifndef _PSP_REG_H */

