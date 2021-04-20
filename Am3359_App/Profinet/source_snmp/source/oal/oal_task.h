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
#ifndef _OAL_TASK_H
#define _OAL_TASK_H

#include "../psp/include/psp_types.h"
#include "../config/config_oal.h"
#include "oal_common.h"
#include "os/oalp_defs.h"
#include "os/oalp_task.h"

#include "../version/ver_oal.h"
#if VER_OAL_MAJOR != 2 || VER_OAL_MINOR != 2
 #error Incompatible OAL version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define TASK_INDEX_INVALID ( (oal_task_index_t)UINT32_MAX )

typedef  OAL_TASK_FN ( * oal_task_fn_t );

/* For creating multiple instances of same task */
typedef uint32_t  oal_task_index_t;

typedef struct
{
  const char_t * name;
  oal_task_fn_t  entry;
  uint32_t       priority;
  uint32_t       stack_size;
#if OAL_STATIC_TASK_STACK
  uint32_t     * stack_ptr;
#endif
} oal_task_dsc_t;

typedef struct
{
  const char_t  * name;
  oal_task_fn_t   entry;
  uint32_t        priority;
  uint32_t        stack_size;
#if OAL_STATIC_TASK_STACK
  uint32_t      * stack_ptr;
#endif
  uint32_t        task_count;
  oal_task_id_t * p_ids;
  uint8_t       * pb_ids_used;
} oal_task_m_dsc_t;

#if OAL_STATIC_TASK_STACK

/* 125 S : MISRA-C:2004 19.13: Use of ## or # in a macro */
/*LDRA_INSPECTED 125 S*/
/* 76 S : MISRA-C:2004 19.12: More than one of # or ## in a macro */
/*LDRA_INSPECTED 76 S*/
/*LDRA_INSPECTED 76 S*/
/* 79 S : MISRA-C:2004 19.4: Macro contains unacceptable items */
/*LDRA_INSPECTED 79 S*/
 #define OAL_TASK_DSC( name, entry, priority, stack_size ) \
  static uint32_t entry##_stack[( ( ( stack_size ) - ( 1u ) ) + (uint16_t)sizeof( uint32_t ) ) / sizeof( uint32_t )]; \
  static const oal_task_dsc_t  entry##_dsc = \
  { \
    ( name ) \
    , ( entry ) \
    , ( priority ) \
    , ( stack_size ) \
    , &entry##_stack[0] \
  }

/* 125 S : MISRA-C:2004 19.13: Use of ## or # in a macro */
/*LDRA_INSPECTED 125 S*/
/* 76 S : MISRA-C:2004 19.12: More than one of # or ## in a macro */
/*LDRA_INSPECTED 76 S*/
/*LDRA_INSPECTED 76 S*/
/* 79 S : MISRA-C:2004 19.4: Macro contains unacceptable items */
/*LDRA_INSPECTED 79 S*/
 #define OAL_TASK_M_DSC( name, entry, priority, stack_size, count ) \
  static uint32_t entry##_stack[( count ) * ( ( ( stack_size ) - ( 1u ) ) + (uint16_t)sizeof( uint32_t ) ) / sizeof( uint32_t )]; \
  static oal_task_id_t           entry##_m_ids[count]; \
  static uint8_t                 entry##_m_ids_used[count]; \
  static const oal_task_m_dsc_t  entry##_dsc = \
  { \
    ( name ) \
    , ( entry ) \
    , ( priority ) \
    , ( stack_size ) \
    , &entry##_stack[0] \
    , ( count ) \
    , entry##_m_ids \
    , entry##_m_ids_used \
  }
#else /* if OAL_STATIC_TASK_STACK */

/* 125 S : MISRA-C:2004 19.13: Use of ## or # in a macro */
/*LDRA_INSPECTED 125 S*/
/* 79 S : MISRA-C:2004 19.4: Macro contains unacceptable items */
/*LDRA_INSPECTED 79 S*/
 #define OAL_TASK_DSC( name, entry, priority, stack_size ) \
  static const oal_task_dsc_t entry##_dsc = \
  { \
    ( name ) \
    , ( &entry ) \
    , ( priority ) \
    , ( stack_size ) \
  }

/* 125 S : MISRA-C:2004 19.13: Use of ## or # in a macro */
/*LDRA_INSPECTED 125 S*/
/* 76 S : MISRA-C:2004 19.12: More than one of # or ## in a macro */
/*LDRA_INSPECTED 76 S*/
/*LDRA_INSPECTED 76 S*/
/* 79 S : MISRA-C:2004 19.4: Macro contains unacceptable items */
/*LDRA_INSPECTED 79 S*/
 #define OAL_TASK_M_DSC( name, entry, priority, stack_size, count ) \
  static oal_task_id_t entry##_m_ids[count]; \
  static uint8_t                 entry##_m_ids_used[count]; \
  static const oal_task_m_dsc_t  entry##_dsc = \
  { \
    ( name ) \
    , ( entry ) \
    , ( priority ) \
    , ( stack_size ) \
    , ( count ) \
    , entry##_m_ids \
    , entry##_m_ids_used \
  }
#endif /* if OAL_STATIC_TASK_STACK */

oal_ret_t oal_task_create ( oal_task_t * p_task, const oal_task_dsc_t * task_dsc, oal_task_id_t * task_id );
oal_ret_t oal_task_delete ( oal_task_t * p_task );
oal_ret_t oal_task_m_dsc_init ( const oal_task_m_dsc_t * task_dsc );
oal_ret_t oal_task_m_create ( oal_task_t * p_task, const oal_task_m_dsc_t * task_dsc, oal_task_index_t task_index, oal_task_id_t * task_id );
oal_ret_t oal_task_m_delete ( oal_task_t * p_task, const oal_task_m_dsc_t * task_dsc, oal_task_id_t task_id );
oal_task_index_t oal_task_m_get_index ( const oal_task_m_dsc_t * task_dsc, oal_task_id_t task_id );
void oal_task_yield ( void );
oal_task_id_t oal_task_get_id ( void );
void oal_task_sleep ( uint32_t ms );

#ifdef __cplusplus
}
#endif

#endif /* ifndef _OAL_TASK_H */

