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
/*  Component           : LLDP                                               */
/* $Workfile:: lldptx.h                                                     $*/
/* $Revision:: 6                                                            $*/
/* $Date:: 31/03/16 16:26                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*  -LLDP Tx state machine                                                   */
/*---------------------------------------------------------------------------*/

#ifndef _LLDPTX_H
#define _LLDPTX_H

#define SET_CURRENT_PTR(a, b)    (a)->stLldpTx.lpbyCurrentFrameBuffer = b
#define GET_CURRENT_PTR(a)       (a)->stLldpTx.lpbyCurrentFrameBuffer
#define UPDATE_CURRENT_PTR(a, b) (a)->stLldpTx.lpbyCurrentFrameBuffer = (a)->stLldpTx.lpbyCurrentFrameBuffer + b;\
                                 (a)->stLldpTx.wTxFrameLen = (APP_WORD)((a)->stLldpTx.wTxFrameLen + b)



#define GET_BASE_BUF_PTR(a)      (a)->stLldpTx.lpbyTxFrameBuffer
#define GET_BUF_LEN(a)           (a)->stLldpTx.wTxFrameLen
#define CLEAR_BUF_LEN(a)         (a)->stLldpTx.wTxFrameLen = 0



/* Tx state machine */
#define ST_TX_LLDP_INITIALIZE           0
#define ST_TX_LLDP_IDLE                 1
#define ST_TX_LLDP_INFO_FRAME           2
#define ST_TX_LLDP_SHUTDOWN_FRAME       3
#define ST_TX_LLDP_WAIT_SHUTDOWN_WHILE  4

#define DELTA_TIMER 10

/* LLDP Tx Timers */
typedef struct _LLDP_TX_TIMERS {

  APP_WORD  wTxShutdownWhile;             /* 802.1AB 10.5.3.1*/
  APP_WORD  wTxDelayWhile;                /* 802.1AB 10.5.3.1*/
  APP_WORD  wTxTTR;                       /* 802.1AB 10.5.3.1*/
}ST_LLDP_TX_TIMERS, APP_FAR * LPST_LLDP_TX_TIMERS;

/* LLDP Tx Stat */
typedef struct _LLDP_TX_STATS {

  APP_DWORD dwStatsFrameOutTotal;         /* 802.1AB 10.5.2.1*/
}ST_LLDP_TX_STATS, APP_FAR * LPST_LLDP_TX_STATS;

/* LLDP TX State Machine */
typedef struct _LLDP_TX_SM {
        /* 802.1AB 10.5.4.1*/
  APP_BYTE    byTxState;
  APP_BYTE    bySomethingChangeLocal;
  APP_WORD    wTxTTL;               /* 802.1AB 10.5.4.1 */
  APP_LPBYTE  lpbyTxFrameBuffer;
  APP_LPBYTE  lpbyCurrentFrameBuffer;
  APP_WORD    wTxFrameLen;

  ST_LLDP_TX_STATS   stLldpTxStats;
  ST_LLDP_TX_TIMERS  stLLDPTXTimers;
}ST_LLDP_TX_SM, APP_FAR * LPST_LLDP_TX_SM;


#endif




