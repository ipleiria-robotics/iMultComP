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
#include <stddef.h>
#include <stdint.h>
#include "../../config/config_oal.h"
#include "../oal_mutex.h"
#include <ti/osal/SemaphoreP.h>

#include "../../version/ver_oal_os.h"
#if VER_OAL_SYSBIOS_MAJOR != 1 || VER_OAL_SYSBIOS_MINOR != 3
 #error Incompatible OAL_SYSBIOS version number!
#endif
#include "../../version/ver_oal.h"
#if VER_OAL_MAJOR != 2 || VER_OAL_MINOR < 2
 #error Incompatible OAL version number!
#endif


/*
** oal_mutex_create
**
** Create mutex.
**
** Output: p_mutex - pointer to mutex
** Return: OAL_*
*/
#if OAL_MUTEX_SUPPORTED
oal_ret_t oal_mutex_create ( oal_mutex_t * p_mutex )
{
  oal_ret_t         rc = OAL_ERR_RESOURCE;
  SemaphoreP_Params  params;

  SemaphoreP_Params_init( &params );
  params.mode = SemaphoreP_Mode_BINARY;

  *p_mutex = SemaphoreP_create( 1, &params );
  if ( *p_mutex != NULL )
  {
    rc = OAL_SUCCESS;
  }


  return rc;
} /* oal_mutex_create */
#endif  /* if OAL_MUTEX_SUPPORTED */


/*
** oal_mutex_delete
**
** Delete mutex.
**
** Input: p_mutex - pointer to mutex
** Return: OAL_*
*/
#if OAL_MUTEX_SUPPORTED
oal_ret_t oal_mutex_delete ( oal_mutex_t * p_mutex )
{
  oal_ret_t  rc = OAL_ERR_RESOURCE;

  if ( *p_mutex )
  {
    SemaphoreP_delete( p_mutex );
    rc = OAL_SUCCESS;
  }

  return rc;
}
#endif


/*
** oal_mutex_get
**
** Get mutex.
**
** Input: p_mutex - pointer to mutex
** Return: OAL_*
*/
#if OAL_MUTEX_SUPPORTED
oal_ret_t oal_mutex_get ( oal_mutex_t * p_mutex )
{
  oal_ret_t  rc = OAL_ERR_RESOURCE;
  Bool  ret_sem;

  ret_sem = SemaphoreP_pend( *p_mutex, SemaphoreP_WAIT_FOREVER );
  if ( ret_sem == TRUE )
  {
    rc = OAL_SUCCESS;
  }

  return rc;
}
#endif  /* if OAL_MUTEX_SUPPORTED */


/*
** oal_mutex_put
**
** Put mutex.
**
** Input: p_mutex - pointer to mutex
** Return: OAL_*
*/
#if OAL_MUTEX_SUPPORTED
oal_ret_t oal_mutex_put ( oal_mutex_t * p_mutex )
{
  SemaphoreP_post( *p_mutex );

  return OAL_SUCCESS;
}
#endif

