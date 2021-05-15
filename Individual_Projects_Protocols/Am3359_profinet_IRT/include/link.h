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
/*  Component           : list manager                                       */
/* $Workfile:: link.h                                                       $*/
/* $Revision:: 8                                                            $*/
/* $Date:: 3/12/15 16:02                                                    $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef LINK_H
#define LINK_H

/*****************************************************************************/
/* link */
/*****************************************************************************/
typedef struct _ST_PN_LINK ST_PN_LINK, APP_FAR * PST_PN_LINK ;
typedef struct _ST_DESC_LINK ST_DESC_LINK, * PST_DESC_LINK , APP_FAR * LPST_DESC_LINK;

struct _ST_PN_LINK
{
  PST_PN_LINK pNext;
  PST_PN_LINK pPrev;
  PST_DESC_LINK pParent;
};

struct _ST_DESC_LINK
{
  ST_PN_LINK  stLink;
  APP_DWORD dwOffset;
  APP_WORD wPhase;
  APP_WORD wNb;
};

/*****************************************************************************/
/* Functions */
/*****************************************************************************/
/* link methods */
APP_VOID InitLink        (PST_PN_LINK   pstLink);
APP_VOID InitDescLink    (PST_DESC_LINK pstParent);
PST_DESC_LINK RemoveLink (PST_PN_LINK   pstLink);
PST_PN_LINK PopLink      (PST_DESC_LINK pstParent);
APP_VOID PushLink        (PST_DESC_LINK pstParent,PST_PN_LINK pstLink);

APP_BOOL IsEmptyDescLink (PST_DESC_LINK pstParent);
APP_BOOL IsTailLink      (PST_PN_LINK   pstLink);
PST_PN_LINK GetHeadLink  (PST_DESC_LINK pstParent);
PST_PN_LINK GetLinkNext  (PST_PN_LINK   pstLink);
APP_VOID moveLinkItem    (LPST_DESC_LINK lpstSrc, LPST_DESC_LINK lpstDst);

/*****************************************************************************/
#else
/*  #error "Re-Include ERROR" */
#endif /* __DEF_TOOLS_H */
