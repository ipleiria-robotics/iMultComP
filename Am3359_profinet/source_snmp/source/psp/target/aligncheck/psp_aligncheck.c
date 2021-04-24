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
#include "../../include/psp_aligncheck.h"
#include "../../include/psp_syserr.h"
#include "../../include/psp_types.h"


#include "../../../version/ver_psp_aligncheck.h"
#if VER_PSP_ALIGNCHECK_MAJOR != 1 || VER_PSP_ALIGNCHECK_MINOR != 0
 #error Incompatible PSP_ALIGNCHECK version number!
#endif

#if ( PSP_ALIGN_CHECK_TEST == 0 )


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
void psp_aligncheck ( const uint8_t * p_ucBuff )
{
  /* 95 S : MISRA-C:2004 11.1,11.4: Casting operation to a pointer. */
  /*LDRA_INSPECTED 95 S*/
  /*lint -e{923} */
  if ( (uint8_t)( (uint32_t)p_ucBuff & 0x03U ) != 0U )
  {
    psp_syserr();
  }
}


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
void psp_check_buff_length ( uint16_t unLngth )
{
  if ( ( unLngth & 0x03U ) != 0U )
  {
    psp_syserr();
  }
} /* psp_check_buff_length */
#endif /* if ( PSP_ALIGN_CHECK_TEST == 0 ) */

