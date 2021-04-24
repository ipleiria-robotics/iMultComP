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
#ifndef _PSP_RTC_H
#define _PSP_RTC_H

#include "psp_types.h"

#include "../../version/ver_psp_rtc.h"
#if VER_PSP_RTC_MAJOR != 2
 #error "VER_PSP_RTC_MAJOR invalid"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Range of microseconds */
#define PSP_RTC_USEC_MIN   0u
#define PSP_RTC_USEC_MAX   999999u

/* Range of seconds */
#define PSP_RTC_SEC_MIN    0u
#define PSP_RTC_SEC_MAX    59u

/* Range of minutes */
#define PSP_RTC_MINUTE_MIN 0u
#define PSP_RTC_MINUTE_MAX 59u

/* Range of hours */
#define PSP_RTC_HOUR_MIN   0u
#define PSP_RTC_HOUR_MAX   23u

/* Range of days */
#define PSP_RTC_DAY_MIN    1u
#define PSP_RTC_DAY_MAX    31u

/* Range of week days */
#define PSP_RTC_WDAY_MIN   0u
#define PSP_RTC_WDAY_MAX   6u

/* Range of months */
#define PSP_RTC_MONTH_MIN  1u
#define PSP_RTC_MONTH_MAX  12u

/* Range of years */
#define PSP_RTC_YEAR_MIN   1980u
#define PSP_RTC_YEAR_MAX   2099u

/* Range of hours to UTC */
#define PSP_RTC_ZHOUR_MIN  -11
#define PSP_RTC_ZHOUR_MAX  11

/* Range of minutes to UTC */
#define PSP_RTC_ZMIN_MIN   0u
#define PSP_RTC_ZMIN_MAX   59u

typedef struct
{                  /* local time */
  uint32_t  usec;  /* Microseconds (range: 0..999999). Not used by filesystems. */
  uint8_t   sec;   /* Seconds (range: 0..59) */
  uint8_t   min;   /* Minutes (range: 0..59) */
  uint8_t   hour;  /* hour (range: 0..23) */
  uint8_t   day;   /* Day of month (range: 1..31) */
  uint8_t   month; /* Month (range: 1..12) */
  uint16_t  year;  /* Year in 4 digit format (range: 1980..2099) */
  uint8_t   wday;  /* Days since Sunday (range: 0..6). Not used by filesystems. */
  int8_t    zhour; /* Zone differential time (hours) to UTC (range: -11..11). Not used by filesystems. */
  uint8_t   zmin;  /* Zone differential time (minutes) to UTC (range: 0..59). Not used by filesystems. */
} t_psp_timedate;

void psp_rtc_init ( void );
void psp_getcurrenttimedate ( t_psp_timedate * p_timedate );
void psp_setcurrenttimedate ( t_psp_timedate * p_timedate );
void psp_rtc_adjust_freq ( int64_t delta );
uint8_t psp_convert_to_gmt_unix_time ( t_psp_timedate * p_timedate, uint32_t * p_gmt_unix_time );
void psp_get_gmt_unix_time ( uint32_t * p_gmt_unix_time );

#ifdef __cplusplus
}
#endif


#endif /* _PSP_RTC_H */

