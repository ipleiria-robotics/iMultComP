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
#include "../../include/psp_types.h"
#include "../../include/psp_rtc.h"

#include "../../../version/ver_psp_rtc.h"
#if VER_PSP_RTC_MAJOR != 2
 #error "VER_PSP_RTC_MAJOR invalid"
#endif
#if VER_PSP_RTC_MINOR != 1
 #error "VER_PSP_RTC_MINOR invalid"
#endif

  t_psp_timedate g_timedate;

/* initialize RTC */
void psp_rtc_init ( void )
{
  /* initialize RTC */
  g_timedate.sec = 0;
  g_timedate.min = 0;
  g_timedate.hour = 12u;
  g_timedate.wday = 0U;

  g_timedate.day = 1u;
  g_timedate.month = 1u;
  g_timedate.year = 2016u;
  g_timedate.zhour = 0;
  g_timedate.zmin = 0;
}


/****************************************************************************
 *
 * psp_getcurrenttimedate
 *
 * need to be ported depending on system, it retreives the
 * current time and date.
 * Please take care of correct roll-over handling.
 * Roll-over problem is to read a date at 23.59.59 and then reading time at
 * 00:00.00.
 *
 * INPUT
 *
 * p_timedate - pointer where to store time and date
 *
 ***************************************************************************/
void psp_getcurrenttimedate ( t_psp_timedate * p_timedate )
{


  if ( p_timedate != NULL )
  {
    *p_timedate = g_timedate;
  }
} /* psp_getcurrenttimedate */


/****************************************************************************
 *
 * psp_setcurrenttimedate
 *
 * need to be ported depending on system, it sets the
 * current time and date.
 * Please take care of correct roll-over handling.
 *
 * INPUT
 *
 * p_timedate - pointer where to store time and date
 *
 ***************************************************************************/
void psp_setcurrenttimedate ( t_psp_timedate * p_timedate )
{


  if ( p_timedate != NULL )
  {
    g_timedate = *p_timedate;
  }
} /* psp_setcurrenttimedate */


/****************************************************************************
 *
 * psp_rtc_adjust_freq
 *
 * need to be ported depending on system, it sets the
 * current time and date.
 * Adjust frequency of system clock.
 *
 * INPUT
 *
 * delta - pointer where to store time and date
 *
 ***************************************************************************/
void psp_rtc_adjust_freq( int64_t delta )
{
  HCC_UNUSED_ARG(delta);
  /* Implement frequency adjustment of RTC, see RFC 1305 Appendix G. Computer Clock Modelling and Analysis */
}
