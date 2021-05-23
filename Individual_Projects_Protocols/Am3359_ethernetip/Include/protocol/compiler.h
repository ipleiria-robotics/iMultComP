/*
+-------------------------------------------------------------------------------+
| Woodhead Software & Electronics                                               |
| -------------------------------                                               |
|                                                                               |
|  Copyright 2001-2006 (c) Woodhead Software & Electronics. All rights reserved.|
+-------------------------------------------------------------------------------+
  Description .......: Defines specific to a compiler
                       
+-------------------------------------------------------------------------------+
  $Workfile: compiler.h $
  $Revision: 12 $
  $Date: 21/07/10 16:30 $
+-------------------------------------------------------------------------------+
  Version | Date     | Author | Comments
						mm/dd/yy
  1.0.0     02/19/08   C.D.     Creation
+-------------------------------------------------------------------------------+
*/

#ifndef __COMPILER_HEADER__
#define __COMPILER_HEADER__

/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/

#define FIELD_PACKED
#define STRUCT_PACKED __attribute__ ((packed, aligned(1)))

/*---------------------------------------------------------------------------*/

#endif