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
/*  Component           : Tools                                              */
/* $Workfile:: pool.h                                                       $*/
/* $Revision:: 7                                                            $*/
/* $Date:: 19/08/14 11:16                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*   manage a pool of ethernet frame                                         */
/*   - All ressources allocated in init                                      */
/*   - Allow to do a resrvation of 1 resources (lock)                        */
/*---------------------------------------------------------------------------*/

#ifndef __POOL_H__
#define __POOL_H__

typedef struct _pool_item
{
  APP_LPVOID   lpstNextElement; /* Newt elements is a LPST_POOL_ITEM pointer */
  APP_DWORD    dwLineLastAction;
  APP_BYTE     byLocked;
  /* Followed by byteData[wSizeElts */
}ST_POOL_ITEM, APP_FAR *LPST_POOL_ITEM;

typedef struct _pool{
  LPST_POOL_ITEM lpstFirstElement;    /* First element of the queue. APP_NULL If queue is empty */
  LPST_POOL_ITEM lpstLastElement;     /* Last element of the queue. APP_NULL If queue is empty */
  LPST_POOL_ITEM *lpstArrayBase;
  APP_WORD       wCount; /*size*/
  APP_WORD       wSizeElts;
  APP_WORD       wMaxElts;
}ST_POOL,APP_FAR *LPST_POOL;

/* ------------- Public Interface -------------------- */
#define pool_lock_elt(_lpPool, _ppvData)                                     pool_lock_elt_d(_lpPool, _ppvData, __LINE__)
#define pool_unlock_elt(_lpPool, _pvData)                                    pool_unlock_elt_d(_lpPool, _pvData, __LINE__)
#define pool_unlock_elt_at_end(_lpPool, _lpbyData)                           pool_unlock_elt_at_end_d(_lpPool, _lpbyData, __LINE__)
#define pool_delete_elements(_lpPool, _lpbyPreviousElements, _lpbyElements)  pool_delete_elements_d(_lpPool, _lpbyPreviousElements, _lpbyElements, __LINE__)

APP_WORD   pool_init               (LPST_POOL lpPool, APP_WORD wMaxElt, APP_WORD wSizeElt);
APP_WORD   pool_init_empty_pool    (LPST_POOL lpPool, APP_WORD wMaxElt, APP_WORD wSizeElt);
APP_VOID   pool_destroy            (LPST_POOL lpPool);
APP_WORD   pool_lock_elt_d         (LPST_POOL lpPool, APP_LPBYTE APP_FAR * ppvData, APP_DWORD dwLine);
APP_WORD   pool_unlock_elt_d       (LPST_POOL lpPool, APP_LPBYTE pvData, APP_DWORD dwLine);
APP_WORD   pool_unlock_elt_at_end_d(LPST_POOL lpPool, APP_LPBYTE lpbyData, APP_DWORD dwLine);
APP_WORD   pool_delete_elements_d  (LPST_POOL lpPool, APP_LPBYTE lpbyPreviousElements, APP_LPBYTE lpbyElements, APP_DWORD dwLine);
APP_LPBYTE pool_get_next           (LPST_POOL lpPool, APP_LPBYTE lpbyPreviousData);

#define  pool_nbfree(pPool)   ((pPool)->wCount)
#define  pool_nbused(pPool)   ((pPool)->wMaxElts-(pPool)->wCount)
#define  pool_sizeelt(pPool)  ((pPool)->wSizeElts)
#define  pool_maxframe(pPool) ((pPool)->wMaxElts)
#define  pool_isfree(pPool)   ((((pPool)->wCount)) != 0)

#endif
