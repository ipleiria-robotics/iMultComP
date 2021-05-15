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
#include "../oal_event.h"


#include "../../version/ver_oal_os.h"
#if VER_OAL_SYSBIOS_MAJOR != 1 || VER_OAL_SYSBIOS_MINOR != 3
 #error Incompatible OAL_SYSBIOS version number!
#endif
#include "../../version/ver_oal.h"
#if VER_OAL_MAJOR != 2 || VER_OAL_MINOR < 2
 #error Incompatible OAL version number!
#endif


/*
** oal_event_create
**
** Create event.
**
** Output: p_event - pointer to event
** Return: OAL_*
*/
#if OAL_EVENT_SUPPORTED
oal_ret_t oal_event_create ( oal_event_t * p_event )
{
  oal_ret_t  rc = OAL_SUCCESS;
  EventP_Params eventParams;
  EventP_Params_init(&eventParams);

  *p_event = EventP_create(&eventParams);
  if ( *p_event == 0 )
  {
    rc = OAL_ERR_RESOURCE;
  }

  return rc;
} /* oal_event_create */
#endif  /* if OAL_EVENT_SUPPORTED */


/*
** oal_event_delete
**
** Delete event.
**
** Input: p_event - pointer to event
** Return: OAL_*
*/
#if OAL_EVENT_SUPPORTED
oal_ret_t oal_event_delete ( oal_event_t * p_event )
{
   EventP_delete( p_event );

  return OAL_SUCCESS;
}
#endif


/*
** oal_event_clear
**
** Clear event flags in an event group.
**
** Input: p_event - pointter to event
**        cflags  - flags to clear
**        task_id - task ID (may not be required)
** Return: OAL_*
*/
#if OAL_EVENT_SUPPORTED
oal_ret_t oal_event_clear ( oal_event_t * p_event, oal_event_flags_t cflags, oal_task_id_t task_id )
{
  oal_event_flags_t  aflags;

  (void)task_id;
  aflags = EventP_getPostedEvents( *p_event ); /* Get pending events */
  if ( aflags & cflags )                      /* Check flag status */
  {
    aflags = EventP_pend( *p_event, 0, cflags, 0 );
    cflags &= aflags;
    aflags ^= cflags;
    if ( aflags )
    {
        EventP_post( *p_event, aflags ); /* Send pending events - cleared event again */
    }
  }

  return OAL_SUCCESS;
} /* oal_event_clear */
#endif /* if OAL_EVENT_SUPPORTED */


/*
** oal_event_get
**
** Get event.
**
** Input: p_event - pointer to event
**        wflags - wait for these flags
**        timeout - timeout (OAL_WAIT_FOREVER/..)
** Output: sflags - set flags
** Return: OAL_*
*/
#if OAL_EVENT_SUPPORTED
oal_ret_t oal_event_get ( oal_event_t * p_event, oal_event_flags_t wflags, oal_event_flags_t * sflags, oal_event_timeout_t timeout )
{
  oal_event_flags_t  cflags;
  oal_ret_t          rc = OAL_ERR_TIMEOUT;

  if ( timeout != OAL_WAIT_FOREVER )
  {
    if ( timeout != 0 )
    {
      timeout /= OAL_TICK_RATE;
      if ( timeout == 0 )
      {
        timeout = 1;
      }
    }
  }

  cflags = EventP_pend( *p_event, 0, wflags, timeout );
  if ( cflags != 0 )
  {
    if ( cflags & wflags )
    {
      *sflags = cflags & wflags;
      rc = OAL_SUCCESS;
    }

    cflags &= ~wflags;
    if ( cflags )
    {
      EventP_post( *p_event, cflags );
    }
  }

  return rc;
} /* oal_event_get */
#endif  /* if OAL_EVENT_SUPPORTED */


/*
** oal_event_set
**
** Set event.
**
** Input: p_event - pointer to event
**        flags - flags to set
**        task_id - task ID (may not be required)
** Return: OAL_*
*/
#if OAL_EVENT_SUPPORTED
oal_ret_t oal_event_set ( oal_event_t * p_event, oal_event_flags_t flags, oal_task_id_t task_id )
{
  (void)task_id;

  EventP_post( *p_event, flags );

  return OAL_SUCCESS;
}
#endif


/*
** oal_event_set_int
**
** Set event from interrupt.
**
** Input: p_event - pointer to event
**        flags - flags to set
**        task_id - task ID (may not be required)
** Return: OAL_*
*/
#if OAL_EVENT_SUPPORTED
oal_ret_t oal_event_set_int ( oal_event_t * p_event, oal_event_flags_t flags,  oal_task_id_t task_id )
{
  return oal_event_set( p_event, flags, task_id );
}
#endif


