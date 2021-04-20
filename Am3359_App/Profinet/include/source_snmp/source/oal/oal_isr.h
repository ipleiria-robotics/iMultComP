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
#ifndef _OAL_ISR_H
#define _OAL_ISR_H

#include "../psp/include/psp_types.h"
#include "oal_common.h"
#include "os/oalp_defs.h"
#include "../config/config_oal.h"

#if OAL_USE_PLATFORM_ISR
 #if OAL_ISR_SUPPORTED
  #include "../psp/target/isr/psp_isr.h"
 #endif
#else
 #include "os/oalp_isr.h"
#endif

#include "../version/ver_oal.h"
#if VER_OAL_MAJOR != 2 || VER_OAL_MINOR != 2
 #error Incompatible OAL version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OAL_USE_PLATFORM_ISR

/* 79 S : MISRA-C:2004 19.4: Macro contains unacceptable items */
/*LDRA_INSPECTED 79 S*/
 #define OAL_ISR_FN PSP_ISR_FN
#endif
typedef  OAL_ISR_FN ( * oal_isr_fn_t );

typedef struct
{
  uint32_t      id;
  uint32_t      pri;
  oal_isr_fn_t  fn;
} oal_isr_dsc_t;


#if OAL_USE_PLATFORM_ISR

/* 79 S : MISRA-C:2004 19.4: Macro contains unacceptable items */
/*LDRA_INSPECTED 79 S*/
 #define oal_isr_id_t psp_isr_id_t

/* 79 S : MISRA-C:2004 19.4: Macro contains unacceptable items */
/*LDRA_INSPECTED 79 S*/
 #define OAL_ISR_PRE  PSP_ISR_PRE

/* 79 S : MISRA-C:2004 19.4: Macro contains unacceptable items */
/*LDRA_INSPECTED 79 S*/
 #define OAL_ISR_POST PSP_ISR_POST

oal_ret_t psp_isr_install ( const oal_isr_dsc_t * isr_dsc, oal_isr_id_t * isr_id );
oal_ret_t psp_isr_enable ( oal_isr_id_t isr_id );
oal_ret_t psp_isr_disable ( oal_isr_id_t isr_id );
oal_ret_t psp_isr_delete ( oal_isr_id_t isr_id );

oal_ret_t psp_int_enable ( void );
oal_ret_t psp_int_disable ( void );

 #define oal_isr_install ( psp_isr_install )
 #define oal_isr_enable  ( psp_isr_enable )
 #define oal_isr_disable ( psp_isr_disable )
 #define oal_isr_delete  ( psp_isr_delete )

 #define oal_int_enable  ( psp_int_enable )
 #define oal_int_disable ( psp_int_disable )

#else /* if OAL_USE_PLATFORM_ISR */

oal_ret_t oal_isr_install ( const oal_isr_dsc_t * isr_dsc, oal_isr_id_t * isr_id );
oal_ret_t oal_isr_enable ( oal_isr_id_t isr_id );
oal_ret_t oal_isr_disable ( oal_isr_id_t isr_id );
oal_ret_t oal_isr_delete ( oal_isr_id_t isr_id );

oal_ret_t oal_int_enable ( void );
oal_ret_t oal_int_disable ( void );

#endif /* if OAL_USE_PLATFORM_ISR */

#ifdef __cplusplus
}
#endif

#endif /* ifndef _OAL_ISR_H */

