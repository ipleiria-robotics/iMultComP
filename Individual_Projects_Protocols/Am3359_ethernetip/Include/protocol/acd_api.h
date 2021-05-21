/*
+------------------------------------------------------------------------------+
| Woodhead Software & Electronics (molex)                                             |
| -------------------------------                                              |
| Copyright 2011 (c)                                                           |
| Woodhead Software & Electronics, a Division of Molex. All rights reserved.   |
+------------------------------------------------------------------------------+
  Description .......: ACD


+------------------------------------------------------------------------------+
  File Name..........: acd_api.h
+------------------------------------------------------------------------------+
  Version | Date        | Author			           | Comments
  1.0.0     01/09/2011   Jean-Yves BEAUGRAND		    ACD api layer
+------------------------------------------------------------------------------+
*/
#ifndef __ACD_API_HEADER__
#define __ACD_API_HEADER__



/*****************************************************************************/
#define ACD_CMD_TASK_FINALYZE             1
#define ACD_CMD_START		                  2
#define ACD_CMD_STOP		                  3
#define ACD_CMD_QC_STARTED	              4
#define ACD_CMD_LINK		                  5
#define ACD_CMD_INC_TICK	                6
#define ACD_CMD_RCV_ARP_FRAME	            7

/*****************************************************************************/
#define BUF_PARAM_SIZE                200
#define NUM_MSG_ACD_QUEUE             100

/* Task Define */
#define ACD_TASK_PRIORITY	            5
#define ACD_TASK_OPTIONS              0
#define ACD_TASK_STACK_SIZE           20000

/*****************************************************************************/
typedef struct
{
  EIP_WORD wCmd;
  EIP_BYTE byBufParam[BUF_PARAM_SIZE];
} ST_ACD;

/*****************************************************************************/

ACD_API	EIP_BOOL ACD_Init(EIP_VOID);
ACD_API	EIP_BOOL ACD_Exit(EIP_VOID);

ACD_API	EIP_BOOL ACD_Start(EIP_DWORD dwIpAddress, EIP_BYTE byMacAddress[6], EIP_DWORD dwOptions);
ACD_API	EIP_BOOL ACD_Stop(EIP_VOID);
ACD_API	EIP_BOOL ACD_QC_Started(EIP_VOID);
ACD_API	EIP_BOOL ACD_Link(EIP_BYTE byPort, EIP_BYTE byPortState);
ACD_API	EIP_BOOL ACD_IncTick(EIP_VOID);
ACD_API	EIP_BOOL ACD_RcvArpFrame(EIP_WORD wFrameSize, EIP_BYTE * pbyFrame);
/*****************************************************************************/
#endif
