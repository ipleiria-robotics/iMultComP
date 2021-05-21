/*
+------------------------------------------------------------------------------+
| Woodhead Software & Electronics (molex)                                             |
| -------------------------------                                              |
| Copyright 2011 (c)                                                           |
| Woodhead Software & Electronics, a Division of Molex. All rights reserved.   |
+------------------------------------------------------------------------------+
  Description .......: ACD


+------------------------------------------------------------------------------+
  File Name..........: user_acd.h
+------------------------------------------------------------------------------+
  Version | Date        | Author			           | Comments
  1.0.0     01/09/2011   Jean-Yves BEAUGRAND		    ACD user layer
+------------------------------------------------------------------------------+
*/

#ifndef __USER_ACD_HEADER__
#define __USER_ACD_HEADER__

/*****************************************************************************/

#define ACD_EVENT_LINK_INTEGRITY		    1
#define ACD_EVENT_NO_LINK			        2
#define ACD_EVENT_IP_LOST			        3
#define ACD_EVENT_IP_ANNOUNCED              4
#define ACD_EVENT_DEFENSE                   5

#define ACD_ACTIVITY_NO_CONFLICT            0
#define ACD_ACTIVITY_PROBE_IPV4_ADDRESS     1
#define ACD_ACTIVITY_ON_GOING_DETECTION     2
#define ACD_ACTIVITY_SEMI_ACTIVE_PROBE      3

typedef struct
{
  EIP_BYTE byAcdActivity;
  EIP_BYTE byRemoteMAC[6];
  EIP_BYTE byArpPdu[28];
} ST_ACD_CONFLICT_DETECTED;

/*****************************************************************************/
EIP_BOOL USER_ACD_Event(EIP_DWORD dwEvent);
EIP_BOOL USER_ACD_ConflictDetected(ST_ACD_CONFLICT_DETECTED stAcdConflictDetected);
EIP_BOOL USER_ACD_SendArpFrame(EIP_WORD wFrameSize, EIP_BYTE * byFrame);
EIP_BOOL USER_ACD_SIM_Print(EIP_DWORD dwFlag, EIP_CHAR * lpString);

/*****************************************************************************/
#endif
