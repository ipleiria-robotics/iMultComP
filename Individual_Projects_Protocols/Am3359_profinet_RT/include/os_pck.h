/*---------------------------------------------------------------------------*/
/*  Copyright (C) 2006 Woodhead Software & Electonics. All rights reserved.  */
/*---------------------------------------------------------------------------*/
/*  This program is protected by international copyright.                    */
/*                                                                           */
/*  The use of this software including but not limited to its Source Code    */
/*  is subject to restrictions as agreed in the license agreement between    */
/*  you and Woodhead.                                                        */
/*  Copying or distribution is not allowed unless expressly permitted        */
/*  according to your license agreement with Woodhead.                       */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  Project             : STACK PROFINET Controller                          */
/*  Component           : Hardware Interface                                 */
/* $Workfile:: os_pck.h                                                     $*/
/* $Revision:: 1                                                            $*/
/* $Date:: 13/04/16 17:17                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*  OS almignment & packing                                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef _OS_PACK_H
#define _OS_PACK_H

/* Sanity check */
#ifdef PACK
  #pragma message ("WARNING: Packing problem detected")
#endif

#define PACK

#define PACK_ALIGNEMENT(p) p
#define STRUCT_ALIGNEMENT  __attribute__ ((packed, aligned(1)))

/*substitute DWORDs with two WORDs for alignment request*/
#ifdef FIX_DWORD_ALIGNMENT
  #define ALIGNED_DWORD_DECL(dw) APP_WORD dw##_w1; APP_WORD dw##_w2
  #define APP_MEM_ALIGNMENT 4
  #define APP_MEM_ALIGN_SIZE(size) (((size) + APP_MEM_ALIGNMENT - 1) & ~(APP_MEM_ALIGNMENT-1))
#else
  #define ALIGNED_DWORD_DECL(dw) APP_DWORD dw
  #define APP_MEM_ALIGN_SIZE(size) (size)
#endif
/*define check to make sure the structures have the correct size
  it is recommended to enable for the first compilation and turn off if
  no change to suppress many static char definition in object file*/
#ifndef STATIC_STRUCT_SIZE_CHECK
/*#define STATIC_STRUCT_SIZE_CHECK*/
#endif

/*macro for compile-time structure size verification*/
#ifdef STATIC_STRUCT_SIZE_CHECK
  #define STRUCT_SIZE_CHECK(s, l) static char* p_##s = (sizeof(s) != l);
#else
  #define STRUCT_SIZE_CHECK(s, l)
#endif

#undef _OS_PACK_H
#endif
