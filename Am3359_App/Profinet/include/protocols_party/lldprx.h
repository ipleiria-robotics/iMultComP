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
/* $Workfile:: lldprx.h                                                      $*/
/* $Revision:: 3                                                             $*/
/* $Date:: 29/07/14 15:50                                                    $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*  - LLDP Receiver header                                                   */
/*---------------------------------------------------------------------------*/
#ifndef _LLDPRX_H
#define _LLDPRX_H

#ifdef LLDP_RECEIVER

/* Rx state machine */
#define ST_RX_LLDP_WAIT_PORT_OPERATIONNAL       0
#define ST_RX_LLDP_INITIALIZE                   1
#define ST_RX_LLDP_DELETE_AGED_INFO             2
#define ST_RX_LLDP_RX_WAIT_FOR_FRAME            3
#define ST_RX_LLDP_RX_FRAME                     4
#define ST_RX_LLDP_DELETE_INFO                  5
#define ST_RX_LLDP_UPDATE_INFO                  6

/* LLDP Rx Stat */
typedef struct _LLDP_RX_STATS {

  APP_DWORD dwStatsAgeOutsTotal;          /* 802.1AB 10.5.2.2*/
  APP_DWORD dwStatsFramesDiscardedTotal;  /* 802.1AB 10.5.2.2*/
  APP_DWORD dwStatsFramesInErrorsTotal;   /* 802.1AB 10.5.2.2*/
  APP_DWORD dwStatsFramesInTotal;         /* 802.1AB 10.5.2.2*/
  APP_DWORD dwStatsTLVsDiscardedTotal;    /* 802.1AB 10.5.2.2*/
  APP_DWORD dwStatsTLVsUnrecognizedTotal; /* 802.1AB 10.5.2.2*/
}ST_LLDP_RX_STATS, APP_FAR * LPST_LLDP_RX_STATS;

/* LLDP Rx Timers */
typedef struct _LLDP_RX_TIMERS {

  APP_WORD  wRxInfoTTL;             /* 802.1AB 10.5.3.2*/
  APP_WORD  wTooManyNeighborsTimer; /* 802.1AB 10.5.3.2*/
}ST_LLDP_RX_TIMERS, APP_FAR * LPST_LLDP_RX_TIMERS;

/* LLDP RX State Machine */
typedef struct _LLDP_RX_SM {

  /* 802.1AB 10.5.5.1*/
  APP_BYTE           byRxState;
  APP_BOOL           bBadFrame;              /* 802.1AB 10.5.5.1*/
  APP_BOOL           bRcvFrame;              /* 802.1AB 10.5.5.1*/
  APP_BOOL           bRxChanges;             /* 802.1AB 10.5.5.1*/
  APP_BOOL           bRxInfoAge;             /* 802.1AB 10.5.5.1*/
  APP_WORD           wRxTTL;                 /* 802.1AB 10.5.5.1*/
  APP_BOOL           bTooManyNeighbors;      /* 802.1AB 10.5.5.1*/
  APP_BOOL           bSomethingChangeRemote; /* 802.1AB 10.5.5.1*/
  APP_LPBYTE         lpbyRxFrameBuffer;  /* Buffer containing received frame */
  APP_WORD           wRxFrameLen;        /* Len of received frame */
  ST_LLDP_TLV        stRxTLV[MAX_TLV_PER_LLDPDU];          /* List of received TLVs */
  ST_LLDP_RX_STATS   stLldpRxStats;
  ST_LLDP_RX_TIMERS  stLLDPRXTimers;
}ST_LLDP_RX_SM, APP_FAR * LPST_LLDP_RX_SM;

#endif

#endif
