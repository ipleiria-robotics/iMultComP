/****************************************************************************
 *
 *            Copyright (c) 2014-2017 by HCC Embedded
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
#ifndef _PSP_BN_ALIGNCHECK_H_
#define _PSP_BN_ALIGNCHECK_H_

#include "psp_types.h"

#include "../../version/ver_psp_aligncheck.h"
#if VER_PSP_ALIGNCHECK_MAJOR != 1 || VER_PSP_ALIGNCHECK_MINOR != 0
 #error Incompatible PSP_ALIGNCHECK version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define PSP_ALIGN_CHECK_TEST 1          /* set to 1 if running encrytpion tests */


/*
 * Needed for tests.
 */
#if ( PSP_ALIGN_CHECK_TEST == 1 )
 #define psp_aligncheck( a ) \
  do \
  { \
    if ( ( (uint32_t)( a ) & 0x03U ) != 0U ) \
    { \
      psp_syserr(); \
    } \
  } \
  while ( 0U )
 #define psp_check_buff_length( a ) \
  do \
  { \
    if ( ( ( a ) & 0x03U ) != 0U ) \
    { \
      psp_syserr(); \
    } \
  } \
  while ( 0U )
#else /* if ( PSP_ALIGN_CHECK_TEST == 1 ) */


/*************************************************************************
 * Function name :
 * uint8_t psp_aligncheck ( const uint8_t * p_ucBuff )
 *
 * Return :
 *   none
 *
 * Inputs:
 *   p_ucBuff       : pointer to the first element of the buffer to have
 *                  address alignment verified
 *
 * Description :
 *   Checks if buffer first element's address is aligned properly to 4 bytes.
 *
 * Note:
 *   none
 *
 **************************************************************************/
void psp_aligncheck ( const uint8_t * p_ucBuff );


/*************************************************************************
 * Function name :
 * uint8_t psp_check_buff_length ( uint16_t unLngth )
 *
 * Return :
 *   none
 *
 * Inputs:
 *   unLngth       : length of the buffer to be checked
 *
 * Description :
 *   Checks if the buffer size is the multiplication of 4.
 *
 * Note:
 *   none
 *
 **************************************************************************/
void psp_check_buff_length ( uint16_t unLngth );
#endif /* if ( PSP_ALIGN_CHECK_TEST == 1 ) */

#ifdef __cplusplus
}
#endif

#endif /* _PSP_BN_ALIGNCHECK_H_ */
