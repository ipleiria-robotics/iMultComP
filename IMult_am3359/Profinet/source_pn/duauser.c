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
/*  Project             : STACK PROFINET User DUA add file                   */
/*  Component           :                                                    */
/* $Workfile:: duauser.c                                                    $*/
/* $Revision:: 5                                                            $*/
/* $Date:: 16/11/16 17:49                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*  Windows user application sample                                          */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#include <Profinet/include/os.h>
#include <Profinet/include/stackdev.h>
#include <Profinet/include/pncbdev.h>
#include <Profinet/include/rdcbdev.h>
#include <Profinet/include/dua.h>
#include <Profinet/include/duauser.h>

#include <Profinet/include/system.h>
#include <Profinet/include/wtrace.h>
#include <Profinet/include/stk_sckt.h>
#include <Profinet/include/os_sckt.h>
#include <Profinet/include/hwif.h>

#include <osal/MailboxP.h>
#include <osal/ClockP.h>
#include <Profinet/include/iPNDrv.h>

#include <Include/Board_am335x/board_spi.h>

#include <Profinet/include/iPNLegacy.h>

/* Flash header file */


#include <Include/Board_am335x/board_spi.h>

#include <Profinet/include/snmp_ndk_interface.h>

extern Board_flashHandle flashHandle;


extern ST_PND_CONF       gStPndConfiguration;
extern APP_BYTE          gbySyncMasterMac[MAC_ADDRESS_LEN];
extern APP_LPVOID        glpPhysicalDevice;

extern APP_WORD          gbyLinkState[WSE_LLDP_MAX_PORT];

#ifdef PN_RTC3_SUPPORTED
extern APP_DWORD         gdwCableDelayRemote[WSE_LLDP_MAX_PORT];
#endif

MailboxP_Handle gmbxUser = APP_NULL;

#define DUAUSER_TRACE(...) { /*char bufftoto[512]={0}; OS_SPRINTF(bufftoto, __VA_ARGS__); UARTPutString(uartInstance, bufftoto);*/}



/**
 * UserChangeName:
 *
 * @param  lpDeviceName
 * @param  wBlockQ
 * @return APP_VOID
 */
APP_VOID  UserChangeName(APP_LPCHAR lpDeviceName, APP_WORD wBlockQualifier)
{
  DUAUSER_TRACE("\n --> Name to set: %s \n",lpDeviceName);

  OS_MEMSET(gStPndConfiguration.stRemData.sName, 0, NAMELEN);
  gStPndConfiguration.stRemData.wNameLen = 0;

  if(1 == wBlockQualifier)
  {
    OS_MEMCOPY(gStPndConfiguration.stRemData.sName, lpDeviceName, OS_STRLEN(lpDeviceName));
    gStPndConfiguration.stRemData.wNameLen = OS_STRLEN(lpDeviceName);
  }

  setDcpFilterStationName((const uint8_t*)gStPndConfiguration.stRemData.sName, gStPndConfiguration.stRemData.wNameLen);

  UserSaveRemanentConf(&gStPndConfiguration.stRemData, APP_NULL);
}

/**
 * UserChangeIp:
 *
 * @param  dwIp
 * @param  dwSubMask
 * @param  dwGateway
 * @param  wBlockQualifier
 * @return APP_VOID
 */
APP_WORD  UserChangeIp(APP_DWORD dwIp, APP_DWORD dwSubMask, APP_DWORD dwGateway, APP_WORD wBlockQualifier)
{

  DUAUSER_TRACE("\n --> Ip param to set IP:0X%lX \n",dwIp);

  if(1 == wBlockQualifier)
  {
    gStPndConfiguration.stRemData.dwIpAddress  = dwIp;
    gStPndConfiguration.stRemData.dwSubNetMask = dwSubMask;
    gStPndConfiguration.stRemData.dwGateway    = dwGateway;
    gStPndConfiguration.stRemData.wIpMode      = PND_IP_STATIC;
  }
  else
  {
    gStPndConfiguration.stRemData.dwIpAddress  = 0;
    gStPndConfiguration.stRemData.dwSubNetMask = 0;
    gStPndConfiguration.stRemData.dwGateway    = 0;
    gStPndConfiguration.stRemData.wIpMode      = PND_IP_STATIC;
  }

  UserSetDeviceIP(dwIp, dwSubMask, dwGateway);
  UserSaveRemanentConf(&gStPndConfiguration.stRemData, APP_NULL);

  return OS_OK;
}

/**
 * DuaResetIp:
 *
 * @return APP_WORD OS_OK
 */
APP_WORD  UserResetIp(APP_VOID)
{
  UserChangeIp(0,0,0,0);
  return OS_OK;
}


APP_WORD UpdateIP(APP_DWORD dwIp, APP_DWORD dwSubMask)
{
  CI_IPNET NA;
  void *      hCfgIpAddr;
  int iResult = 0;

  /* Setup manual IP address */
  bzero(&NA, sizeof(NA));
  NA.IPAddr  = HTONL(dwIp);
  NA.IPMask  = HTONL(dwSubMask);

  strcpy(NA.Domain, "demo.net");
  NA.NetType = 0;

  /* get the current static IP entry */
  iResult = CfgGetEntry(0, CFGTAG_IPNET, 1, 1, &hCfgIpAddr);

  if(1==iResult)
  {
    /* remove the current static IP entry*/
    CfgRemoveEntry(0, hCfgIpAddr);
  }

  /* ip  = 0, no need to configure */
  if((0 == dwIp) && (0== dwSubMask))
    return OS_OK;

  /* add a new static IP entry */
  CfgAddEntry(0, CFGTAG_IPNET, 1, 0, sizeof(CI_IPNET), (uint8_t *)&NA, 0);

  return OS_OK;
}

APP_WORD UpdateGateway(APP_DWORD dwGateway)
{
  CI_ROUTE RT;
  void *      hCfgIpAddr;
  int iResult = 0;

  /* Setup manual IP address */
  bzero(&RT, sizeof(RT));
  RT.IPDestAddr  = 0;
  RT.IPDestMask  = 0;
  RT.IPGateAddr  = HTONL(dwGateway);

  /* get the current static IP entry */
  iResult = CfgGetEntry(0, CFGTAG_ROUTE, 1, 1, &hCfgIpAddr);

  if(1==iResult)
  {
    /* remove the current static IP entry*/
    CfgRemoveEntry(0, hCfgIpAddr);
  }

  /* ip  = 0, no need to configure */
  if((0 == dwGateway)){
    return OS_OK;
  }

  /* add a new static IP entry */
  iResult = CfgAddEntry(0, CFGTAG_ROUTE, 1, 0, sizeof(CI_ROUTE), (uint8_t *)&RT, 0);

  if(1!=iResult)
  {
    return OS_NOT_OK;
  }

  return OS_OK;
}


APP_WORD UserSetDeviceIP(APP_DWORD dwIp, APP_DWORD dwSubMask, APP_DWORD dwGateway)
{

  if(OS_OK != UpdateIP(dwIp, dwSubMask) || OS_OK != UpdateGateway(dwGateway))
  {
  return OS_NOT_OK;
  }

  return OS_OK;
}

APP_CHAR * UserGetInterfaceDeviceIPString()
{
  /* This shall be a valid IP on a valid interface*/
  return PND_ITF_IP_ADRESSE;
}

APP_DWORD UserGetDeviceIP(LPST_PND_REMANENT_DATA lpstRemData)
{
  /*
  we should use ip parameters from flash but on this os sample we don't change interface IP address.
  The IO-Device can have an IP on network that is not the true one (on interface)
  */
  return lpstRemData->dwIpAddress;
}

APP_DWORD UserGetDeviceMASK(LPST_PND_REMANENT_DATA lpstRemData)
{
  /*
  we should use ip parameters from flash but on this os sample we don't change interface IP address.
  The IO-Device can have an IP on network that is not the true one (on interface)
  */
  return lpstRemData->dwSubNetMask;
}

APP_DWORD UserGetDeviceGATEWAY(LPST_PND_REMANENT_DATA lpstRemData)
{
  /*
  we should use ip parameters from flash but on this os sample we don't change interface IP address.
  The IO-Device can have an IP on network that is not the true one (on interface)
  */
  return lpstRemData->dwGateway;
}


APP_DWORD UserLoadRemanentConf(LPST_PND_REMANENT_DATA lpstRemData)
{
   uint32_t ioMode;
   ioMode = BOARD_FLASH_QSPI_IO_MODE_QUAD;

   if(0 != Board_flashRead(flashHandle,SPI_EEPROM_DEVICECONF_OFFSET,(uint8_t*)lpstRemData,sizeof(ST_PND_REMANENT_DATA),(void *)(&ioMode)))
   {
       OS_ASSERT(APP_FALSE);
	   return OS_NOT_OK;
   }
   return OS_OK;
}


APP_DWORD UserResetToFactory(LPST_PND_REMANENT_DATA lpstRemData)
{
  UserChangeName("", 1);
  UserResetIp();
  SNMP_resetField();

  return OS_OK;
}

APP_DWORD UserFactoryReset(LPST_PND_REMANENT_DATA lpstRemData)
{
  DUAUSER_TRACE("UserFactoryReset \n\r");

  UserChangeName("", 1);
  UserResetIp();
  SNMP_resetField();

  // IM default values
  OS_MEMSET(lpstRemData->IM1_Tag_Function, 0x20, IM_Tag_Function_LEN);
  OS_MEMSET(lpstRemData->IM1_Tag_Location, 0x20, IM_Tag_Location_LEN);
  OS_MEMSET(lpstRemData->IM2_Date        , 0x20, IM_Date_LEN);
  OS_MEMSET(lpstRemData->IM3_Descriptor  , 0x20, IM_Descriptor_LEN);
  OS_MEMSET(lpstRemData->IM4_Signature   , 0x00, IM_Signature_LEN);

  return UserSaveRemanentConf(lpstRemData, APP_NULL);
}


APP_DWORD UserSetDefaultRemanentConf(LPST_PND_REMANENT_DATA lpstRemData)
{
  int i = 0;

  DUAUSER_TRACE("UserSetDefaultRemanentConf \n\r");

  OS_MEMSET(lpstRemData, 0, sizeof(ST_PND_REMANENT_DATA));
  OS_MEMCOPY( (lpstRemData->sName), PND_NAME_FROM_CONFIG, PND_NAME_SIZE_FROM_CONFIG);
  lpstRemData->dwVersion                  = REMANENT_DATA_VERSION; /*"1.0.0.1"*/
  lpstRemData->dwRemDataLen               = sizeof(ST_PND_REMANENT_DATA);
  lpstRemData->dwIpAddress                = 0;
  lpstRemData->wIpMode                    = PND_IP_STATIC;
  lpstRemData->dwSubNetMask               = 0xffff0000;
  lpstRemData->dwGateway                  = 0x00000000;
  lpstRemData->byFsuInitiativeDelayMode   = PND_INITIAVE_DELAY_MODE_OFF;
  lpstRemData->wHelloDelay                = PND_HELLO_DELAY_0100MS;
  lpstRemData->wHelloInterval             = PND_HELLO_INTERVAL_0030MS;
  lpstRemData->wHelloRetryCount           = 3;
  OS_MEMSET(lpstRemData->IM1_Tag_Function, 0x20, IM_Tag_Function_LEN);
  OS_MEMSET(lpstRemData->IM1_Tag_Location, 0x20, IM_Tag_Location_LEN);
  OS_MEMSET(lpstRemData->IM2_Date        , 0x20, IM_Date_LEN);
  OS_MEMSET(lpstRemData->IM3_Descriptor  , 0x20, IM_Descriptor_LEN);
  OS_MEMSET(lpstRemData->IM4_Signature   , 0x00, IM_Signature_LEN);

  lpstRemData->byIsAdjustFsuActivate    = APP_FALSE;
  lpstRemData->byItfAdjustEnable        = APP_FALSE;

  #ifdef PN_RTC3_SUPPORTED
  lpstRemData->byIsAdjustIrDataEnable   = APP_FALSE;
  lpstRemData->byIsAdjustSyncDataEnable = APP_FALSE;
  OS_MEMSET(&lpstRemData->stAdjustIrData, 0, sizeof(ST_PDIRDATA));
  OS_MEMSET(&lpstRemData->stAdjustSyncData, 0, sizeof(ST_PDSYNCDATA));
  #endif

  #ifdef MRP_SUPPORTED
  lpstRemData->byIsCheckMRPActivate   = APP_FALSE;
  lpstRemData->byIsAdjustMRPActivate  = APP_FALSE;
  #endif

  for(i = 0; i < PN_CONFIGURATION_PORT_SIZE; i++)
  {
    lpstRemData->stPortRemData[i].dwApi                                = APP_FALSE;
    lpstRemData->stPortRemData[i].wSlot                                = APP_FALSE;
    lpstRemData->stPortRemData[i].wSubSlot                             = APP_FALSE;
    lpstRemData->stPortRemData[i].byIsCheckPeerActivate                = APP_FALSE;
    lpstRemData->stPortRemData[i].byIsCheckMAUTypeModeActive           = APP_FALSE;
    lpstRemData->stPortRemData[i].byIsCheckMAUTypeActive               = APP_FALSE;
    lpstRemData->stPortRemData[i].byCheckLinkStateActivate             = APP_FALSE;
    lpstRemData->stPortRemData[i].byIsAdjustMAUTypeActivate            = APP_FALSE;
    lpstRemData->stPortRemData[i].byIsAdjustPeerToPeerBoundaryActivate = APP_FALSE;
    lpstRemData->stPortRemData[i].byIsAdjustDCPBoundaryActivate        = APP_FALSE;
    lpstRemData->stPortRemData[i].byIsAdjustPreambuleLengthActivate    = APP_FALSE;

    #ifdef MRP_SUPPORTED
    lpstRemData->stPortRemData[i].byIsAdjustMRPActivate                = APP_FALSE;
    #endif

    lpstRemData->stPortRemData[i].wCheckMAUTypeMode                    = 0;
    lpstRemData->stPortRemData[i].wAdjustMAUType                       = 0;
    lpstRemData->stPortRemData[i].wCheckMAUtType                       = 0x0010;

    #ifdef PN_RTC3_SUPPORTED
    lpstRemData->stPortRemData[i].byIsCheckLineDelayeActivate          = APP_FALSE;
    lpstRemData->stPortRemData[i].byCheckSyncModeActivate              = APP_FALSE;
    lpstRemData->stPortRemData[i].byIsAdjustDomainBoundaryActivate     = APP_FALSE;
    lpstRemData->stPortRemData[i].byIsAdjustLinkStateActivate          = APP_FALSE;
    lpstRemData->stPortRemData[i].byIsAdjustMulticastBoundaryActivate  = APP_FALSE;
    #endif
  }
  return UserSaveRemanentConf(lpstRemData, APP_NULL);
}

APP_DWORD UserSaveRemanentConf(LPST_PND_REMANENT_DATA lpstRemData, APP_LPBYTE lpbyDataChange)
{
  ST_PND_REMANENT_DATA stRemData;
  uint32_t blockNum, pageNum;
  uint32_t ioMode;

  if(OS_OK == UserLoadRemanentConf(&stRemData))
  {
    if(0 == OS_MEMCMP(lpstRemData, &stRemData, sizeof(ST_PND_REMANENT_DATA)))
    {
      /* same data nothing to do */
      if(APP_NULL != lpbyDataChange)
      {
        *lpbyDataChange = APP_FALSE;
      }
      return OS_OK;
    }
  }

  if(APP_NULL != lpbyDataChange)
  {
    *lpbyDataChange = APP_TRUE;
  }


   ioMode = BOARD_FLASH_QSPI_IO_MODE_QUAD;
   Board_flashOffsetToBlkPage(flashHandle,SPI_EEPROM_DEVICECONF_OFFSET,
                          &blockNum, &pageNum);

   Board_flashEraseBlk(flashHandle, blockNum);


   if(0 != Board_flashWrite(flashHandle, SPI_EEPROM_DEVICECONF_OFFSET, (uint8_t*)lpstRemData,sizeof(ST_PND_REMANENT_DATA), (void *)(&ioMode)))
   {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
   }

  return OS_OK;
}

#ifdef PN_RTC3_SUPPORTED

/*


setProfile (Int32 portNumber, Int32 profileNumber, Int32 rxSoGValue, Int32 txSoGValue)
Port ass                   1                    1              10120              9073
Port ass                   2                    1                  0                 0

mapPhaseToProfile(Int32 portNumber, Int32 phaseNumber, Int32 profileNumberRx, Int32 profileNumberTx)

TxRx ass                          1                 1                     1                     1
TxRx ass                          2                 1                     1                     1*/


APP_WORD portAssignment(APP_BYTE byPortId, APP_DWORD dwNumberOfAssignment, APP_LPDWORD lpdwTxBeginOfGreen, APP_LPDWORD lpdwRxBeginOfGreen)
{
  APP_BYTE byProfile = 1;

  byPortId = byPortId +1;

  DUAUSER_TRACE("Port ass %d %d %d %d \n\r", byPortId, dwNumberOfAssignment, lpdwTxBeginOfGreen[0], lpdwRxBeginOfGreen[0]);

  for(byProfile=0; byProfile < dwNumberOfAssignment; byProfile++)
  {
    if(0 > setProfile(byPortId, byProfile+1, lpdwRxBeginOfGreen[byProfile], lpdwTxBeginOfGreen[byProfile]))
    {
      OS_ASSERT(APP_FALSE);
      return OS_NOT_OK;
    }
  }
  return OS_OK;
}

APP_WORD phaseTxRxAssignment(APP_BYTE byPortId, APP_DWORD dwNumberOfPhase, APP_BYTE byTxPhaseAssignment, APP_BYTE byRxPhaseAssignment)
{
  byPortId = byPortId +1;

  DUAUSER_TRACE("TxRx ass %d %d %d %d \n\r", byPortId, dwNumberOfPhase, byRxPhaseAssignment, byTxPhaseAssignment);

  if(0 > mapPhaseToProfile(byPortId, dwNumberOfPhase, byRxPhaseAssignment, byTxPhaseAssignment))
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  return OS_OK;
}

APP_WORD UserRTC3PortAssigment(APP_DWORD dwNumberofPorts, LPST_PDIRBEGINENDDATA lpstPDIrBeginData)
{
  APP_BYTE  byIndex = 0;
  APP_WORD wPhaseNumber = 0;
  APP_DWORD dwTxBG[WSE_LLDP_MAX_PORT][MAX_NUMBER_ASSIGNMENT];
  APP_DWORD dwRxBG[WSE_LLDP_MAX_PORT][MAX_NUMBER_ASSIGNMENT];

  OS_MEMSET(&dwTxBG[0][0], 0, WSE_LLDP_MAX_PORT*MAX_NUMBER_ASSIGNMENT*sizeof(APP_DWORD));
  OS_MEMSET(&dwRxBG[0][0], 0, WSE_LLDP_MAX_PORT*MAX_NUMBER_ASSIGNMENT*sizeof(APP_DWORD));

  for(byIndex=0; byIndex < dwNumberofPorts; byIndex++)
  {
    for(wPhaseNumber = 0; wPhaseNumber < lpstPDIrBeginData->dwNumberOfAssignment[byIndex]; wPhaseNumber++)
    {
      /* Foreach defined phase => start of red, start of orange, start of Green*/
      dwTxBG[byIndex][wPhaseNumber] = lpstPDIrBeginData->st_Assignment[byIndex].dwGreenBegin[wPhaseNumber*2] ;
      dwRxBG[byIndex][wPhaseNumber] = lpstPDIrBeginData->st_Assignment[byIndex].dwGreenBegin[(wPhaseNumber*2)+1] ;
    }

    if(OS_OK != portAssignment(byIndex, lpstPDIrBeginData->dwNumberOfAssignment[byIndex], &dwTxBG[byIndex][0], &dwRxBG[byIndex][0]))
    {
      return OS_NOT_OK;
    }
  }

  return OS_OK;
}

APP_WORD UserRTC3PhaseAssigment(APP_DWORD dwNumberofPorts, LPST_PDIRBEGINENDDATA lpstPDIrBeginData)
{
  APP_BYTE byIndex = 0;
  APP_BYTE byPhaseIndex = 0;
  APP_WORD wPhaseNumber = 0;
  APP_WORD wPhaseMaxNumber = 0;
  APP_WORD wValidPhaseNumber =0;

  APP_BYTE byTxAssignment[WSE_LLDP_MAX_PORT][MAX_PHASE_ASSIGNMENT];
  APP_BYTE byRxAssignment[WSE_LLDP_MAX_PORT][MAX_PHASE_ASSIGNMENT];

  OS_MEMSET(&byTxAssignment[0][0], 0, WSE_LLDP_MAX_PORT*MAX_PHASE_ASSIGNMENT*sizeof(APP_BYTE));
  OS_MEMSET(&byRxAssignment[0][0], 0, WSE_LLDP_MAX_PORT*MAX_PHASE_ASSIGNMENT*sizeof(APP_BYTE));

  //Experiment in assigning Profiles to Phases

  wPhaseMaxNumber = lpstPDIrBeginData->st_Assignment[byIndex].dwNumberOfPhases;
  for(byIndex=0; byIndex < dwNumberofPorts;byIndex++)
    {
		for(wPhaseNumber = 0; wPhaseNumber < (MAX_PHASE_ASSIGNMENT/wPhaseMaxNumber); wPhaseNumber++)
		{
			for(wValidPhaseNumber = 0; wValidPhaseNumber < wPhaseMaxNumber; wValidPhaseNumber++)
			{
					/* Foreach defined phase => start of red, start of orange, start of Green*/
				byTxAssignment[byIndex][wPhaseNumber*wPhaseMaxNumber + wValidPhaseNumber] = (lpstPDIrBeginData->st_Assignment[byIndex].wPhaseAssignmentTX[wValidPhaseNumber] & 0x0F) +1 ; /* profile from 1 to 5*/
				byRxAssignment[byIndex][wPhaseNumber*wPhaseMaxNumber + wValidPhaseNumber] = (lpstPDIrBeginData->st_Assignment[byIndex].wPhaseAssignmentRX[wValidPhaseNumber] & 0x0F) + 1; /* profile from 1 to 5*/
			}

		}
  }

  for(byPhaseIndex=0; byPhaseIndex < MAX_PHASE_ASSIGNMENT; byPhaseIndex++)
  {
    for(byIndex=0; byIndex < dwNumberofPorts;byIndex++)
    {
      if(OS_OK != phaseTxRxAssignment(byIndex, byPhaseIndex+1, byTxAssignment[byIndex][byPhaseIndex], byRxAssignment[byIndex][byPhaseIndex]))
      {
        return OS_NOT_OK;
      }
    }
  }

  return OS_OK;
}

APP_WORD UserRTC3SetFrameID(APP_WORD wStartOfFrameId, APP_WORD wEndOfFrameId)
{
  DUAUSER_TRACE("wStartOfFrameId 0x%04X wEndOfFrameId 0x%04X \n\r", wStartOfFrameId, wEndOfFrameId);

  if (0 > setRedGuard(wStartOfFrameId, wEndOfFrameId))
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  return OS_OK;
}

APP_WORD UserRTC3SetInterface( LPST_CLR_RPC_UUID lpIrdataUUID,
                                              APP_DWORD dwMaxBrideDelay,
                                              APP_DWORD dwNumberOfPort,
                                              APP_DWORD dwMaxPortTxDelay1,
                                              APP_DWORD dwMaxPorRxDelay1,
                                              APP_DWORD dwMaxLineRxDelayport1,
                                              APP_DWORD dwYellowtimePort1 ,
                                              APP_DWORD dwMaxPortTxDelay2,
                                              APP_DWORD dwMaxPortRxDelay2,
                                              APP_DWORD dwMaxLineRxDelayport2,
                                              APP_DWORD dwYellowtimePort2)
{

  DUAUSER_TRACE("dwMaxBrideDelay 0x%08X (%d)\n\r", dwMaxBrideDelay, dwMaxBrideDelay);

  if( 0 > setMaxBridgeDelay(dwMaxBrideDelay))
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  DUAUSER_TRACE("dwMaxLineRxDelayport1 0x%08X (%d)\n\r", dwMaxLineRxDelayport1, dwMaxLineRxDelayport1);

  if( 0 > setMaxLineRxDelay(1, dwMaxLineRxDelayport1))
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  DUAUSER_TRACE("dwMaxLineRxDelayport2 0x%08X (%d)\n\r", dwMaxLineRxDelayport2, dwMaxLineRxDelayport2);

  if( 0 > setMaxLineRxDelay(2, dwMaxLineRxDelayport2))
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  DUAUSER_TRACE("dwYellowtimePort1 0x%08X (%d)\n\r", dwYellowtimePort1, dwYellowtimePort1);

  if ( 0 > setYellowPeriod(dwYellowtimePort1))
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }

  return OS_OK;
}



APP_WORD UserSetDcpBoundary(APP_BYTE byPort, LPST_PND_REMANENT_DATA lpstRemData)
{
  /* DCP_Identify filter */
  APP_BYTE byDcpBoundary[MAC_ADDRESS_LEN] = {0x01,0x0E,0xCF,0x00,0x00,0x00};
  int iBit=0;

  for(iBit=0;iBit<32;iBit++)
  {
    byDcpBoundary[5] = iBit;

    /* convert int to hex string */
    if( (1<<iBit) == (lpstRemData->stPortRemData[byPort].dwDCPBoundary & (1<<iBit)) )
    {
      setStaticFilterTable(byDcpBoundary, RCV_NO_FWD, byPort+1, FORWARDING);
    }
    else
    {
      /* Do not block */
      setStaticFilterTable(byDcpBoundary, RCV_FWD, byPort+1, FORWARDING);
    }
  }


  return OS_OK;
}

APP_WORD UserSetDomainBoundaryIngress(APP_BYTE byPort, LPST_PND_REMANENT_DATA lpstRemData)
{
  APP_BYTE byMac1[MAC_ADDRESS_LEN] = {0x01,0x0E,0xCF,0x00,0x04,0x20};
  APP_BYTE byMac2[MAC_ADDRESS_LEN] = {0x01,0x0E,0xCF,0x00,0x04,0x40};
  APP_BYTE byMac3[MAC_ADDRESS_LEN] = {0x01,0x0E,0xCF,0x00,0x04,0x80};
  /*char bufftoto[512]={0};

  UARTPutString(uartInstance, "UserSetDomainBoundaryIngress\n\r");
  OS_SPRINTF(bufftoto, "byPort:%d, lpstRemData->stPortRemData[byPort].dwAdjustDomainBoundaryIngress:%d \n\r", byPort, lpstRemData->stPortRemData[byPort].dwAdjustDomainBoundaryIngress);
  UARTPutString(uartInstance, bufftoto);
*/
  uint8_t cutthroughPort = 0x1 & (~byPort);
  if((DWORD_BITFIELD_BIT0 & lpstRemData->stPortRemData[byPort].dwAdjustDomainBoundaryIngress) == DWORD_BITFIELD_BIT0)
  {
    /*Block an incoming frame with the multicast MAC address
    01-0E-CF-00-04-20, 01-0E-CF-00-04-40 and 01-0E-CF-00-04-80*/
    /*UARTPutString(uartInstance, "NO_RCV_NO_FWD\n\r");*/
    if((DWORD_BITFIELD_BIT0 & lpstRemData->stPortRemData[cutthroughPort].dwAdjustDomainBoundaryEgress) == DWORD_BITFIELD_BIT0)
    {
        setStaticFilterTable(byMac1, NO_RCV_NO_FWD, byPort+1, FORWARDING);
        setStaticFilterTable(byMac2, NO_RCV_NO_FWD, byPort+1, FORWARDING);
        setStaticFilterTable(byMac3, NO_RCV_NO_FWD, byPort+1, FORWARDING);
    }
    else
    {
        setStaticFilterTable(byMac1, NO_RCV_FWD, byPort+1, FORWARDING);
        setStaticFilterTable(byMac2, NO_RCV_FWD, byPort+1, FORWARDING);
        setStaticFilterTable(byMac3, NO_RCV_FWD, byPort+1, FORWARDING);
    }
  }
  else
  {
    /*UARTPutString(uartInstance, "RCV_FWD\n\r");*/
    if((DWORD_BITFIELD_BIT0 & lpstRemData->stPortRemData[cutthroughPort].dwAdjustDomainBoundaryEgress) == DWORD_BITFIELD_BIT0)
    {
        setStaticFilterTable(byMac1, RCV_NO_FWD, byPort+1, FORWARDING);
        setStaticFilterTable(byMac2, RCV_NO_FWD, byPort+1, FORWARDING);
        setStaticFilterTable(byMac3, RCV_NO_FWD, byPort+1, FORWARDING);
    }
    else
    {
        setStaticFilterTable(byMac1, RCV_FWD, byPort+1, FORWARDING);
        setStaticFilterTable(byMac2, RCV_FWD, byPort+1, FORWARDING);
        setStaticFilterTable(byMac3, RCV_FWD, byPort+1, FORWARDING);
    }
  }
  return OS_OK;
}

APP_WORD UserSetDomainBoundaryEgress(APP_BYTE byPort, LPST_PND_REMANENT_DATA lpstRemData)
{
  APP_BYTE byMac1[MAC_ADDRESS_LEN] = {0x01,0x0E,0xCF,0x00,0x04,0x20};
  APP_BYTE byMac2[MAC_ADDRESS_LEN] = {0x01,0x0E,0xCF,0x00,0x04,0x40};
  APP_BYTE byMac3[MAC_ADDRESS_LEN] = {0x01,0x0E,0xCF,0x00,0x04,0x80};

  /*char bufftoto[512]={0};

  UARTPutString(uartInstance, "UserSetDomainBoundaryEgress\n\r");
  OS_SPRINTF(bufftoto, "byPort:%d, lpstRemData->stPortRemData[byPort].dwAdjustDomainBoundaryEgress:%d \n\r", byPort, lpstRemData->stPortRemData[byPort].dwAdjustDomainBoundaryEgress);
  UARTPutString(uartInstance, bufftoto);*/
  uint8_t cutthroughPort = 0x1 & (~byPort);

  if((DWORD_BITFIELD_BIT0 & lpstRemData->stPortRemData[byPort].dwAdjustDomainBoundaryEgress) == DWORD_BITFIELD_BIT0)
  {
      /*UARTPutString(uartInstance, "RCV_NO_FWD\n\r");*/
      /*Block an outgoing frame with the multicast MAC address
       * 01-0E-CF-00-04-20, 01-0E-CF-00-04-40 and 01-0E-CF-00-04-80*/
    if((DWORD_BITFIELD_BIT0 & lpstRemData->stPortRemData[cutthroughPort].dwAdjustDomainBoundaryIngress) == DWORD_BITFIELD_BIT0)
    {
        setStaticFilterTable(byMac1, NO_RCV_NO_FWD, cutthroughPort+1, FORWARDING);
        setStaticFilterTable(byMac2, NO_RCV_NO_FWD, cutthroughPort+1, FORWARDING);
        setStaticFilterTable(byMac3, NO_RCV_NO_FWD, cutthroughPort+1, FORWARDING);
    }
    else
    {
        setStaticFilterTable(byMac1, RCV_NO_FWD, cutthroughPort+1, FORWARDING);
        setStaticFilterTable(byMac2, RCV_NO_FWD, cutthroughPort+1, FORWARDING);
        setStaticFilterTable(byMac3, RCV_NO_FWD, cutthroughPort+1, FORWARDING);
    }
  }
  else
  {
    /*UARTPutString(uartInstance, "RCV_FWD\n\r");*/
      if((DWORD_BITFIELD_BIT0 & lpstRemData->stPortRemData[cutthroughPort].dwAdjustDomainBoundaryIngress) == DWORD_BITFIELD_BIT0)
      {
        setStaticFilterTable(byMac1, NO_RCV_FWD, cutthroughPort+1, FORWARDING);
        setStaticFilterTable(byMac2, NO_RCV_FWD, cutthroughPort+1, FORWARDING);
        setStaticFilterTable(byMac3, NO_RCV_FWD, cutthroughPort+1, FORWARDING);
      }
      else
      {
        setStaticFilterTable(byMac1, RCV_FWD, cutthroughPort+1, FORWARDING);
        setStaticFilterTable(byMac2, RCV_FWD, cutthroughPort+1, FORWARDING);
        setStaticFilterTable(byMac3, RCV_FWD, cutthroughPort+1, FORWARDING);
      }
  }
  return OS_OK;
}

APP_WORD UserSetDomainBoundaryAnnounce(APP_BYTE byPort, LPST_PND_REMANENT_DATA lpstRemData)
{
  APP_BYTE byPtcpAnnounce[MAC_ADDRESS_LEN] = {0x01,0x0E,0xCF,0x00,0x04,0x00};

  /*char bufftoto[512]={0};
  UARTPutString(uartInstance, "UserSetDomainBoundaryAnnounce\n\r");*/

  if( ((DWORD_BITFIELD_BIT0 & lpstRemData->stPortRemData[byPort].dwAdjustDomainBoundaryEgress) == DWORD_BITFIELD_BIT0) &&
    ( (DWORD_BITFIELD_BIT0 & lpstRemData->stPortRemData[byPort].dwAdjustDomainBoundaryIngress) == DWORD_BITFIELD_BIT0))
  {
    /*UARTPutString(uartInstance, "NO_RCV_NO_FWD\n\r");*/
    setStaticFilterTable(byPtcpAnnounce, NO_RCV_NO_FWD, byPort+1, FORWARDING);
  }
  else
  {
    /*UARTPutString(uartInstance, "RCV_FWD\n\r");*/
    setStaticFilterTable(byPtcpAnnounce, RCV_FWD, byPort+1, FORWARDING);
  }
  return OS_OK;
}

APP_WORD UserSetMulticastBoundary(APP_BYTE byPort, LPST_PND_REMANENT_DATA lpstRemData)
{
  APP_BYTE byRtc2MultiCast[MAC_ADDRESS_LEN] = {0x01,0x0E,0xCF,0x00,0x02,0x00};

  int iBit=0;

  for(iBit=0;iBit<32;iBit++)
  {
    /*OS_SPRINTF((char *)szMac, "010ECF0002%02X", iBit);*/
    byRtc2MultiCast[5] = iBit;

    /* convert int to hex string */
    if( (1<<iBit) == (lpstRemData->stPortRemData[byPort].dwAdjustMulticastBoundary & (1<<iBit)) )
    {
      setStaticFilterTable(byRtc2MultiCast, NO_RCV_NO_FWD, byPort+1, FORWARDING);
    }
    else
    {
      /* Do not block */
      setStaticFilterTable(byRtc2MultiCast, RCV_FWD, byPort+1, FORWARDING);
    }
  }

  return OS_OK;
}

APP_WORD UserSetBoundaries(LPST_PND_REMANENT_DATA lpstRemData)
{
  APP_BYTE byPort  = 0;

  for (byPort=0; byPort<WSE_LLDP_MAX_PORT;byPort++)
  {
    UserSetDcpBoundary(byPort,lpstRemData);
    UserSetDomainBoundaryEgress(byPort,lpstRemData);
    UserSetDomainBoundaryIngress(byPort,lpstRemData);
    UserSetDomainBoundaryAnnounce(byPort,lpstRemData);
    UserSetMulticastBoundary(byPort, lpstRemData);
  }

  return OS_OK;
}


APP_WORD UserSetPTCP(LPST_PND_REMANENT_DATA lpstRemData)
{
  APP_BYTE bySyncID = 0;
  APP_WORD wStatus = 0;

  APP_BYTE byEmptyMac[MAC_ADDRESS_LEN] = {0};

  if(OS_OK != UserRTC3SetInterface(&lpstRemData->stAdjustIrData.stPDIrGlobaldata.IRDataUUID,
        lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwMaxBridgeDelay,
        lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwNumberofPorts,
        lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwMaxTxPort[0],
        lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwMaxRxPort[0],
        lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwMaxLineRxDelay[0],
        lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwYellowTime[0],
        lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwMaxTxPort[1],
        lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwMaxRxPort[1],
        lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwMaxLineRxDelay[1],
        lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwYellowTime[1]))
  {
    return OS_NOT_OK;
  }

  if(OS_OK != UserRTC3SetFrameID(lpstRemData->stAdjustIrData.stPDIrBeginData.wStartOfRedFrameID, lpstRemData->stAdjustIrData.stPDIrBeginData.wEndOfRedFrameID))
  {
   return OS_NOT_OK;
  }

  if(OS_OK != UserRTC3PortAssigment(lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwNumberofPorts, &lpstRemData->stAdjustIrData.stPDIrBeginData))
  {
   return OS_NOT_OK;
  }

  if(OS_OK != UserRTC3PhaseAssigment(lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwNumberofPorts, &lpstRemData->stAdjustIrData.stPDIrBeginData))
  {
   return OS_NOT_OK;
  }

  if(OS_OK !=  UpdateLLDP_PTCPStatusTLV(byEmptyMac, &(lpstRemData->stAdjustSyncData), &(lpstRemData->stAdjustIrData.stPDIrGlobaldata), &(lpstRemData->stAdjustIrData.stPDIrBeginData)))
  {
   return OS_NOT_OK;
  }

  bySyncID = (APP_BYTE)((lpstRemData->stAdjustSyncData.wSyncProperties>>8) & 0x1F);

  if(OS_OK !=  PTCP_TI_SyncStart(bySyncID, &(lpstRemData->stAdjustSyncData.PTCPSubDomainUUID), lpstRemData->stAdjustSyncData.dwSyncSendFactor, lpstRemData->stAdjustSyncData.wSendClockFactor, lpstRemData->stAdjustSyncData.wPTCPTimeOutFactor, lpstRemData->stAdjustSyncData.wPTCPTakeOverTimeOutFactor, lpstRemData->stAdjustSyncData.dwPLLWindow, &wStatus) )
  {
   return OS_NOT_OK;
  }
  return OS_OK;
}

APP_WORD UserUnSetPTCP(LPST_PND_REMANENT_DATA lpstRemData)
{
  APP_WORD wStatus = 0;

  if(OS_OK !=  PTCP_TI_SyncStop(0, &wStatus))
  {
    return OS_NOT_OK;
  }

  return OS_OK;
}

APP_WORD UserSysEthGetCableDelay(APP_WORD wPort, APP_LPDWORD lpdwCableDelay)
{
  wPort--;

  *lpdwCableDelay = gdwCableDelayRemote[wPort];

  return OS_OK;
}

#endif




APP_WORD UserApplyConf(LPST_PND_REMANENT_DATA lpstOldRemData, LPST_PND_REMANENT_DATA lpstRemData)
{
  APP_WORD wIndex  = 0;
#ifdef PN_RTC3_SUPPORTED
  APP_BYTE bySyncID = 0;
  APP_WORD wStatus  = 0;
#endif


  if(APP_NULL == lpstRemData)
    return OS_NOT_OK;

  if(APP_NULL == lpstOldRemData)
  {
    for (wIndex=0; wIndex<WSE_LLDP_MAX_PORT;wIndex++)
    {
      if(APP_TRUE == lpstRemData->stPortRemData[wIndex].byIsAdjustMAUTypeActivate)
      {
        if(OS_OK != UserSysEthSetPortMautype(wIndex+1, lpstRemData->stPortRemData[wIndex].wAdjustMAUType, 0))
        {
          return OS_NOT_OK;
        }
      }
      else
      {
        /* No Adjust : autoneg*/
        if(OS_OK != UserSysEthSetPortMautype(wIndex+1, 0, 1))
        {
          return OS_NOT_OK;
        }
      }
    }

    setDcpFilterStationName((const uint8_t*)lpstRemData->sName, lpstRemData->wNameLen);

#ifdef PN_RTC3_SUPPORTED

    if((APP_TRUE == lpstRemData->byIsAdjustIrDataEnable) && (APP_TRUE == lpstRemData->byIsAdjustSyncDataEnable))
    {
    	/*Apply completed new conf*/
    	if(OS_OK != UserSetPTCP(lpstRemData))
    	{
    		return OS_NOT_OK;
    	}
    }
    else
    {
      if(OS_OK != UserUnSetPTCP(lpstRemData))
      {
        return OS_NOT_OK;
      }
    }

#endif

  }
  else
  {
    /* Connect phase */
    if(0 != OS_MEMCMP(lpstOldRemData, lpstRemData, sizeof(ST_PND_REMANENT_DATA)))
    {

      for (wIndex=0; wIndex<WSE_LLDP_MAX_PORT;wIndex++)
      {

        if((lpstOldRemData->stPortRemData[wIndex].byIsAdjustMAUTypeActivate != lpstRemData->stPortRemData[wIndex].byIsAdjustMAUTypeActivate)       ||
                          (lpstOldRemData->stPortRemData[wIndex].wAdjustMAUType != lpstRemData->stPortRemData[wIndex].wAdjustMAUType))
        {
          if(APP_TRUE == lpstRemData->stPortRemData[wIndex].byIsAdjustMAUTypeActivate)
          {
            if(OS_OK != UserSysEthSetPortMautype(wIndex+1, lpstRemData->stPortRemData[wIndex].wAdjustMAUType, 0))
            {
              return OS_NOT_OK;
            }
          }
          else
          {
            /* No Adjust : autoneg*/
            if(OS_OK != UserSysEthSetPortMautype(wIndex+1, 0, 1))
            {
              return OS_NOT_OK;
            }
          }
        }

#ifdef PN_RTC3_SUPPORTED
        if((lpstOldRemData->stPortRemData[wIndex].byIsAdjustDCPBoundaryActivate != lpstRemData->stPortRemData[wIndex].byIsAdjustDCPBoundaryActivate)       ||
                  (lpstOldRemData->stPortRemData[wIndex].dwDCPBoundary != lpstRemData->stPortRemData[wIndex].dwDCPBoundary))
        {
          UserSetDcpBoundary(wIndex, lpstRemData);
        }

        if((lpstOldRemData->stPortRemData[wIndex].byIsAdjustDomainBoundaryActivate != lpstRemData->stPortRemData[wIndex].byIsAdjustDomainBoundaryActivate)       ||
           (lpstOldRemData->stPortRemData[wIndex].dwAdjustDomainBoundaryIngress != lpstRemData->stPortRemData[wIndex].dwAdjustDomainBoundaryIngress))

        {
          UserSetDomainBoundaryIngress(wIndex, lpstRemData);
        }

        if((lpstOldRemData->stPortRemData[wIndex].byIsAdjustDomainBoundaryActivate != lpstRemData->stPortRemData[wIndex].byIsAdjustDomainBoundaryActivate)       ||
           (lpstOldRemData->stPortRemData[wIndex].dwAdjustDomainBoundaryEgress != lpstRemData->stPortRemData[wIndex].dwAdjustDomainBoundaryEgress))

        {
          UserSetDomainBoundaryEgress(wIndex, lpstRemData);
        }

        if((lpstOldRemData->stPortRemData[wIndex].byIsAdjustDomainBoundaryActivate != lpstRemData->stPortRemData[wIndex].byIsAdjustDomainBoundaryActivate)       ||
           (lpstOldRemData->stPortRemData[wIndex].dwAdjustDomainBoundaryEgress != lpstRemData->stPortRemData[wIndex].dwAdjustDomainBoundaryEgress)               ||
           (lpstOldRemData->stPortRemData[wIndex].dwAdjustDomainBoundaryIngress != lpstRemData->stPortRemData[wIndex].dwAdjustDomainBoundaryIngress))

        {
          UserSetDomainBoundaryAnnounce(wIndex, lpstRemData);
        }


        if((lpstOldRemData->stPortRemData[wIndex].byIsAdjustMulticastBoundaryActivate != lpstRemData->stPortRemData[wIndex].byIsAdjustMulticastBoundaryActivate)       ||
           (lpstOldRemData->stPortRemData[wIndex].dwAdjustMulticastBoundary != lpstRemData->stPortRemData[wIndex].dwAdjustMulticastBoundary))

        {
          UserSetMulticastBoundary(wIndex, lpstRemData);
        }
#endif
      }


#ifdef PN_RTC3_SUPPORTED
      if((lpstOldRemData->byIsAdjustIrDataEnable != lpstRemData->byIsAdjustIrDataEnable)       ||
         (0 != OS_MEMCMP(&lpstOldRemData->stAdjustIrData, &lpstRemData->stAdjustIrData, sizeof(ST_PDIRDATA))))
      {
        DUAUSER_TRACE("\nConfiguration Ir change PDIrGlobaldata\n\r");

        if((0 != OS_MEMCMP(&lpstOldRemData->stAdjustIrData.stPDIrGlobaldata, &lpstRemData->stAdjustIrData.stPDIrGlobaldata, sizeof(ST_PDIRGLOBALDATA))))
        {
          UserRTC3SetInterface(&lpstRemData->stAdjustIrData.stPDIrGlobaldata.IRDataUUID,
                     lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwMaxBridgeDelay,
                     lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwNumberofPorts,
                     lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwMaxTxPort[0],
                     lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwMaxRxPort[0],
                     lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwMaxLineRxDelay[0],
                     lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwYellowTime[0],
                     lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwMaxTxPort[1],
                     lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwMaxRxPort[1],
                     lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwMaxLineRxDelay[1],
                     lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwYellowTime[1]);
        }

        if( (lpstOldRemData->stAdjustIrData.stPDIrBeginData.wStartOfRedFrameID != lpstRemData->stAdjustIrData.stPDIrBeginData.wStartOfRedFrameID) ||
            (lpstOldRemData->stAdjustIrData.stPDIrBeginData.wEndOfRedFrameID   != lpstRemData->stAdjustIrData.stPDIrBeginData.wEndOfRedFrameID) )
        {
          UserRTC3SetFrameID(lpstRemData->stAdjustIrData.stPDIrBeginData.wStartOfRedFrameID, lpstRemData->stAdjustIrData.stPDIrBeginData.wEndOfRedFrameID);
        }

        if( (lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwNumberofPorts != lpstOldRemData->stAdjustIrData.stPDIrGlobaldata.dwNumberofPorts) ||
            (0 != OS_MEMCMP(&lpstOldRemData->stAdjustIrData.stPDIrBeginData, &lpstRemData->stAdjustIrData.stPDIrBeginData, sizeof(ST_PDIRBEGINENDDATA))))
        {

          UserRTC3PortAssigment (lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwNumberofPorts, &lpstRemData->stAdjustIrData.stPDIrBeginData);
          UserRTC3PhaseAssigment(lpstRemData->stAdjustIrData.stPDIrGlobaldata.dwNumberofPorts, &lpstRemData->stAdjustIrData.stPDIrBeginData);
        }

        if( OS_NOT_OK == UpdateLLDP_PTCPStatusTLV(gbySyncMasterMac, &lpstRemData->stAdjustSyncData, &lpstRemData->stAdjustIrData.stPDIrGlobaldata, &lpstRemData->stAdjustIrData.stPDIrBeginData))
        {
          OS_ASSERT(APP_FALSE);
          return OS_NOT_OK;
        }
      }


      if((lpstOldRemData->byIsAdjustSyncDataEnable       != lpstRemData->byIsAdjustSyncDataEnable)       ||
        (0 != OS_MEMCMP(&lpstOldRemData->stAdjustSyncData, &lpstRemData->stAdjustSyncData, sizeof(ST_PDSYNCDATA))))
      {
        DUAUSER_TRACE("\n Configuration Sync change \n\r");

        if(APP_TRUE == lpstRemData->byIsAdjustSyncDataEnable)
        {
          if( OS_NOT_OK == UpdateLLDP_PTCPStatusTLV(gbySyncMasterMac, &lpstRemData->stAdjustSyncData, &lpstRemData->stAdjustIrData.stPDIrGlobaldata, &lpstRemData->stAdjustIrData.stPDIrBeginData))
          {
            OS_ASSERT(APP_FALSE);
            return OS_NOT_OK;
          }
          /* Re init ptcp engine*/
          bySyncID = (APP_BYTE)((lpstRemData->stAdjustSyncData.wSyncProperties>>8) & 0x1F);

          if(OS_NOT_OK == PTCP_TI_SyncStart(bySyncID, &(lpstRemData->stAdjustSyncData.PTCPSubDomainUUID), lpstRemData->stAdjustSyncData.dwSyncSendFactor, lpstRemData->stAdjustSyncData.wSendClockFactor, lpstRemData->stAdjustSyncData.wPTCPTimeOutFactor, lpstRemData->stAdjustSyncData.wPTCPTakeOverTimeOutFactor, lpstRemData->stAdjustSyncData.dwPLLWindow, &wStatus) )
          {
            OS_ASSERT(APP_FALSE);
            return OS_NOT_OK;
          }
        }
        else
        {
          /*Stop ptcp engine*/
          bySyncID = (APP_BYTE)((lpstOldRemData->stAdjustSyncData.wSyncProperties>>8) & 0x1F);

          if( OS_OK != PTCP_TI_SyncStop(bySyncID, &wStatus))
          {
            OS_ASSERT(APP_FALSE);
            return OS_NOT_OK;
          }
        }

      }
#endif

      OS_MEMCOPY(lpstOldRemData, lpstRemData, sizeof(ST_PND_REMANENT_DATA));
    }
  }
  return OS_OK;
}





APP_DWORD UserConvertMsTotick(APP_DWORD dwPauseTime_ms)
{
  unsigned int ticks = 0;

  if(dwPauseTime_ms == 0)
  {
    ticks = 0;
  }
  else
  {
    ticks = dwPauseTime_ms / ((double)ClockP_getTickPeriod()/1000) ;
    if(ticks < 1) ticks = 1;
  }

  return ticks;


}

APP_DWORD UserInit(APP_VOID)
{
	MailboxP_Params   mbxParams;

	//MailboxP_Params_init(&mbxParams);

  gmbxUser = MailboxP_create(sizeof(USER_MAILBOX_TYPE), MAX_USER_MSG, &mbxParams);

  if(gmbxUser == NULL)
  {
    DUAUSER_TRACE("ERROR can't impossible to create mailbox.\n");
    return (OS_NOT_OK);
  }
#ifdef PN_RTC3_SUPPORTED
  if(OS_OK != PTCP_TI_SyncInit())
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }
#endif

  return OS_OK;
}


APP_DWORD UserExit(APP_VOID)
{
	MailboxP_delete(&gmbxUser);

#ifdef PN_RTC3_SUPPORTED
  if(OS_OK != PTCP_TI_SyncExit())
  {
    OS_ASSERT(APP_FALSE);
    return OS_NOT_OK;
  }
#endif

  return OS_OK;
}


APP_WORD UserSysEthGetStatistics(APP_WORD wPort, APP_LPDWORD lpdwIfInOctets,  APP_LPDWORD lpdwifInDiscards, APP_LPDWORD lpdwifInErrors, 
                                            APP_LPDWORD lpdwifOutOctets, APP_LPDWORD lpdwifOutDiscards, APP_LPDWORD lpdwifOutErrors)    
{
  *lpdwIfInOctets = getIfInOctets(wPort);
  *lpdwifOutOctets = getIfOutOctets(wPort);

  *lpdwifInDiscards = getIfInDiscards(wPort);
  *lpdwifOutDiscards  = getIfOutDiscards(wPort);

  *lpdwifInErrors = getIfInErrors(wPort);
  *lpdwifOutErrors = getIfOutErrors(wPort);
  return OS_OK;
}


APP_WORD UserSysEthGetLinkState(APP_WORD wPort, APP_LPWORD  lpwLinkState)
{
  wPort--;

  *lpwLinkState = gbyLinkState[wPort];

  return OS_OK;
}

APP_WORD UserSysEthSetPortMautype(APP_WORD wPort, APP_WORD wMAUType, APP_BYTE byAutoNeg)
{
  wPort--;

  if(1==byAutoNeg)
  {
    PN_SetPhyConfig(PHY_CONFIG_AUTONEG, wPort);
  }
  else
  {
    switch(wMAUType)
    {
      case USER_SYS_ETH_MAUTYPE_100BASETXFD:
        PN_SetPhyConfig(PHY_CONFIG_100FD, wPort);
        break;

      case USER_SYS_ETH_MAUTYPE_100BASETXHD:
        PN_SetPhyConfig(PHY_CONFIG_100HD, wPort);
        break;

      default:
        PN_SetPhyConfig(PHY_CONFIG_AUTONEG, wPort);
        break;
    }
  }

  return OS_OK;
}


APP_WORD UserSysEthGetPortMautype(APP_WORD wPort, APP_LPWORD lpwMAUType)
{
  APP_BYTE byPhyMode = PHY_CONFIG_AUTONEG;

  wPort--;

  if(PN_GetLinkStatus(wPort))
  {

      byPhyMode = PN_GetPhyConfig(wPort);

      switch(byPhyMode)
      {
        case PHY_CONFIG_AUTONEG:
          *lpwMAUType = USER_SYS_ETH_MAUTYPE_100BASETXFD;
          break;

        case PHY_CONFIG_100FD:
          *lpwMAUType = USER_SYS_ETH_MAUTYPE_100BASETXFD;
          break;

        case PHY_CONFIG_10FD:
          *lpwMAUType = USER_SYS_ETH_MAUTYPE_10BASETXFD;
          break;

        default:
          *lpwMAUType = USER_SYS_ETH_MAUTYPE_100BASETXFD;
          break;
      }
  }
  else
      *lpwMAUType = USER_SYS_ETH_MAUTYPE_NONE;

  return OS_OK;
}

APP_DWORD UserManageEvent(APP_DWORD dwTimeout)
{
  APP_BYTE byPort = 0;
  APP_BYTE byLinkState = 0;
  APP_WORD wMAUType = 0;

  APP_WORD wStatus = 0;
  APP_DWORD  dwTicks = 0;
  USER_MAILBOX_TYPE UserMailBox;
#ifdef MRP_SUPPORTED
  MRPLinkState link_status = MRP_LNK_DOWN;
#endif

  dwTicks = UserConvertMsTotick(dwTimeout);

  if(MailboxP_pend(gmbxUser, &UserMailBox, dwTicks) == MailboxP_OK)
  {

    if(UserMailBox.wCodeFnct == USER_LINKINFO)
    {
      byPort = UserMailBox.byMsg[0];
      byLinkState = UserMailBox.byMsg[1];

      UserSysEthGetPortMautype(byPort+1, &wMAUType);

      if(OS_OK != PND_InformLinkOrMAUTypeChange(glpPhysicalDevice, 0, 0, 0x8000 + byPort + 1, wMAUType, byLinkState, 0, APP_NULL, &wStatus))
      {
        OS_ASSERT(FALSE);
      }

#ifdef FSU_SUPPORTED
      if(LINK_PORT_STATE_UP == byLinkState)
      {
        if(PND_DCP_Hello_Req(glpPhysicalDevice, byPort, &wStatus))
        {
          OS_ASSERT(FALSE);
        }
      }
#endif
#ifdef MRP_SUPPORTED
      if(LINK_PORT_STATE_UP == byLinkState)
      {
        link_status = MRP_LNK_UP;
      }

      MRP_PortItf_T_signalLocalLinkChange(byPort+1, link_status);
#endif
    }
    return OS_OK;
  }
  return OS_OK;
}


APP_VOID UserResetRemanentConf()
{
    ST_PND_REMANENT_DATA tempRemData;
    UserSetDefaultRemanentConf(&tempRemData);
}
