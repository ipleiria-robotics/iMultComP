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
#ifndef _PSP_ENDIANNESS_H
#define _PSP_ENDIANNESS_H

#include "psp_types.h"

#include "../../version/ver_psp_endianness.h"
#if VER_PSP_ENDIANNESS_MAJOR != 1 || VER_PSP_ENDIANNESS_MINOR != 7
 #error Incompatible PSP_ENDIANNESS version number!
#endif


/* little endian read macros */

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_RD_LE16( p )                        \
  (uint16_t)( ( (uint16_t)( ( p )[1] ) << 8u )  \
            + ( (uint16_t)( ( p )[0] ) ) )

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_RD_LE24( p )                        \
  (uint32_t)( ( (uint32_t)( ( p )[2] ) << 16u ) \
            + ( (uint32_t)( ( p )[1] ) << 8u )  \
            + ( (uint32_t)( ( p )[0] ) ) )

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_RD_LE32( p )                        \
  (uint32_t)( ( (uint32_t)( ( p )[3] ) << 24u ) \
            + ( (uint32_t)( ( p )[2] ) << 16u ) \
            + ( (uint32_t)( ( p )[1] ) << 8u )  \
            + ( (uint32_t)( ( p )[0] ) ) )

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_RD_LE48( p )                        \
  (uint64_t)( ( (uint64_t)( ( p )[5] ) << 40u ) \
            + ( (uint64_t)( ( p )[4] ) << 32u ) \
            + ( (uint64_t)( ( p )[3] ) << 24u ) \
            + ( (uint64_t)( ( p )[2] ) << 16u ) \
            + ( (uint64_t)( ( p )[1] ) << 8u )  \
            + ( (uint64_t)( ( p )[0] ) ) )

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_RD_LE64( p )                        \
  (uint64_t)( ( (uint64_t)( ( p )[7] ) << 56u ) \
            + ( (uint64_t)( ( p )[6] ) << 48u ) \
            + ( (uint64_t)( ( p )[5] ) << 40u ) \
            + ( (uint64_t)( ( p )[4] ) << 32u ) \
            + ( (uint64_t)( ( p )[3] ) << 24u ) \
            + ( (uint64_t)( ( p )[2] ) << 16u ) \
            + ( (uint64_t)( ( p )[1] ) << 8u )  \
            + ( (uint64_t)( ( p )[0] ) ) )



/* little endian write macros */

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_WR_LE16( p, v )                                                 \
  do                                                                        \
  {                                                                         \
    ( ( p )[0] ) = ( (uint8_t)( ( (uint16_t)( v ) ) & UINT8_MAX ) );        \
    ( ( p )[1] ) = ( (uint8_t)( ( (uint16_t)( v ) >> 8u ) & UINT8_MAX ) );  \
  }                                                                         \
  while ( 0 )

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_WR_LE24( p, v )                                                 \
  do                                                                        \
  {                                                                         \
    ( ( p )[0] ) = ( (uint8_t)( ( (uint32_t)( v ) ) & UINT8_MAX ) );        \
    ( ( p )[1] ) = ( (uint8_t)( ( (uint32_t)( v ) >> 8u ) & UINT8_MAX ) );  \
    ( ( p )[2] ) = ( (uint8_t)( ( (uint32_t)( v ) >> 16u ) & UINT8_MAX ) ); \
  }                                                                         \
  while ( 0 )

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_WR_LE32( p, v )                                                 \
  do                                                                        \
  {                                                                         \
    ( ( p )[0] ) = ( (uint8_t)( ( (uint32_t)( v ) ) & UINT8_MAX ) );        \
    ( ( p )[1] ) = ( (uint8_t)( ( (uint32_t)( v ) >> 8u ) & UINT8_MAX ) );  \
    ( ( p )[2] ) = ( (uint8_t)( ( (uint32_t)( v ) >> 16u ) & UINT8_MAX ) ); \
    ( ( p )[3] ) = ( (uint8_t)( ( (uint32_t)( v ) >> 24u ) & UINT8_MAX ) ); \
  }                                                                         \
  while ( 0 )

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_WR_LE48( p, v )                                                 \
  do                                                                        \
  {                                                                         \
    ( ( p )[0] ) = ( (uint8_t)( ( (uint64_t)( v ) ) & UINT8_MAX ) );        \
    ( ( p )[1] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 8u ) & UINT8_MAX ) );  \
    ( ( p )[2] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 16u ) & UINT8_MAX ) ); \
    ( ( p )[3] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 24u ) & UINT8_MAX ) ); \
    ( ( p )[4] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 32u ) & UINT8_MAX ) ); \
    ( ( p )[5] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 40u ) & UINT8_MAX ) ); \
  }                                                                         \
  while ( 0 )

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_WR_LE64( p, v )                                                 \
  do                                                                        \
  {                                                                         \
    ( ( p )[0] ) = ( (uint8_t)( ( (uint64_t)( v ) ) & UINT8_MAX ) );        \
    ( ( p )[1] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 8u ) & UINT8_MAX ) );  \
    ( ( p )[2] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 16u ) & UINT8_MAX ) ); \
    ( ( p )[3] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 24u ) & UINT8_MAX ) ); \
    ( ( p )[4] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 32u ) & UINT8_MAX ) ); \
    ( ( p )[5] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 40u ) & UINT8_MAX ) ); \
    ( ( p )[6] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 48u ) & UINT8_MAX ) ); \
    ( ( p )[7] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 56u ) & UINT8_MAX ) ); \
  }                                                                         \
  while ( 0 )



/* big endian read macros */

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_RD_BE16( p )                        \
  (uint16_t)( ( (uint16_t)( ( p )[0] ) << 8u )  \
            + ( (uint16_t)( ( p )[1] ) ) )

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_RD_BE24( p )                        \
  (uint32_t)( ( (uint32_t)( ( p )[0] ) << 16u ) \
            + ( (uint32_t)( ( p )[1] ) << 8u )  \
            + ( (uint32_t)( ( p )[2] ) ) )

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_RD_BE32( p )                        \
  (uint32_t)( ( (uint32_t)( ( p )[0] ) << 24u ) \
            + ( (uint32_t)( ( p )[1] ) << 16u ) \
            + ( (uint32_t)( ( p )[2] ) << 8u )  \
            + ( (uint32_t)( ( p )[3] ) ) )

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_RD_BE48( p )                        \
  (uint64_t)( ( (uint64_t)( ( p )[0] ) << 40u ) \
            + ( (uint64_t)( ( p )[1] ) << 32u ) \
            + ( (uint64_t)( ( p )[2] ) << 24u ) \
            + ( (uint64_t)( ( p )[3] ) << 16u ) \
            + ( (uint64_t)( ( p )[4] ) << 8u )  \
            + ( (uint64_t)( ( p )[5] ) ) )

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_RD_BE64( p )                        \
  (uint64_t)( ( (uint64_t)( ( p )[0] ) << 56u ) \
            + ( (uint64_t)( ( p )[1] ) << 48u ) \
            + ( (uint64_t)( ( p )[2] ) << 40u ) \
            + ( (uint64_t)( ( p )[3] ) << 32u ) \
            + ( (uint64_t)( ( p )[4] ) << 24u ) \
            + ( (uint64_t)( ( p )[5] ) << 16u ) \
            + ( (uint64_t)( ( p )[6] ) << 8u )  \
            + ( (uint64_t)( ( p )[7] ) ) )



/* big endian write macros */

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_WR_BE16( p, v )                                                 \
  do                                                                        \
  {                                                                         \
    ( ( p )[1] ) = ( (uint8_t)( ( (uint16_t)( v ) ) & UINT8_MAX ) );        \
    ( ( p )[0] ) = ( (uint8_t)( ( (uint16_t)( v ) >> 8u ) & UINT8_MAX ) );  \
  }                                                                         \
  while ( 0 )

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_WR_BE24( p, v )                                                 \
  do                                                                        \
  {                                                                         \
    ( ( p )[2] ) = ( (uint8_t)( ( (uint32_t)( v ) ) & UINT8_MAX ) );        \
    ( ( p )[1] ) = ( (uint8_t)( ( (uint32_t)( v ) >> 8u ) & UINT8_MAX ) );  \
    ( ( p )[0] ) = ( (uint8_t)( ( (uint32_t)( v ) >> 16u ) & UINT8_MAX ) ); \
  }                                                                         \
  while ( 0 )

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_WR_BE32( p, v )                                                 \
  do                                                                        \
  {                                                                         \
    ( ( p )[3] ) = ( (uint8_t)( ( (uint32_t)( v ) ) & UINT8_MAX ) );        \
    ( ( p )[2] ) = ( (uint8_t)( ( (uint32_t)( v ) >> 8u ) & UINT8_MAX ) );  \
    ( ( p )[1] ) = ( (uint8_t)( ( (uint32_t)( v ) >> 16u ) & UINT8_MAX ) ); \
    ( ( p )[0] ) = ( (uint8_t)( ( (uint32_t)( v ) >> 24u ) & UINT8_MAX ) ); \
  }                                                                         \
  while ( 0 )

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_WR_BE48( p, v )                                                 \
  do                                                                        \
  {                                                                         \
    ( ( p )[5] ) = ( (uint8_t)( ( (uint64_t)( v ) ) & UINT8_MAX ) );        \
    ( ( p )[4] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 8u ) & UINT8_MAX ) );  \
    ( ( p )[3] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 16u ) & UINT8_MAX ) ); \
    ( ( p )[2] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 24u ) & UINT8_MAX ) ); \
    ( ( p )[1] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 32u ) & UINT8_MAX ) ); \
    ( ( p )[0] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 40u ) & UINT8_MAX ) ); \
  }                                                                         \
  while ( 0 )

/* 340 S : MISRA-C:2004 19.7: Use of function like macro. */
/*LDRA_INSPECTED 340 S*/
#define PSP_WR_BE64( p, v )                                                 \
  do                                                                        \
  {                                                                         \
    ( ( p )[7] ) = ( (uint8_t)( ( (uint64_t)( v ) ) & UINT8_MAX ) );        \
    ( ( p )[6] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 8u ) & UINT8_MAX ) );  \
    ( ( p )[5] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 16u ) & UINT8_MAX ) ); \
    ( ( p )[4] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 24u ) & UINT8_MAX ) ); \
    ( ( p )[3] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 32u ) & UINT8_MAX ) ); \
    ( ( p )[2] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 40u ) & UINT8_MAX ) ); \
    ( ( p )[1] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 48u ) & UINT8_MAX ) ); \
    ( ( p )[0] ) = ( (uint8_t)( ( (uint64_t)( v ) >> 56u ) & UINT8_MAX ) ); \
  }                                                                         \
  while ( 0 )

#endif /* ifndef _PSP_ENDIANNESS_H_ */

