/*
+-------------------------------------------------------------------------------+
| Woodhead Software & Electronics                                               |
| -------------------------------                                               |
|                                                                               |
|  Copyright 2004-2008 (c) Woodhead Software & Electronics. All rights reserved.|
+-------------------------------------------------------------------------------+
  Description .......: Timer implementation                                                         
                       
+-------------------------------------------------------------------------------+
  $Workfile: user_timer.h $
  $Revision: 8 $
  $Date: 10/12/10 14:21 $
+-------------------------------------------------------------------------------+
  Version | Date     | Author | Comments
			mm/dd/yy
  1.0.0		  09/07/08  C.Dupuit  Initial implementation
+-------------------------------------------------------------------------------+
*/

#ifndef __USER_TIMER_H_
#define __USER_TIMER_H_
//*****************************************************************************

/* Time in µs of one Tick – how often EIP_Process API is called */
#define TICK_PERIOD      500

/* Number of tick the Production timer will execute */
#define TIMER_TICK_IO_PROD_X         1
/* Number of tick the Consumption timer will execute */
#define TIMER_TICK_IO_CONS_X         4
/* Number of tick the explicit message timer will execute */
#define TIMER_TICK_EM_X             10
/* Number of tick the connections timer will execute */
#define TIMER_TICK_CONNECTIONS     100
/* Number of tick the timer “slow” will execute */
#define TIMER_TICK_SLOW_TIMER      100
/* Production time offset for the first production on a IO connection */
#define PRODUCTION_START_OFFSET      4

/* Number of ticks to wait before to re open a connection in error (10 s) */
#define IO_CNXN_WAIT_AFTER_ERROR  (1000000 / (TICK_PERIOD * 10))

//*****************************************************************************
#endif
