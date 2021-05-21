/*
+-------------------------------------------------------------------------------+
| Woodhead Software & Electronics                                               |
| -------------------------------                                               |
|                                                                               |
|  Copyright 2001-2008 (c) Woodhead Software & Electronics. All rights reserved.|
+-------------------------------------------------------------------------------+
  Description .......: Includes specific to an OS
                       
+-------------------------------------------------------------------------------+
  $Workfile: os_inc.h $
  $Revision: 12 $
  $Date: 18/10/10 10:40 $
+-------------------------------------------------------------------------------+
  Version | Date     | Author | Comments
						mm/dd/yy
  1.0.0     09/07/08   C.D.     Creation
+-------------------------------------------------------------------------------+
*/

#ifndef __OS_INCLUDE_H_
#define __OS_INCLUDE_H_

//*****************************************************************************

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <signal.h>

#include <string.h>
#include <stdlib.h>

#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Mailbox.h>


#include <ti/ndk/inc/netmain.h>
#include <ti/ndk/inc/tools/servers.h>
#include <ti/ndk/inc/socket.h>


#include <xdc/cfg/global.h>



//*****************************************************************************
#endif
