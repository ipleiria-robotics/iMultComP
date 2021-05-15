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
#include "../oal_isr.h"
#include "../../config/config_oal_os.h"

#include <ti/osal/HwiP.h>

#include "../../version/ver_oal_os.h"
#if VER_OAL_SYSBIOS_MAJOR != 1 || VER_OAL_SYSBIOS_MINOR != 3
 #error Incompatible OAL_SYSBIOS version number!
#endif
#include "../../version/ver_oal.h"
#if VER_OAL_MAJOR != 2 || VER_OAL_MINOR < 2
 #error Incompatible OAL version number!
#endif

typedef struct
{
  uint32_t    used;
  uint32_t    id;
  HwiP_Handle  hdl;
} oal_isr_hwi_dsc_t;

static oal_isr_hwi_dsc_t  g_oal_isr_hwi_dsc[OAL_ISR_COUNT];
static HwiP_Params         hwi_params;
//static Error_Block        eb;


/*
** oal_isr_get_free_hwi_dsc
**
** Get next free HWI descriptor.
**
** Return: OAL_*
*/
static oal_ret_t oal_isr_get_free_hwi_dsc ( oal_isr_hwi_dsc_t * * p_oal_isr_hwi_dsc )
{
  oal_ret_t  ret;
  uint32_t  i;

  ret = OAL_ERROR;
  *p_oal_isr_hwi_dsc = NULL;

  for ( i = 0 ; i < OAL_ISR_COUNT ; i++ )
  {
    if ( g_oal_isr_hwi_dsc[i].used == 0u )
    {
      *p_oal_isr_hwi_dsc = &g_oal_isr_hwi_dsc[i];
      ret = OAL_SUCCESS;
      i = OAL_ISR_COUNT;
    }
  }

  return ret;
} /* oal_isr_get_free_hwi_dsc */


/*
** oal_isr_init
**
** Initialize ISR module.
**
** Return: OAL_*
*/
#if OAL_ISR_SUPPORTED
static oal_ret_t oal_isr_init ( void )
{
  static uint32_t  isr_initialized = 0;
  uint32_t         i;

  if ( isr_initialized == 0 )
  {
    for ( i = 0 ; i < OAL_ISR_COUNT ; i++ )
    {
      g_oal_isr_hwi_dsc[i].used = 0;
      g_oal_isr_hwi_dsc[i].hdl  = NULL;
    }

    HwiP_Params_init( &hwi_params );
    //hwi_params.enableInt = 0;
    //Error_init( &eb );
    isr_initialized = 1;
  }

  return OAL_SUCCESS;
} /* oal_isr_init */
#endif /* if OAL_ISR_SUPPORTED */


/*
** oal_isr_install
**
** Install ISR.
**
** Input: isr_dsc - ISR descriptor
** Output: isr_id - ISR ID
** Return: OAL_*
*/
#if OAL_ISR_SUPPORTED
oal_ret_t oal_isr_install ( const oal_isr_dsc_t * isr_dsc, oal_isr_id_t * isr_id )
{
  oal_isr_hwi_dsc_t * p_oal_isr_hwi_dsc;
  oal_ret_t           ret;

  ret = OAL_ERROR;

  oal_isr_init();

  if ( oal_isr_get_free_hwi_dsc( &p_oal_isr_hwi_dsc ) == OAL_SUCCESS )
  {
    p_oal_isr_hwi_dsc->hdl = HwiP_create( isr_dsc->id, isr_dsc->fn, &hwi_params);
    if ( p_oal_isr_hwi_dsc->hdl != NULL )
    {
      p_oal_isr_hwi_dsc->used = 1u;
      p_oal_isr_hwi_dsc->id   = isr_dsc->id;
      *isr_id = ( p_oal_isr_hwi_dsc - g_oal_isr_hwi_dsc );
      ret = OAL_SUCCESS;
    }
  }

  return ret;
} /* oal_isr_install */
#endif /* if OAL_ISR_SUPPORTED */


/*
** oal_isr_delete
**
** Delete ISR.
**
** Input: isr_id - ISR ID
** Return: OAL_*
*/
#if OAL_ISR_SUPPORTED
oal_ret_t oal_isr_delete ( oal_isr_id_t isr_id )
{
  oal_ret_t  ret;

  ret = OAL_ERROR;

  if ( ( isr_id < OAL_ISR_COUNT )
      && ( g_oal_isr_hwi_dsc[isr_id].used != 0u ) )
  {
    HwiP_delete( &g_oal_isr_hwi_dsc[isr_id].hdl );
    g_oal_isr_hwi_dsc[isr_id].hdl  = NULL;
    g_oal_isr_hwi_dsc[isr_id].used = 0;
    ret = OAL_SUCCESS;
  }

  return ret;
}
#endif /* if OAL_ISR_SUPPORTED */


/*
** oal_isr_enable
**
** Enable ISR.
**
** Input: isr_id - ISR ID
** Return: OAL_*
*/
#if OAL_ISR_SUPPORTED
oal_ret_t oal_isr_enable ( oal_isr_id_t isr_id )
{
  oal_ret_t  ret;

  ret = OAL_ERROR;

  if ( ( isr_id < OAL_ISR_COUNT )
      && ( g_oal_isr_hwi_dsc[isr_id].used != 0u ) )
  {
    HwiP_enableInterrupt( g_oal_isr_hwi_dsc[isr_id].id );
    ret = OAL_SUCCESS;
  }

  return ret;
}
#endif


/*
** oal_isr_disable
**
** Disable ISR.
**
** Input: isr_id - ISR ID
** Return: OAL_*
*/
#if OAL_ISR_SUPPORTED
oal_ret_t oal_isr_disable ( oal_isr_id_t isr_id )
{
  oal_ret_t  ret;

  ret = OAL_ERROR;

  if ( ( isr_id < OAL_ISR_COUNT )
      && ( g_oal_isr_hwi_dsc[isr_id].used != 0u ) )
  {
    HwiP_disableInterrupt( g_oal_isr_hwi_dsc[isr_id].id );
    ret = OAL_SUCCESS;
  }

  return ret;
}
#endif


/*
** oal_int_enable
**
** Enable global interrupts.
**
** Return: OAL_*
*/
#if OAL_ISR_SUPPORTED
oal_ret_t oal_int_enable ( void )
{
  HwiP_enable();
  return OAL_SUCCESS;
}
#endif


/*
** oal_int_disable
**
** Disable global interrupts
**
** Return: OAL_*
*/
#if OAL_ISR_SUPPORTED
oal_ret_t oal_int_disable ( void )
{
  HwiP_disable();
  return OAL_SUCCESS;
}
#endif

