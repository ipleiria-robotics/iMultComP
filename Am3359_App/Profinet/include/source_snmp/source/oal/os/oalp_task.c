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
#include "../oal_task.h"
#include "../../psp/include/psp_string.h"

#include "../../version/ver_oal_os.h"
#if VER_OAL_SYSBIOS_MAJOR != 1 || VER_OAL_SYSBIOS_MINOR != 3
 #error Incompatible OAL_SYSBIOS version number!
#endif
#include "../../version/ver_oal.h"
#if VER_OAL_MAJOR != 2 || VER_OAL_MINOR < 2
 #error Incompatible OAL version number!
#endif

int oal_task_pre(void)
{
   int rc;

   /* allocate the file descriptor environment for this task */
   rc = fdOpenSession(TaskP_self());
   if (rc == 1)
      return (OAL_SUCCESS);
   else
      return (OAL_ERR_RESOURCE);
}

/*
** oal_task_init
**
** Initialize task module.
**
** Return: OAL_*
*/
#if OAL_TASK_SUPPORTED
static oal_ret_t oal_task_init ()
{
  static uint8_t  task_initialized = 0u;

  if ( task_initialized == 0u )
  {
    task_initialized = 1u;
  }

  return OAL_SUCCESS;
}
#endif /* if OAL_TASK_SUPPORTED */


/*
** oal_task_create
**
** Create a task.
**
** Input: p_task - pointer to the task
**        oal_task_dsc - task descriptor
** Output: task_id - created task ID
** Return: OAL_*
*/
#if OAL_TASK_SUPPORTED
oal_ret_t oal_task_create ( oal_task_t * p_task, const oal_task_dsc_t * task_dsc, oal_task_id_t * task_id )
{
  oal_ret_t           rc;
  TaskP_Params         params;
  //static Error_Block  task_eb;

  //rc = oal_task_init( &task_eb );
  rc = oal_task_init();

  if ( rc == OAL_SUCCESS )
  {
    rc = OAL_ERR_RESOURCE;
    TaskP_Params_init( &params );
    params.priority = task_dsc->priority;
    params.stacksize = task_dsc->stack_size;

    *p_task = TaskP_create((void*)task_dsc->entry, &params);
    if ( *p_task != NULL )
    {
      *task_id = *p_task;
      rc = OAL_SUCCESS;
    }
  }

  return rc;
} /* oal_task_create */
#endif /* if OAL_TASK_SUPPORTED */


/*
** oal_task_delete
**
** Delete the caller task.
**
** Input: p_task - pointer to the task
** Return: OAL_*
*/
#if OAL_TASK_SUPPORTED
oal_ret_t oal_task_delete ( oal_task_t * p_task )
{
  TaskP_delete( p_task );

  return OAL_SUCCESS;
}
#endif


/*
** oal_task_yield
**
** Yield current task
*/
#if OAL_TASK_SUPPORTED
void oal_task_yield ( void )
{
  TaskP_yield();
}
#endif


/*
** oal_task_get_id
**
** Get task ID.
**
** Return: Task ID
*/
#if OAL_TASK_GET_ID_SUPPORTED
oal_task_id_t oal_task_get_id ( void )
{
  return TaskP_self();
}
#endif


/*
** oal_task_sleep
**
** Suspends the caller task for 'ms' milliseconds
**
** Input: ms - milliseconds to sleep
*/
#if OAL_TASK_SLEEP_SUPPORTED
void oal_task_sleep ( uint32_t ms )
{
  ms /= OAL_TICK_RATE;
  if ( ms == 0 )
  {
    ms = 1;
  }

  TaskP_sleep( ms );
}
#endif

