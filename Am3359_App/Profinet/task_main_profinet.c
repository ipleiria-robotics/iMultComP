/**
* @file main.c
*
* @brief Basic example for a PROFINET slave device running on AM335x/AM437x/AM57xx devices.
* Requires a full ICSS system (both PRUs) and two external phys connected to
* ICSS MII/MDIO interfaces
*
* \par
* Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
* \par
*
*/

/*! \mainpage PROFINET Slave Device Application
 * \version 1.0.1
 *
 * \section intro_sec Introduction
 *
 * This is an example PROFINET slave device application based on Profinet Drivers provided in PRU-ICSS
 * Profinet slave Package and Molex PROFINET stack. This example may be used for evaluation and test of
 * the TI PROFINET Slave device solution. It may also serve as a starting project for
 * customer applications.
 *
 * Build any configuration of the examples and use resulting binaries on
 * supported boards. See the GSD folder for required GSD file. Testing requires a PROFINET capable PLC.
 * How to use a PLC is not part of this documentation.
 *
 */

#include "app.h"

uint8_t info_mailbox;

SemaphoreP_Handle switchReady = NULL;

/** @brief rotary switch value on ICEv2 */
Uint8 nodeAddr;         /**< rotary switch value on ICEv2 */

/** \brief PRU-ICSS LLD handle */
PRUICSS_Handle prusshandle;
/** \brief ICSSEmac LLD handle */
ICSS_EmacHandle appEmacHandle;
/** \brief Profinet handle */
PN_Handle appPnHandle;
APP_BOOL gBlinkActive = APP_FALSE;



extern PRUICSS_Config pruss_config[2 + 1];
uint8_t lclMac[6];
extern ICSS_EmacBaseAddrCfgParams icss_EmacBaseAddrCfgParams[3];


/*****************************************************************************************************************
 *                                          MOLEX Stack references                                               *
 *************************************************************************************************************** */

/** @brief 'main' function of Molex stack (extern) */
extern void main_pn(void);
/** @brief Function to reset the Remanent data in flash with default values */
extern APP_VOID   UserResetRemanentConf();
/** @brief SNMP stack initialization (mib_impl.c) */
extern void initSNMP(void);
/** @brief stack input data buffer */
extern APP_BYTE  gbyInput_Data[MAX_DATA_FRAME_IO];
/** @brief stack output data buffer */
extern APP_BYTE  gbyOutputData[MAX_DATA_FRAME_IO];
/** @brief stack AR status info */
extern APP_BYTE  gbyOutputAPDU[MAX_ARD_PER_DEVICE];
/** @brief stack current data */
extern APP_BYTE  gbyCurrentData;
/**@brief stack user ALARM flag */
extern volatile unsigned char appAlarmActive;

extern int32_t EmacInit(STKEVENT_Handle hEvent);
/* Toggles the GPIO for HVS read */
extern void LoadData();
/** @brief stack debug buffer */


/******************************************************************************************************************
 *                              AUX Function                                                                *
 **************************************************************************************************************** */

uint8_t ICSSEmacDRVInit(ICSS_EmacHandle handle, uint8_t instance); // ICSS EMAC LLD handle init   --- return E_FAIL on failure, S_PASS on success

void PN_initDefaultIRTHandle(PN_Handle pnHandle, ICSS_EmacHandle emacHandle);

/**
 * @brief Function which writes Firmware version and features into DRAM0/1
 *        This is to be done before NIMU driver initialization.
 *
 * @param null
 */
void PN_writeFirmwareFeature();

/**
 * @brief Function to Get Mac id
 *
 * @param null
 */
void getMacid(char *buf_str);


void taskLedBlink(UArg arg0, UArg arg1);
void taskPruss(UArg a0, UArg a1);

void appOutData(UArg arg0, UArg arg1);

/******************************************************************************************************************
 *                              Init peripheral                                                                  *
 **************************************************************************************************************** */

ICSS_EmacFwStaticMmap *pIcssEmacStaticMMap;
ICSS_EmacFwDynamicMmap *pIcssEmacDynamicMMap;


void init_peripheral_profinet(){


    PN_socHwinit(PRUICSS_INSTANCE);

    prusshandle = PRUICSS_create(pruss_config, PRUICSS_INSTANCE);
    appEmacHandle = (ICSS_EmacHandle)malloc(sizeof(ICSS_EmacConfig));
    appPnHandle = (PN_Handle)malloc(sizeof(PN_Config));


    ICSSEMAC_InitConfig *switchEmacCfg;

    switchEmacCfg = (ICSSEMAC_InitConfig *)malloc(sizeof(ICSSEMAC_InitConfig));
    /* PRUICSS_INSTANCE == PRUICSSx_PHY_ADDRESS */
    switchEmacCfg->phyAddr[0] = Board_getPhyAddress(PRUICSS_INSTANCE, 1);
    switchEmacCfg->phyAddr[1] = Board_getPhyAddress(PRUICSS_INSTANCE, 2);

    switchEmacCfg->portMask = ICSS_EMAC_MODE_SWITCH;
    switchEmacCfg->ethPrioQueue = ICSS_EMAC_QUEUE3;
    switchEmacCfg->halfDuplexEnable = 0;
    switchEmacCfg->enableIntrPacing = ICSS_EMAC_ENABLE_PACING;
    switchEmacCfg->ICSS_EmacIntrPacingMode = ICSS_EMAC_INTR_PACING_MODE1;
    switchEmacCfg->pacingThreshold = 100;

    switchEmacCfg->learningEn = 1;

    PN_socgetMACAddress(INTERFACE_MAC,lclMac);


    switchEmacCfg->macId = lclMac;
    switchEmacCfg->rxIntNum = PN_RX_INT_NUM; /*20*/
    switchEmacCfg->linkIntNum = PN_LINK_INT_NUM; /*26*/

    ICSSEmacDRVInit(appEmacHandle, PRUICSS_BASEADDRESS_INSTANCE);


    ((ICSS_EmacObject *)appEmacHandle->object)->pruIcssHandle = prusshandle;
    ((ICSS_EmacObject *)appEmacHandle->object)->emacInitcfg = switchEmacCfg;



    icss_emacGetFwMMapInitConfig(1, &pIcssEmacStaticMMap, &pIcssEmacDynamicMMap);


}



/******************************************************************************************************************
 *                              TASK MAIN PROFINET                                                                 *
 **************************************************************************************************************** */
void task_main_profinet(){



    UART_printf("\n************** Profinet INIT ******************- \n");

    info_mailbox = 0;                      // Variable to delete task -> read to mailbox
    Bool bRunApplication = TRUE;

    TaskP_Params taskParams;
    SemaphoreP_Params semParams;

    PRUICSS_IntcInitData pruss_intc_initdata = PRUSS_INTC_INITDATA;

    // init profinet restant
    icss_emacSetFwMMapInitConfig(appEmacHandle, 1, pIcssEmacStaticMMap, pIcssEmacDynamicMMap);
    ICSS_EmacInit(appEmacHandle, &pruss_intc_initdata, ICSS_EMAC_MODE_SWITCH);

    /*Workaround:To write firmware feature set and version to DRAM. Required as ICSS EMAC APIs check for
     * the data. Need to move to firmware data section and load via firmware load API*/
    PN_writeFirmwareFeature();
    PN_initDefaultIRTHandle(appPnHandle, appEmacHandle);
    (appPnHandle->pnPtcpConfig)->ecapPwmssBaseAddr = PN_CSL_ECAP_REG;

    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    semParams.name = "switchReady";
    switchReady = SemaphoreP_create(0, &semParams);

//##########################################

    TaskP_Params_init(&taskParams);
    taskParams.stacksize = 0x600;
    taskParams.priority = 8;
    TaskP_create(taskLedBlink, &taskParams);
    UART_printf("\n**************init task led  ******************- \n");

    TaskP_Params_init(&taskParams);
    taskParams.priority = 9;        /* want stack prio higher than LED I/O*/
    taskParams.stacksize = 8192;
    taskParams.name = (uint8_t *)"SwitchTask";
    TaskP_create(taskPruss, &taskParams);
    UART_printf("\n**************init taskPruss  ******************- \n");


    TaskP_Params_init(&taskParams);
    taskParams.priority = 8;
    taskParams.name = (uint8_t *)"appOutData";
    taskParams.stacksize = 2048;
    TaskP_create(appOutData, &taskParams);
    UART_printf("\n************** appOutData  ******************- \n");

    /* all PN driver tasks moved to iPnOs.c*/
    if(PN_initOs(appPnHandle) < 0)
    {
        UART_printf("Can't initialize PN tasks\n");
        exit(-1);           /* fatal*/
    }

    TaskP_Params_init(&taskParams);
    taskParams.priority = 10;
    taskParams.name = (uint8_t *)"RxTask";
    taskParams.stacksize = 8192;
    taskParams.arg0 = (void *)appEmacHandle;

    ((ICSS_EmacObject *)appEmacHandle->object)->rxTaskHandle = TaskP_create((void *)ICSS_EMacOsRxTaskFnc, &taskParams);

    if(OSDRV_addNetifEntry(&EmacInit, appEmacHandle) == 0){
        UART_printf("Can't initialize OSDRV_addNetifEntry\n");
    }

    do{

         // verify mailbox to end Tasks
         Mailbox_pend(mailbox_Handle_profinet, &info_mailbox, BIOS_NO_WAIT);
         if(info_mailbox == 3){
                    bRunApplication = FALSE;
                    //TaskP_delete(&pdiTask);
                    //TaskP_delete(&ledTaskHndl);
                    //TaskP_delete(&sync0Task);
                    //TaskP_delete(&sync1Task);

                    UART_printf("    Terminate Ethercat protocol \n    ");
                    info_mailbox = 0;
          }


          // -------------------------------------------------------------------------------------------------------------------



     }while(bRunApplication == TRUE);






}

//--------------------------------------------------- TASK PROFINET  --------------------------------------------------------------------------

void appOutData(UArg arg0, UArg arg1){

    SemaphoreP_pend(switchReady, BIOS_WAIT_FOREVER);
    SemaphoreP_post(switchReady);

    // le a informaçao vinda do tia
    Uint8 prevState_1 = gbyInput_Data[1];
    Uint8 prevState_9 = gbyInput_Data[9];

    do
       {

           if(nodeAddr == 4)    /* local loopback mode*/
           {
               Board_setDigOutput(gbyCurrentData);
           }

           else                       /* communication data output*/
           {
               if(gbyInput_Data[1] != prevState_1 || gbyInput_Data[9] != prevState_9 )
               {
                   // informaçao do tia-> escreve nos leds
                   prevState_1 = gbyInput_Data[1];
                   prevState_9 = gbyInput_Data[9];
                   Board_setDigOutput(gbyInput_Data[1] | gbyInput_Data[9] );
                   UART_printf("prevstate[1] : %d\n", prevState_1 );
                   UART_printf("prevstate[9] : %d\n", prevState_9 );
                   gbyCurrentData = (prevState_1);
               }
           }

           TaskP_sleep(20);         /* update every 1ms*/

           // para o modo RT -> array gbyInput_Data[1] -> Modular_1 -> subslot Out
                           // -> array gbyInput_Data[9] -> Modular_1 -> subslot subslot_IN_OUT_OUT -> a parte do Q_out



       }
       while(1);

}

void taskPruss(UArg a0, UArg a1)
{
    char chassisMacAddress[17];
    char macAddressText[64];

    PN_socInit();
    UART_printf("\nVersion - ");
    UART_printf(IND_PKG_VERSION);

    UART_printf("\nBoard Name \t: ");
    UART_printf(boardInfo.boardName);

    UART_printf("\n\rChip Revision \t: ");
    UART_printf(boardInfo.version);

    getMacid(chassisMacAddress);
    sprintf(macAddressText, "\n\n\rChassis MAC Address - %s", chassisMacAddress);
    UART_printf(macAddressText);
    UART_printf("\r\nBuild Timestamp      : %s %s", __DATE__, __TIME__);


    UART_printf("\n\r%s Sample application on MPU \r", APP_NAME);


    if(PN_initDrv(appPnHandle) == 0)
    {
        char logText[255];
        sprintf(logText, "\n%s: v%s\r\n", APP_NAME, APP_VERSION);
        UART_printf(logText);
        sprintf(logText, "Firmware : v%d.%d.%d-%d\r\n", FIRMWARE_VERSION_MAJOR, FIRMWARE_VERSION_MINOR,FIRMWARE_VERSION_BUILD, FIRMWARE_RELEASE_TYPE);
        UART_printf(logText);

        initSNMP();                             /* enable SNMP module*/

        PRUICSS_pruEnable(prusshandle, 1);
        PRUICSS_pruEnable(prusshandle, 0);
#ifdef FACTORY_TESTING
        iRtcEnableIsr();        /* usually done inside stack...*/
        SemaphoreP_post(
            switchReady);            /* this will enable all tasks waiting for a link*/

        while(1)
        {
            TaskP_sleep(100);
        }

#else

        PN_configurePHYLeds();
        SemaphoreP_post(switchReady);            /* this will enable all tasks waiting for a link*/

        main_pn();                              /* this is a while(1) until error!*/
#endif
    }

}




/**
 * @brief alive task - indicates running host processor by blinkind LED
 * also counts states and other conditions signaling
 *
 * @param arg0 not used
 * @param arg1 not used
 */
void taskLedBlink(UArg arg0, UArg arg1)
{
    int CurrentLedsValue = 0;
    int Blink = 0;
    int i = 0;
    volatile int connectCount = 0;  /* make sure it gets written back to DRAM for monitoring via DAP*/
    int connectStat = 0;            /* 2 - drop occurred!*/

    SemaphoreP_pend(switchReady, BIOS_WAIT_FOREVER); /* wait for init*/
    SemaphoreP_post(switchReady);

    do
    {
        if(gBlinkActive == APP_TRUE)
        {
            gBlinkActive = APP_FALSE;                   /*This implements a DCP blink!*/
            for(i = 0; i < 3; i++)
            {
                Board_setTriColorLED(BOARD_TRICOLOR0_GREEN, 1);
                TaskP_sleep(500);
                Board_setTriColorLED(BOARD_TRICOLOR0_GREEN, 0);
                TaskP_sleep(500);
            }
        }

        /* set status LED for valid input*/
        switch(gbyOutputAPDU[0])
        {
            case 0xFF:
                {
                    /* Never connected */
                    if((CurrentLedsValue & BOARD_TRICOLOR1_YELLOW) == BOARD_TRICOLOR1_YELLOW)      /* LED6B Yes!*/
                    {
                        CurrentLedsValue = CurrentLedsValue & ~BOARD_TRICOLOR1_YELLOW;   /* Off LED6B*/
                        CurrentLedsValue = CurrentLedsValue | BOARD_TRICOLOR1_GREEN;     /* On LED6G*/
                    }

                    else
                    {

                        if((CurrentLedsValue & BOARD_TRICOLOR1_RED) == BOARD_TRICOLOR1_RED)      /* LED6R Yes!*/
                        {
                            CurrentLedsValue = CurrentLedsValue & ~BOARD_TRICOLOR1_RED;    /*Off LED6R*/
                            CurrentLedsValue = CurrentLedsValue | BOARD_TRICOLOR1_YELLOW;  /* On LED6B*/
                        }

                        else
                        {
                            CurrentLedsValue = CurrentLedsValue & ~BOARD_TRICOLOR1_GREEN;  /* Off LED6G*/
                            CurrentLedsValue = CurrentLedsValue | BOARD_TRICOLOR1_RED; /* On LED6R*/
                        }
                    }

                    if(connectStat == 1)
                    {
                        connectStat = 0;
                    }

                    break;
                }

            case 0x35:
                {
                    /* connected */
                    CurrentLedsValue = CurrentLedsValue & ~(BOARD_TRICOLOR1_YELLOW |  BOARD_TRICOLOR1_RED) ;      /* Off LED6B/6R*/
                    CurrentLedsValue = CurrentLedsValue | BOARD_TRICOLOR1_GREEN;                /* On LED6G*/

                    if(connectStat == 0)
                    {
                        connectStat = 1;
                        connectCount++;

                        if(1 == nodeAddr)
                        {
                            char line2[20];
                            sprintf(line2, "Cnt: %d", connectCount);
                        }
                    }

                    break;
                }

            default:/*Not connected */
                CurrentLedsValue = CurrentLedsValue & ~(BOARD_TRICOLOR1_GREEN |
                                                        BOARD_TRICOLOR1_RED);        /*Off LED6G/6R*/
                CurrentLedsValue = CurrentLedsValue |
                                   BOARD_TRICOLOR1_YELLOW;               /* On LED6B*/

                if(connectStat == 1)
                {
                    connectStat = 0;

                    if(1 == nodeAddr)
                    {
                        char line2[20];
                        sprintf(line2, "Cnt: %d", connectCount);
                    }
                }
        }

        /* small blink on current value only for LED6 */
        Board_setTriColorLED(BOARD_TRICOLOR1_YELLOW | BOARD_TRICOLOR1_RED |BOARD_TRICOLOR1_GREEN , 0);

        OsWait_ms(200);

        if((CurrentLedsValue & BOARD_TRICOLOR0_GREEN) == BOARD_TRICOLOR0_GREEN){/*Blink management*/       /* LED5G Yes!*/
            Blink ++ ;
            CurrentLedsValue = CurrentLedsValue & ~BOARD_TRICOLOR0_GREEN; /* LED OFF*/        /* LED5G Off*/
        }
        Board_setTriColorLED(CurrentLedsValue , 1);
        OsWait_ms(200);
    }
    while(1);
}




















//----------------------------------------------------------------------------------------------------------------------------------
void PN_initDefaultIRTHandle(PN_Handle pnHandle, ICSS_EmacHandle emacHandle){

    /* Memory allocations */
    /* Allocate memory of PN Lists */
    pnHandle->cpmList = (t_descList *)malloc(sizeof(t_descList));
    pnHandle->ppmList = (t_descList *)malloc(sizeof(t_descList));

    /* Allocate memory of PN config struct */
    pnHandle->currPN = (t_cfgPN *)malloc(sizeof(t_cfgPN));

    /* Allocate memory of PN mem block pointers */
    pnHandle->ppmBlock = (t_ppmBlock *)malloc(PTCP_NUM_PORTS * sizeof(t_ppmBlock));
    pnHandle->cpmBlock = (t_cpmBlock *)malloc(sizeof(t_cpmBlock));

    /* Allocate memory of PN Legact packet */
    pnHandle->pLegPkt = (t_rtcPacket *)malloc(sizeof(t_rtcPacket));

    /* Allocate memory of PN Interrupt config struct */
     pnHandle->pnIntConfig = (PN_IntConfig *)malloc(sizeof(PN_IntConfig));
    (pnHandle->pnIntConfig)->ppmIntConfig = (PN_IntAttrs *)malloc(sizeof(PN_IntAttrs));
    (pnHandle->pnIntConfig)->cpmIntConfig = (PN_IntAttrs *)malloc(sizeof(PN_IntAttrs));
    (pnHandle->pnIntConfig)->dhtIntConfig = (PN_IntAttrs *)malloc(sizeof(PN_IntAttrs));
    (pnHandle->pnIntConfig)->ptcpIntConfig = (PN_IntAttrs *)malloc(sizeof(PN_IntAttrs));

    /* Allocate memory of PN PTCP Config struct */
    pnHandle->pnPtcpConfig = (PN_PtcpConfig *)malloc(sizeof(PN_PtcpConfig));
    (pnHandle->pnPtcpConfig)->deviceSyncInfo = (ptcpSyncInfo_t *)malloc(sizeof(ptcpSyncInfo_t));
    (pnHandle->pnPtcpConfig)->currentPtcpStatus = (currentPtcpStatus_t *)malloc(sizeof(currentPtcpStatus_t));

    (pnHandle->pnPtcpConfig)->portTimes = (ptcpPortDelayVal_t *)malloc(
               PTCP_NUM_PORTS * sizeof(ptcpPortDelayVal_t));
       (pnHandle->pnPtcpConfig)->deviceDelays = (deviceDelays_t *)malloc(
                   PTCP_NUM_PORTS * sizeof(deviceDelays_t));
       (pnHandle->pnPtcpConfig)->devicePortOffsets = (devicePortOffsets_t *)malloc(
                   PTCP_NUM_PORTS * sizeof(devicePortOffsets_t));
       (pnHandle->pnPtcpConfig)->pnPtcpDebugAttrs = (PN_PtcpDebug *)malloc(sizeof(
                   PN_PtcpDebug));

       (pnHandle->pnPtcpConfig)->ptcpTimer = (PN_PtcpTimerAttrs*)malloc(sizeof(PN_PtcpTimerAttrs));
       /*configure the pulse width for sync0: 5000+1 cycles i.e. 25us*/
       (pnHandle->pnPtcpConfig)->ptcpSync0PinPulseWidth = 5000;
       /*program cmp1 reg with period, used for sync0 signal generation: 10 us*/
       (pnHandle->pnPtcpConfig)->ptcpSync0PinStart = 10000;
       (pnHandle->pnPtcpConfig)->ptcpSyncFilterfactor = 8;

       /*Allocating memory for ISOM structure*/
       pnHandle->pnIsoMConfig = (PN_IsoMConfig*)malloc(sizeof(PN_IsoMConfig));
       (pnHandle->pnIsoMConfig)->isoMIntConfig = (PN_IntAttrs *)malloc(sizeof(PN_IntAttrs));

       (pnHandle->pnIsoMConfig)->isoMIntCreateFlag = 0;
       (pnHandle->pnIsoMConfig)->isoMNumEvents = 0;
       ((pnHandle->pnIsoMConfig)->isoMIntConfig)->isrFnPtr = NULL;
       ((pnHandle->pnIsoMConfig)->isoMIntConfig)->args = NULL;

       /* Assigning default values */
       /* Assign emac Handle */
       pnHandle->emacHandle = emacHandle;
       pnHandle->initRtcDrvFlag = TRUE;
       pnHandle->initRtcMemFlag = 0;
       pnHandle->mrpState = MRPREADY;
       pnHandle->legState = NOINIT;
       (pnHandle->pnPtcpConfig)->cycleCtrInitPending = 0;
       (pnHandle->pnPtcpConfig)->calculatedCycleCtr = 0;
       (pnHandle->pnPtcpConfig)->masterChange = 0;
       (pnHandle->pnPtcpConfig)->phaseCtrChange = 0;
       (pnHandle->pnPtcpConfig)->maxSeqId = 0;
       (pnHandle->pnPtcpConfig)->minSeqId = 0;
       (pnHandle->pnPtcpConfig)->edmaEcapEvt0ChNum = 38U;
       (pnHandle->pnPtcpConfig)->edmaEcapEvt0LinkChNum = 39U;
       (pnHandle->pnPtcpConfig)->edmaEcapClearChNum = 40U;
       (pnHandle->pnPtcpConfig)->edmaEcapClearLinkChNum = 41U;

       (pnHandle->pnPtcpConfig)->pnPtcpDebugAttrs->maxDeltaT = 0;
       (pnHandle->pnPtcpConfig)->pnPtcpDebugAttrs->minDeltaT = 0;
       (pnHandle->pnPtcpConfig)->pnPtcpDebugAttrs->deltaTOutOfRange = 0;
       (pnHandle->pnPtcpConfig)->pnPtcpDebugAttrs->maxDelay = 0;
       (pnHandle->pnPtcpConfig)->pnPtcpDebugAttrs->minDelay = 0;
       (pnHandle->pnPtcpConfig)->pnPtcpDebugAttrs->delayOutOfRange = 0;

       (pnHandle->pnIntConfig)->ppmIntConfig->isrFnPtr = &PN_ppmIsrHandler;
       (pnHandle->pnIntConfig)->ppmIntConfig->args = pnHandle;
       (pnHandle->pnIntConfig)->cpmIntConfig->isrFnPtr = &PN_cpmIsrHandler;
       (pnHandle->pnIntConfig)->cpmIntConfig->args = pnHandle;
       (pnHandle->pnIntConfig)->dhtIntConfig->isrFnPtr = &PN_dhtIsrHandler;
       (pnHandle->pnIntConfig)->dhtIntConfig->args = pnHandle;
   #ifdef PTCP_SUPPORT
       (pnHandle->pnIntConfig)->ptcpIntConfig->isrFnPtr = &PN_PTCP_isrHandler;
       (pnHandle->pnIntConfig)->ptcpIntConfig->args = pnHandle;
   #endif

       PN_socinitIRTHandle(pnHandle);


}

//-----------------------------------------------------------------------------------------------------------------------------------
uint8_t ICSSEmacDRVInit(ICSS_EmacHandle handle, uint8_t instance){
    int8_t retVal = -1;

     /* LLD attributes mallocs */
     handle->object = (ICSS_EmacObject *)malloc(sizeof(ICSS_EmacObject));
     handle->hwAttrs = (ICSS_EmacHwAttrs *)malloc(sizeof(ICSS_EmacHwAttrs));

     /* Callback mallocs */
     ICSS_EmacCallBackObject *callBackObj = (ICSS_EmacCallBackObject *)malloc(sizeof(
             ICSS_EmacCallBackObject));

     callBackObj->learningExCallBack = (ICSS_EmacCallBackConfig *)malloc(sizeof(
                                           ICSS_EmacCallBackConfig));

     callBackObj->rxRTCallBack = (ICSS_EmacCallBackConfig *)malloc(sizeof(
                                     ICSS_EmacCallBackConfig));
     callBackObj->rxCallBack = (ICSS_EmacCallBackConfig *)malloc(sizeof(
                                   ICSS_EmacCallBackConfig));
     callBackObj->txCallBack = (ICSS_EmacCallBackConfig *)malloc(sizeof(
                                   ICSS_EmacCallBackConfig));

     ((ICSS_EmacObject *)handle->object)->callBackHandle = callBackObj;

     /*Allocate memory for learning*/
     ((ICSS_EmacObject *)handle->object)->macTablePtr = (HashTable_t *)malloc(
                 PTCP_NUM_PORTS * sizeof(HashTable_t));

     /*Allocate memory for PRU Statistics*/
     ((ICSS_EmacObject *)handle->object)->pruStat = (ICSS_EmacPruStatistics_t *)
             malloc(PTCP_NUM_PORTS * sizeof(ICSS_EmacPruStatistics_t));

     /*Allocate memory for Host Statistics*/
     ((ICSS_EmacObject *)handle->object)->hostStat = (ICSS_EmacHostStatistics_t *)
             malloc(PTCP_NUM_PORTS * sizeof(ICSS_EmacHostStatistics_t));

     /*Allocate memory for Storm Prevention*/
     ((ICSS_EmacObject *)handle->object)->stormPrevPtr = (stormPrevention_t *)malloc(
                 PTCP_NUM_PORTS * sizeof(stormPrevention_t));

     /* Base address initialization */
     if(NULL == ((ICSS_EmacHwAttrs *)handle->hwAttrs)->emacBaseAddrCfg)
     {
         ((ICSS_EmacHwAttrs *)handle->hwAttrs)->emacBaseAddrCfg =
             (ICSS_EmacBaseAddressHandle_T)malloc(sizeof(ICSS_EmacBaseAddrCfgParams));
     }


     ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)
             handle->hwAttrs)->emacBaseAddrCfg;

     ICSS_EmacSocGetInitCfg((instance-1), emacBaseAddr );
     emacBaseAddr->dataRam0BaseAddr = icss_EmacBaseAddrCfgParams[instance -
                                      1].dataRam0BaseAddr;
     emacBaseAddr->dataRam1BaseAddr = icss_EmacBaseAddrCfgParams[instance -
                                      1].dataRam1BaseAddr;
     emacBaseAddr->l3OcmcBaseAddr =  icss_EmacBaseAddrCfgParams[instance -
                                     1].l3OcmcBaseAddr;
     emacBaseAddr->prussCfgRegs =  icss_EmacBaseAddrCfgParams[instance -
                                   1].prussCfgRegs;
     emacBaseAddr->prussIepRegs =  icss_EmacBaseAddrCfgParams[instance -
                                   1].prussIepRegs;
     emacBaseAddr->prussIntcRegs = icss_EmacBaseAddrCfgParams[instance -
                                   1].prussIntcRegs;
     emacBaseAddr->prussMiiMdioRegs = icss_EmacBaseAddrCfgParams[instance -
                                      1].prussMiiMdioRegs;
     emacBaseAddr->prussMiiRtCfgRegsBaseAddr = icss_EmacBaseAddrCfgParams[instance -
             1].prussMiiRtCfgRegsBaseAddr;
     emacBaseAddr->prussPru0CtrlRegs = icss_EmacBaseAddrCfgParams[instance -
                                       1].prussPru0CtrlRegs;
     emacBaseAddr->prussPru1CtrlRegs = icss_EmacBaseAddrCfgParams[instance -
                                       1].prussPru1CtrlRegs;
     emacBaseAddr->sharedDataRamBaseAddr = icss_EmacBaseAddrCfgParams[instance -
                                           1].sharedDataRamBaseAddr;
     return retVal;

}
//--------------------------------------------------------------------------------------------------------------------------------------

void PN_writeFirmwareFeature(){

    uint32_t * relAddr = (uint32_t *)(((ICSS_EmacHwAttrs*)(appEmacHandle->hwAttrs))->emacBaseAddrCfg->dataRam0BaseAddr);
    *relAddr++ = ICSS_FIRMWARE_RELEASE_1;
    *relAddr++ =  ICSS_FIRMWARE_RELEASE_2;
    *relAddr = ICSS_FIRMWARE_FEATURE_MASK;
    relAddr = (uint32_t *)(((ICSS_EmacHwAttrs*)(appEmacHandle->hwAttrs))->emacBaseAddrCfg->dataRam1BaseAddr);
    *relAddr++ = ICSS_FIRMWARE_RELEASE_1;
    *relAddr++ =  ICSS_FIRMWARE_RELEASE_2;
    *relAddr = ICSS_FIRMWARE_FEATURE_MASK;
}

void getMacid(char *buf_str)
{
    int i;
    Uint8 bMacAddr[6];
    NIMU_IF_REQ if_req;
    if_req.name[0] = 0;
    if_req.index   = 1;

    NIMUIoctl(NIMU_GET_DEVICE_MAC, &if_req, &bMacAddr, sizeof(bMacAddr));

    char *buf_ptr = buf_str;

    for(i = 0; i < 5; i++)
    {
        if(i != 5)
        {
            buf_ptr += sprintf(buf_ptr, "%02x:", (char)bMacAddr[i]);
        }
    }

    buf_ptr += sprintf(buf_ptr, "%02x", (char)bMacAddr[5]);
}


