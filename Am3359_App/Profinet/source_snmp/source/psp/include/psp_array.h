/***************************************************************************
 *
 *            Copyright (c) 2010-2017 by HCC Embedded
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
#ifndef _PSP_ARRAY_H_
#define _PSP_ARRAY_H_

#include "psp_types.h"

#include "../../version/ver_psp_array.h"
#if VER_PSP_ARRAY_MAJOR != 1 || VER_PSP_ARRAY_MINOR != 0
 #error Incompatible PSP_ARRAY version number!
#endif


/*
 * PSP_BYTE_ORDER_BE - if defined as 0, byte order is set to little endian by default.
 *                     if defined as 1, byte order is set to big endian
 */
#define PSP_BYTE_ORDER_BE          0

#define PSP_ARRAY_BITS_BYTES_CONV  0x03U
#define PSP_ARRAY_BYTES_DWORD_CONV 0x02U

#define PSP_ARRAY_LSB_111          0x07U

#if ( PSP_BYTE_ORDER_BE == 1 )
 #define PSP_RD_32BITARRAY_OFFSET( pointer, offset, arr_length ) \
  ( ( pointer )[( ( arr_length ) - 1U ) - ( offset )] )
#else
 #define PSP_RD_32BITARRAY_OFFSET( pointer, offset, arr_length ) \
  ( ( pointer )[( offset )] )
#endif

#if ( PSP_BYTE_ORDER_BE == 1 )
 #define PSP_WR_32BITARRAY_OFFSET( pointer, offset, arr_length, val32 ) \
  ( ( pointer )[( ( arr_length ) - 1U ) - ( offset )] = ( val32 ) )
#else
 #define PSP_WR_32BITARRAY_OFFSET( pointer, offset, arr_length, val32 ) \
  ( ( pointer )[( offset )] = ( val32 ) )
#endif

#if ( PSP_BYTE_ORDER_BE == 1 )
 #define PSP_RD_8BITARRAY_OFFSET( pointer, offset, arr_length ) \
  ( ( pointer )[( ( arr_length ) - 1U ) - ( offset )] )
#else
 #define PSP_RD_8BITARRAY_OFFSET( pointer, offset, arr_length ) \
  ( ( pointer )[( offset )] )
#endif

#if ( PSP_BYTE_ORDER_BE == 1 )
 #define PSP_WR_8BITARRAY_OFFSET( pointer, offset, arr_length, val8 ) \
  ( ( pointer )[( ( arr_length ) - 1U ) - ( offset )] = ( val8 ) )
#else
 #define PSP_WR_8BITARRAY_OFFSET( pointer, offset, arr_length, val8 ) \
  ( ( pointer )[( offset )] = ( val8 ) )
#endif

#if ( PSP_BYTE_ORDER_BE == 1 )
 #define PSP_MOVE_8BITARRAY_LEFT( p, shift )    \
  ( &( ( p )[-( shift )] ) )
#else
 #define PSP_MOVE_8BITARRAY_LEFT( p, shift )    \
  ( &( ( p )[( shift )] ) )
#endif

#if ( PSP_BYTE_ORDER_BE == 1 )
 #define PSP_GET_LSHALF_ARRAY( pointer, len, buf_len ) \
  ( &( ( pointer )[( ( buf_len ) - 1U ) - ( ( ( len ) / 2U ) - 1U )] ) )
#else
 #define PSP_GET_LSHALF_ARRAY( pointer, len, buf_len ) \
  ( pointer )
#endif

#if ( PSP_BYTE_ORDER_BE == 1 )
 #define PSP_GET_MSHALF_ARRAY( pointer, len, buf_len ) \
  ( &( ( pointer )[( ( buf_len ) - 1U ) - ( ( len ) - 1U )] ) )
#else
 #define PSP_GET_MSHALF_ARRAY( pointer, len, buf_len ) \
  ( &( ( pointer )[( len ) / 2U] ) )
#endif

#if ( PSP_BYTE_ORDER_BE == 1 )
 #define PSP_SET_BIT( pointer, buf_len, bit ) \
  ( ( pointer )[( ( buf_len ) - 1U ) - ( ( bit ) >> PSP_ARRAY_BITS_BYTES_CONV )] |= ( 0x01u << ( ( bit ) & PSP_ARRAY_LSB_111 ) ) )
#else
 #define PSP_SET_BIT( pointer, buf_len, bit ) \
  ( ( pointer )[( bit ) >> PSP_ARRAY_BITS_BYTES_CONV] |= ( 0x01U << ( ( bit ) & PSP_ARRAY_LSB_111 ) ) )
#endif

#endif /* ifndef _PSP_ARRAY_H_ */
