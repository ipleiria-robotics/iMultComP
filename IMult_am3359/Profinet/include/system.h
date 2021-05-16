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
/*  Component           : Production                                         */
/* $Workfile:: system.h                                                     $*/
/* $Revision:: 6                                                            $*/
/* $Date:: 29/07/14 15:51                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/


#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#define OS_OK     1
#define OS_NOT_OK 0
#define OS_NULL   0

/* Ethernet Frame Maximal Length */
#define ETHERNET_MAX_FRAME_LENGTH  1518
#define MAC_ADDRESS_LEN 6

/* Maximal Data in production and consumption frame */
#define MAX_DATA_FRAME_IO          1440

/* Minimal Data in production and consumption frame */
#define MIN_DATA_FRAME_IO_RT_CLASS_1_2_3        40
#define MIN_DATA_FRAME_IO_RT_CLASS_UDP          12

#define INVALID                   0xffff

#define COUNT_OF(array) (sizeof(array)/sizeof(array[0]))

#endif
