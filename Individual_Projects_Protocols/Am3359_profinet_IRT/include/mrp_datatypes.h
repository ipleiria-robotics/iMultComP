/***************************************************************************************
 *********** Copyright 2005 Zurich University of Applied Sciences / InES  **************
 ***************************************************************************************
 **
 **  File             : mrp_datatypes.h
 **
 **  Description      : This file defines the MRP data types.
 **
 ***************************************************************************************
 **  Department       : Institute of Embedded Systems
 **  Project          : MRP
 **  Modul            : MRP Datatypes
 **  Author           : Christian Schaer
 **  Contact          : scc@zhwin.ch
 **  Date             : 10.04.2006
 ****************************** Modifications ******************************************
 **
 **  Date  :    Who  :  Comment  :
 **  10.05.06   scc    initial release
 **************************************************************************************/
/* $Workfile:: mrp_datatypes.h                                              $*/
/* $Revision:: 4                                                            $*/
/* $Date:: 23/06/11 11:34                                                   $*/

#ifndef MRP_DATATYPES_H
#define MRP_DATATYPES_H

#include <stddef.h>
#include "sockuser.h"

#if  !defined(HAVE_LONG_LONG)
#define  HAVE_LONG_LONG    0
#endif

#if  !defined(SIZEOF_INT)
#define  SIZEOF_INT    4
#endif

#if  !defined(SIZEOF_LONG)
#define  SIZEOF_LONG    4
#endif

/************************************************************************/
#if SIZEOF_INT == 4
   typedef unsigned int       Unsigned32;
   typedef unsigned short    Unsigned16;
   typedef signed   int     Signed32;
   typedef unsigned char     UChar;
   typedef unsigned char    Unsigned8;
#else
   #error No suitable types
#endif

typedef unsigned char MRPBoolean;
    

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#endif
