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
#include "../../include/psp_rand.h"
#include "../../include/psp_rtc.h"

#include "../../../version/ver_psp_rand.h"
#if VER_PSP_RAND_MAJOR != 1
 #error "VER_PSP_RAND_MAJOR invalid"
#endif
#if VER_PSP_RAND_MINOR != 0
 #error "VER_PSP_RAND_MINOR invalid"
#endif



static uint32_t  dwrand = 0x729a8fb3;



/****************************************************************************
 *
 * f_getrand
 *
 * val - a number which could be used for random number generator
 *
 * RETURNS
 *
 * 32bit random number
 *
 ***************************************************************************/
uint32_t psp_getrand ( uint32_t val )
{
  uint8_t         i;
  t_psp_timedate  td;

  psp_getcurrenttimedate( &td );

  dwrand ^= ( ( ( (uint32_t)td.sec ) >> 1u )
             | ( ( (uint32_t)td.min ) << 5u )
             | ( ( (uint32_t)td.hour ) << 11u )
             | ( ( (uint32_t)td.day ) << 16u )
             | ( ( (uint32_t)td.month ) << 21u )
             | ( ( (uint32_t)td.year - 1980u ) << 25u ) );

  for ( i = 0u ; i < 32u ; i++ )
  {
    if ( val & 1u )
    {
      dwrand ^= 0x34098bc2;
    }

    if ( dwrand & 0x8000000 )
    {
      dwrand <<= 1u;
      dwrand |= 1u;
    }
    else
    {
      dwrand <<= 1u;
    }

    val >>= 1u;
  }

  return dwrand;
  //return 0xA5A5A5A5;
} /* psp_getrand */


