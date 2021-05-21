/**
* @file main.c
*
* @brief Basic example for an EtherNet/IP device running on AM57xx devices.
* Requires a full ICSS system (both PRUs) and two external phys connected to
* ICSS MII/MDIO interfaces
*
* \par
* Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
* \par
*
*/

/*! \mainpage EthernetIP Adapter Device Application
 * \version 1.0.1
 *
 * \section intro_sec Introduction
 *
 * This is an Ethernet/IP adapter device application example based on IA-SDK. It uses
 * Molex Ethernet/IP and ACD stack. This example may be used for evaluation and test of
 * the TI Ethernet/IP adapter solution. It may also serve as a starting project for
 * customer applications.
 *
 * \section docs_sec More Documentation
 * Further documentation for the required components:
 * \subsection EthernetIP_driver ICSS API
 * IA-SDK: /sdk/protocols/ethernetip_adapter/Docs/ICSS_API/html/index.html
 * \subsection eip_stack EthernetIP Slave Stack
 * Molex: tbd - some material/source here: /sdk/protocols/ethernetip_adapter/Docs
 * \subsection

 * \section install_sec Installation
 *
 * Build any configuration of the examples and use resulting binaries on
 * Am57xx boards.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
/* TI-RTOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/ndk/inc/stkmain.h>
/* PDK Header files */
#include <ti/osal/osal.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/i2c/I2C.h>
#include <ti/drv/spi/SPI.h>

#include <ti/drv/icss_emac/icss_emac_ver.h>

#include <ti/drv/icss_emac/src/icss_emacLoc.h>

#include <ti/drv/icss_emac/icss_emacFwInit.h>
#include <ti/drv/icss_emac/icss_emacLearning.h>
#include <ti/drv/icss_emac/icss_emacStatistics.h>
#include <ti/drv/icss_emac/icss_emacDrv_Def.h>
#include <ti/drv/icss_emac/icss_emacStormControl.h>
#include <ti/transport/ndk/nimu_icss/example/src/osdrv_ndkdeviceconfig.h>
#include <ti/drv/icss_emac/icss_emacDrv.h>
#include <ti/drv/icss_emac/soc/icss_emacSoc.h>
#include <ti/drv/pruss/soc/pruicss_v1.h>
#include <ti/csl/src/ip/icss/V1/cslr_icss_intc.h>
#include <ti/csl/src/ip/mdio/V2/cslr_mdio.h>
#include <ti/csl/src/ip/mdio/V2/csl_mdio.h>
#include <ti/csl/src/ip/icss/V1/cslr_icss_cfg.h>
#if defined(SOC_K2G)
#include <ti/csl/src/ip/bootcfg/V3/csl_bootcfgAux.h>
#include <ti/csl/src/ip/bootcfg/V3/csl_bootcfg.h>
#include <ti/board/src/iceK2G/include/iceK2G_ethernet_config.h>
#include <ti/csl/src/ip/mdio/V1/cslr_mdio.h>
#include <ti/csl/src/ip/mdio/V1/csl_mdio.h>
#include <ti/csl/src/ip/mdio/V1/csl_mdioAux.h>
#include <ti/csl/src/ip/icss/V1/cslr_icss_pru_ctrl.h>
#endif
#if !defined(SOC_K2G)
#include <ti/csl/src/ip/mdio/V2/csl_mdioAux.h>
#endif
#include <ti/transport/ndk/nimu_icss/nimu_icssEth.h>
#ifdef SOC_AM335x
#include <ti/csl/src/ip/gpio/V1/gpio_v2.h>
#include <ti/starterware/include/hw/soc_am335x.h>
#endif
#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/soc/QSPI_v1.h>
/* Flash header file */
#include <ti/drv/spi/SPIver.h>
#include <ti/drv/spi/test/qspi_flash/src/Flash_S25FL/S25FL.h>
#include <Include/board/board_i2cLed.h>
#include <Include/board/board_gpioLed.h>

#include <Include/board/board_misc.h>
#include <Include/board/board_phy.h>
#include <Include/board/board_spi.h>
#include <Include/board/board_eeprom.h>

#include <ti/starterware/include/chipdb.h>
#include <ti/starterware/include/soc_control.h>

#include <Include/firmware/ethernetip_adapter_pru0_bin.h>
#include <Include/firmware/ethernetip_adapter_pru1_bin.h>
#include <Include/protocol/timeSync/icss_timeSyncApi.h>
#include <Include/protocol/timeSync/icss_timeSync.h>
#include <Include/protocol/timeSync/icss_timeSync_init.h>

#include <Include/protocol/icss_ptpd_driver.h>

#include <Include/firmware/version.h>
#include <Include/protocol/eip_main.h>
#include <Include/protocol/icss_dlr.h>
#include <Include/protocol/app_restart.h>
#include <Include/firmware/tiswitch_pruss_intc_mapping.h>
#include <Include/protocol/eip_soc.h>


#ifdef SOC_K2G
#define CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_MASK                 (0x3C000000U)
#define CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_SHIFT                (26U)
#define CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_RESETVAL             (0x00000000U)
#define CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_MAX                  (0x00000004U)
#define CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_MII_MODE_VAL         (2u)
#endif

/**PTP EDMA scratch memory offsets in L3 OCMC*/
#define IEP_CONFIG_ADJ_OFFSET               0xC200

/**
 * @def ECAP_CLR_CONFIG_OFFSET
 *      L3 OCMC memory offset where
 *      ECAP Config used for IEP adjustment is stored
 *      Size is 4 bytes
 */
#define ECAP_CLR_CONFIG_OFFSET              IEP_CONFIG_ADJ_OFFSET + 0x10

/* Board specific definitions */
#ifdef SOC_AM437x
#define MCSPI_INSTANCE         (2U)
#else
#define MCSPI_INSTANCE         (3U)
#endif

#define QSPI_PER_CNT            (1U)
#define QSPI_INSTANCE           (1U)
#define QSPI_OFFSET             (4U)
/** name of the C struct in PRU header file*/
#define PRU0_FIRMWARE_NAME      PRU0_FIRMWARE
#define PRU1_FIRMWARE_NAME      PRU1_FIRMWARE


/**Task handle for EIP*/
Task_Handle main_task;
/**Semaphore to indicate the NDK thread about EIP init*/
Semaphore_Handle appInitSem;
void getMacid(char *buf_str);

/** \brief PRU-ICSS LLD handle */
PRUICSS_Handle prusshandle;
/** \brief ICSSEmac LLD handle */
ICSS_EmacHandle emachandle;


#if defined(SOC_AM335x)
Board_flashHandle flashHandle;
#elif defined(SOC_K2G)
extern Board_flashHandle flashHandle;
extern Board_flashHandle boardFlashHandle;
#else
extern S25FL_Handle flashHandle;
#endif

SPI_Handle handle;

uint8_t lclMac[6];
Board_IDInfo boardInfo;
uint32_t ipAddress;

TimeSync_ParamsHandle_t timeSyncHandle;
/** EIP driver handle */
EIP_Handle eipHandle;
/** EIP DLR driver  handle */
EIP_DLRHandle dlrHandle;
extern int collision_pkt_dropped;
extern int num_of_collision_occured;

extern void Board_initSPI(void);
extern void gpioLedPinmuxConfig();


extern PRUICSS_Config pruss_config[2 + 1];
extern ICSS_EmacBaseAddrCfgParams icss_EmacBaseAddrCfgParams[3];

/*-----------------------PTP global variables---------------------*/

/**When clock goes into sync for very time
 * set this.
 */
uint8_t deviceInSync = 0;
/**If clock goes out of sync and dut was in sync
 * then set this*/
uint8_t ptpError = 0;

/**PTP MAC ID for comparison*/
uint8_t timeSyncMAC[6] = {0x1, 0x1b, 0x19, 0x0, 0x0, 0x0};
uint8_t linkLocalMAC[6] = {0x1, 0x80, 0xc2, 0x0, 0x0, 0xE};

#ifdef PTP_TESTING

/*Min/max PTP offsets used for tracking offset from master*/
extern int32_t min_offset;
extern int32_t max_offset;

#endif //PTP_TESTING

/**Temporary placeholder to copy packets*/
uint8_t  tempFrame[ICSS_EMAC_MAXMTU];

/**
 *  @brief  Function to initialize the DLR driver handle
 *
 *          Does Memory allocation for  DLR driver
 *
 *
 *  @param   dlrHandle   [in]  DLR driver handle
 *  @param   emachandle  [in]  EMAC LLD handle
 *
 *  @retval  none
 *
 */
void EIPAPP_initICSSDlrHandle(EIP_DLRHandle dlrHandle,
                              ICSS_EmacHandle emachandle);
/** \brief stores the board type */


/**
 *  @brief  Function to initialize the EMAC LLD handle
 *
 *          This Function does the memory allocation for the Emac handle. Sets the base address in
 *          the handle.
 *          Crossbar IRQ configuration is also done here
 *
 *
 *  @param   handle    [in]  EMAC LLD handle
 *  @param   instance  [in]  PRUSS instance number
 *  @retval  Success  -   0
 *           Error   -   <0
 */
uint8_t EIPAPP_initICSSEmac(ICSS_EmacHandle handle, uint8_t instance);

/**
 *  @brief  Function to initialize the PTP driver handle
 *
 *          Does Memory allocation, EDMA param configuration
 *
 *
 *  @param   ptpHandle   [in]  PTP driver handle
 *  @param   emachandle  [in]  EMAC LLD handle
 *
 *  @retval  none
 *
 */
int8_t EIPAPP_initICSSPtpHandle(TimeSync_ParamsHandle_t timeSyncHandle,
                                ICSS_EmacHandle emachandle);

/**
 * @brief   Initializes QSPI Driver
 *
 */
void EIPAPP_taskFxn(UArg a0, UArg a1);

/**
 *  @brief  Function which calls an API in the event of a synchronization loss
 *
 *  @retval  None
 */
void EIP_syncLossCallback();


/***********************************************************************/
/* Function Definitions                                                */
/***********************************************************************/


void EIP_syncLossCallback()
{
    /*This indicates a loss of time sync
      Call your function here which handles sync loss*/

    return;
}

/**
 *  @brief  PRUSS Task Definition
 *
 *          Task to load the firmware
 *
 *  @param   a0  [in] argument0
 *  @param   a1  [in] argument1
 *
 *  @retval  none
 *
 */
void EIPAPP_taskPruss(UArg a0, UArg a1)
{
    uint8_t firmwareLoad_done = 0;
    EIP_socInit();

    GPIO_init();

    PRUICSS_pruDisable(prusshandle, ICSS_EMAC_PORT_1 - 1);
    PRUICSS_pruDisable(prusshandle, ICSS_EMAC_PORT_2 - 1);

    if(PRUICSS_pruWriteMemory(prusshandle, PRU_ICSS_IRAM(0) , 0, (uint32_t *) PRU0_FIRMWARE_NAME, sizeof(PRU0_FIRMWARE_NAME)))
    {
        if(PRUICSS_pruWriteMemory(prusshandle, PRU_ICSS_IRAM(1) , 0, (uint32_t *) PRU1_FIRMWARE_NAME, sizeof(PRU1_FIRMWARE_NAME)))
        {
            firmwareLoad_done = TRUE;
        }
    }

    if(firmwareLoad_done)
    {
        PRUICSS_pruEnable(prusshandle, ICSS_EMAC_PORT_1 - 1);
        PRUICSS_pruEnable(prusshandle, ICSS_EMAC_PORT_2 - 1);
    }
}

/**
 *  @brief  EIP Main Task Definition
 *
 *          Prints the application info and calls EIP start
 *
 *  @param   a0  [in] argument0
 *  @param   a1  [in] argument1
 *
 *  @retval  none
 *
 */
void EIPAPP_taskFxn(UArg a0, UArg a1)
{
    UART_printf("\nVersion - ");
    UART_printf(IND_PKG_VERSION);

    UART_printf("\nBoard name \t: ");
    UART_printf(boardInfo.boardName);

    UART_printf("\n\rChip Revision \t: ");
    UART_printf(boardInfo.version);

    UART_printf("\n\rSYS/BIOS Ethernet IP Sample application");
    UART_printf("\r\nBuild Timestamp      : %s %s", __DATE__, __TIME__);
    EIPAPP_main(emachandle);
}

/**
 *  @brief
 *
 *  @param   none
 *  @retval  none
 *
 */

/**
 *  @brief  Application entry point - initializes low level hardware, creates tasks and
 *          starts SYSBIOS
 *
 *  @param   none
 *
 *  @retval  0
 *
 */
int main()
{
    Task_Params taskParams;
    Semaphore_Params semParams1;
    PRUICSS_IntcInitData pruss_intc_initdata = PRUSS_INTC_INITDATA;

    System_atexit(EIPAPP_exitHandler);

    EIP_socHwinit(PRUICSS_INSTANCE);

    memset(&boardInfo, 0, sizeof(Board_IDInfo));
    Board_getIDInfo(&boardInfo);
    UART_printf("boardName: %s\n", boardInfo.boardName);

    prusshandle = PRUICSS_create(pruss_config, PRUICSS_INSTANCE);
    emachandle = (ICSS_EmacHandle)malloc(sizeof(ICSS_EmacConfig));

    ICSSEMAC_InitConfig *switchEmacCfg;
    switchEmacCfg = (ICSSEMAC_InitConfig *)malloc(sizeof(ICSSEMAC_InitConfig));
    switchEmacCfg->phyAddr[0] = Board_getPhyAddress(PRUICSS_INSTANCE, 1);
    switchEmacCfg->phyAddr[1] = Board_getPhyAddress(PRUICSS_INSTANCE, 2);

    switchEmacCfg->portMask = ICSS_EMAC_MODE_SWITCH;
    switchEmacCfg->ethPrioQueue = ICSS_EMAC_QUEUE3;
    switchEmacCfg->halfDuplexEnable = 1;
    switchEmacCfg->enableIntrPacing = ICSS_EMAC_DISABLE_PACING;
    switchEmacCfg->pacingThreshold = 100;

    switchEmacCfg->learningEn = 1;

    EIP_socgetMACAddress(lclMac);

    switchEmacCfg->macId = lclMac;

    EIPAPP_initICSSEmac(emachandle, PRUICSS_BASEADDRESS_INSTANCE);

    ((ICSS_EmacObject *)emachandle->object)->pruIcssHandle = prusshandle;
    ((ICSS_EmacObject *)emachandle->object)->emacInitcfg = switchEmacCfg;

    ICSS_EmacInit(emachandle, &pruss_intc_initdata, ICSS_EMAC_MODE_SWITCH);
    timeSyncHandle = (TimeSync_ParamsHandle_t)malloc(sizeof(TimeSync_ParamsHandle));


    if(EIPAPP_initICSSPtpHandle(timeSyncHandle, emachandle) != TIME_SYNC_OK)
    {
        System_abort("TimeSync memory allocation failed");
    }


    dlrHandle = (EIP_DLRHandle)malloc(sizeof(dlr_Config));
    EIPAPP_initICSSDlrHandle(dlrHandle, emachandle);

    /*Configure interrupts*/
    EIP_configureInterrupts(switchEmacCfg);

    eipHandle = (EIP_Handle)malloc(sizeof(eip_Config));
    eipHandle->dlrHandle = dlrHandle;
    eipHandle->timeSyncHandle = timeSyncHandle;
    eipHandle->emacHandle = emachandle;

    Task_Params_init(&taskParams);
    //taskParams.stackSize = 0x600;
    taskParams.priority = 5;
    taskParams.instance->name = "LEDTask";
    Task_create(EIPUTILS_ledFxn, &taskParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.priority = 1;
    taskParams.instance->name = "UartTask";
    Task_create(EIPAPP_uartTask, &taskParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.priority = 15;
    //taskParams.stackSize = 8192;
    taskParams.instance->name = "SwitchTask";
    Task_create(EIPAPP_taskPruss, &taskParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.priority = 3;
    taskParams.instance->name = "EIPTask";
    main_task = Task_create(EIPAPP_taskFxn, &taskParams, NULL);


    Semaphore_Params_init(&semParams1);
    semParams1.mode = Semaphore_Mode_BINARY;
    appInitSem = Semaphore_create(0, &semParams1, NULL);

#ifdef PTP_TESTING
    Task_Params_init(&taskParams);
    taskParams.priority = 6;
    taskParams.arg0 = (UArg)timeSyncHandle;
    taskParams.instance->name = "PTPStatusTask";
    Task_create(monitorPTPStatus_Task, &taskParams, NULL);
#endif //PTP_TESTING

    Task_Params_init(&taskParams);
    taskParams.priority = 10;
    taskParams.instance->name = (char *)"RxTask";
    taskParams.arg0 = (UArg)emachandle;

    ((ICSS_EmacObject *)emachandle->object)->rxTaskHandle = Task_create(
                (Task_FuncPtr)ICSS_EMacOsRxTaskFnc, &taskParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.priority = 7;
    taskParams.instance->name = (char *)"LinkTask";
    taskParams.arg0 = (UArg)emachandle;

    ((ICSS_EmacObject *)emachandle->object)->linkTaskHandle = Task_create((Task_FuncPtr)ICSS_EMacOsLinkTaskFnc, &taskParams, NULL);

    PRUICSS_pinMuxConfig(prusshandle, 0x0); // PRUSS pinmuxing

    if(OSDRV_addNetifEntry((NIMUInitFn)&EmacInit, emachandle) == 0)
    {
        BIOS_exit(0);
    }

    BIOS_start();
    return 0;
}
/**
 *  @brief  Function to initialize the DLR driver handle
 *
 *          Does Memory allocation for  DLR driver
 *
 *
 *  @param   dlrHandle   [in]  DLR driver handle
 *  @param   emachandle  [in]  EMAC LLD handle
 *
 *  @retval  none
 *
 */
void EIPAPP_initICSSDlrHandle(EIP_DLRHandle dlrHandle,
                              ICSS_EmacHandle emachandle)
{
    dlrHandle->emacHandle = emachandle;
    dlrHandle->dlrObj = (dlrStruct *)malloc(sizeof(dlrStruct));
    dlrHandle->exclusionList = (exceptionList *)malloc(sizeof(exceptionList));
}

/**
 *  @brief  Function to initialize the PTP driver handle
 *
 *          Does Memory allocation, EDMA param configuration
 *
 *
 *  @param   ptpHandle   [in]  PTP driver handle
 *  @param   emachandle  [in]  EMAC LLD handle
 *
 *  @retval  none
 *
 */
int8_t EIPAPP_initICSSPtpHandle(TimeSync_ParamsHandle_t timeSyncHandle,
                                ICSS_EmacHandle emachandle)
{
    int8_t returnVal = TIME_SYNC_OK;
    timeSyncHandle->emacHandle = emachandle;

    /*Configure PTP. These variables must be configured before doing anything else*/
    timeSyncHandle->timeSyncConfig.config = BOTH;
    timeSyncHandle->timeSyncConfig.type = E2E;
    timeSyncHandle->timeSyncConfig.protocol = UDP_IPV4;
    timeSyncHandle->timeSyncConfig.tickPeriod = 500;
    timeSyncHandle->txprotocol = 0;
    timeSyncHandle->timeSyncConfig.icssVersion = PRU_ICSS_VERSION;
    timeSyncHandle->edmaConfig = (emdaConfig_t *)malloc(sizeof(emdaConfig_t));
    timeSyncHandle->edmaConfig->ecapPwmssBaseAddr = EIP_CSL_ECAP_REG;
    timeSyncHandle->edmaConfig->edmaEcapEvt0ChNum = 38U;
    timeSyncHandle->edmaConfig->edmaEcapEvt0LinkChNum = 39U;
    timeSyncHandle->edmaConfig->edmaEcapClearChNum = 40U;
    timeSyncHandle->edmaConfig->edmaEcapClearLinkChNum = 41U;

    /*scratch memory for ECAP EDMA IEP compensation scheme
     * the offsets are in L3 OCMC RAM and must not overlap with memory
     * used for queues*/
    timeSyncHandle->edmaConfig->iep_config_adj_offset = IEP_CONFIG_ADJ_OFFSET;
    timeSyncHandle->edmaConfig->iep_ecap_config_offset = ECAP_CLR_CONFIG_OFFSET;
    timeSyncHandle->tsSyntInfo = (timeSync_SyntInfo_t *)malloc(sizeof(timeSync_SyntInfo_t));
    if(timeSyncHandle->tsSyntInfo == NULL)
    {
        return TIME_SYNC_UNABLE_TO_ALLOC_MEM;
    }

    timeSyncHandle->tsNrrInfo[0] = (timeSync_NrrInfo_t *)malloc(sizeof(timeSync_NrrInfo_t));

    if(timeSyncHandle->tsNrrInfo[0] == NULL)
    {
        return TIME_SYNC_UNABLE_TO_ALLOC_MEM;
    }

    timeSyncHandle->tsNrrInfo[1] = (timeSync_NrrInfo_t *)malloc(sizeof(timeSync_NrrInfo_t));

    if(timeSyncHandle->tsNrrInfo[1] == NULL)
    {
        return TIME_SYNC_UNABLE_TO_ALLOC_MEM;
    }

    timeSyncHandle->syncParam[0] = (syncParam_t *)malloc(sizeof(syncParam_t));

    if(timeSyncHandle->syncParam[0] == NULL)
    {
        return TIME_SYNC_UNABLE_TO_ALLOC_MEM;
    }

    timeSyncHandle->syncParam[1] = (syncParam_t *)malloc(sizeof(syncParam_t));

    if(timeSyncHandle->syncParam[1] == NULL)
    {
        return TIME_SYNC_UNABLE_TO_ALLOC_MEM;
    }

    timeSyncHandle->tsRunTimeVar = (timeSync_RuntimeVar_t *)malloc(sizeof(
                                       timeSync_RuntimeVar_t));

    if(timeSyncHandle->tsRunTimeVar == NULL)
    {
        return TIME_SYNC_UNABLE_TO_ALLOC_MEM;
    }

    timeSyncHandle->delayParams = (delayReqRespParams_t *)malloc(sizeof(
                                      delayReqRespParams_t));

    if(timeSyncHandle->delayParams == NULL)
    {
        return TIME_SYNC_UNABLE_TO_ALLOC_MEM;
    }

    timeSyncHandle->offsetAlgo  = (timeSync_Offset_Stable_Algo_t *)malloc(sizeof(
                                      timeSync_Offset_Stable_Algo_t));

    if(timeSyncHandle->offsetAlgo == NULL)
    {
        return TIME_SYNC_UNABLE_TO_ALLOC_MEM;
    }

    /*Allocate Rx and Tx packet buffers*/
    returnVal = TimeSync_alloc_PktBuffer(timeSyncHandle);

    if(returnVal != TIME_SYNC_OK)
    {
        return TIME_SYNC_UNABLE_TO_ALLOC_MEM;
    }

    /*Set only if a custom Tx LLD API is used*/
    timeSyncHandle->timeSyncConfig.custom_tx_api = 0;
    /*Set to 1 if Rx timestamps are coming from shared RAM*/
    timeSyncHandle->timeSyncConfig.timestamp_from_shared_ram = 1;

    /*If there is no forwarding between ports then set this*/
    timeSyncHandle->timeSyncConfig.emac_mode = 0;
    timeSyncHandle->timeSyncConfig.hsrEnabled = 0;

    timeSyncHandle->rxTimestamp_gPTP = (timeStamp *)malloc(sizeof(timeStamp));

    timeSyncHandle->timeSyncConfig.domainNumber[0] = 0;
    timeSyncHandle->timeSyncConfig.domainNumber[1] = 0;

    timeSyncHandle->timeSyncConfig.logAnnounceRcptTimeoutInterval =
        DEFAULT_ANNOUNCE_TIMEOUT_LOG_INTERVAL;
    timeSyncHandle->timeSyncConfig.logAnnounceSendInterval =
        DEFAULT_ANNOUNCE_SEND_LOG_INTERVAL;
    timeSyncHandle->timeSyncConfig.logPDelReqPktInterval =
        DEFAULT_PDELAY_REQ_LOG_INTERVAL;
    timeSyncHandle->timeSyncConfig.logSyncInterval = DEFAULT_SYNC_SEND_LOG_INTERVAL;
    /*No asymmetry*/
    timeSyncHandle->timeSyncConfig.asymmetryCorrection[0] = 0;
    timeSyncHandle->timeSyncConfig.asymmetryCorrection[1] = 0;
    timeSyncHandle->timeSyncConfig.pdelayBurstNumPkts =
        3;      /*3 frames sent in a burst*/
    timeSyncHandle->timeSyncConfig.pdelayBurstInterval =
        200;   /*gap between each frame is 100ms*/
    timeSyncHandle->timeSyncConfig.sync0_interval =
        1000000;      /*1 milisec value*/
    /*Register callback*/
    timeSyncHandle->timeSyncConfig.timeSyncSyncLossCallBackfn =
        (TimeSync_SyncLossCallBack_t)EIP_syncLossCallback;

    /*Configure Master params*/
    timeSyncHandle->timeSyncConfig.isMaster = 0;
    timeSyncHandle->timeSyncConfig.masterParams.priority1 = TIMESYNC_DEFAULT_PRIO_1;
    timeSyncHandle->timeSyncConfig.masterParams.priority2 = TIMESYNC_DEFAULT_PRIO_2;
    timeSyncHandle->timeSyncConfig.masterParams.clockAccuracy = TIMESYNC_DEFAULT_CLOCK_ACCURACY; /*greater than 10s */
    timeSyncHandle->timeSyncConfig.masterParams.clockClass = TIMESYNC_DEFAULT_CLOCK_CLASS;
    timeSyncHandle->timeSyncConfig.masterParams.clockVariance = TIMESYNC_DEFAULT_CLOCK_VARIANCE;
    timeSyncHandle->timeSyncConfig.masterParams.stepRemoved = TIMESYNC_DEFAULT_STEPS_REMOVED;
    timeSyncHandle->timeSyncConfig.masterParams.UTCOffset = TIMESYNC_UTC_OFFSET;
    timeSyncHandle->timeSyncConfig.masterParams.timeSource = TIMESYNC_DEFAULT_TIME_SOURCE; /*Internal oscillator*/

    timeSyncHandle->timeSyncConfig.masterParams.ptp_flags[TS_PTP_TIMESCALE_INDEX] = 1;
    timeSyncHandle->timeSyncConfig.masterParams.ptp_flags[TS_PTP_TWO_STEP_INDEX] = 1;

    return TIME_SYNC_OK;

}

/**
 *  @brief  Function to initialize the EMAC LLD handle
 *
 *          This Function does the memory allocation for the Emac handle. Sets the base address in
 *          the handle.
 *          Crossbar IRQ configuration is also done here
 *
 *
 *  @param   handle    [in]  EMAC LLD handle
 *  @param   instance  [in]  PRUSS instance number
 *  @retval  Success  -   0
 *           Error   -   <0
 */
uint8_t EIPAPP_initICSSEmac(ICSS_EmacHandle handle, uint8_t instance)
{
    ICSS_EmacBaseAddressHandle_T emacBaseAddr;
    ICSS_EmacCallBackObject *callBackObj;

    /* LLD attributes mallocs */
    handle->object = (ICSS_EmacObject *)malloc(sizeof(ICSS_EmacObject));
    handle->hwAttrs = (ICSS_EmacHwAttrs *)malloc(sizeof(ICSS_EmacHwAttrs));

    /* Callback mallocs */
    callBackObj = (ICSS_EmacCallBackObject *)malloc(sizeof(
                      ICSS_EmacCallBackObject));

    callBackObj->learningExCallBack = (ICSS_EmacCallBackConfig *)malloc(sizeof(
                                          ICSS_EmacCallBackConfig));
    callBackObj->rxCallBack = (ICSS_EmacCallBackConfig *)malloc(sizeof(
                                  ICSS_EmacCallBackConfig));
    callBackObj->txCallBack = (ICSS_EmacCallBackConfig *)malloc(sizeof(
                                  ICSS_EmacCallBackConfig));

    callBackObj->rxRTCallBack = (ICSS_EmacCallBackConfig *)malloc(sizeof(
                                    ICSS_EmacCallBackConfig));
    ((ICSS_EmacObject *)handle->object)->callBackHandle = callBackObj;

    /*Allocate memory for learning*/
    ((ICSS_EmacObject *)handle->object)->macTablePtr = (HashTable_t *)malloc(
                NUM_PORTS * sizeof(HashTable_t));

    /*Allocate memory for PRU Statistics*/
    ((ICSS_EmacObject *)handle->object)->pruStat = (ICSS_EmacPruStatistics_t *)
            malloc(NUM_PORTS * sizeof(ICSS_EmacPruStatistics_t));

    /*Allocate memory for Host Statistics*/
    ((ICSS_EmacObject *)handle->object)->hostStat = (ICSS_EmacHostStatistics_t *)
            malloc(NUM_PORTS * sizeof(ICSS_EmacHostStatistics_t));

    /*Allocate memory for Storm Prevention*/
    ((ICSS_EmacObject *)handle->object)->stormPrevPtr = (stormPrevention_t *)malloc(
                NUM_PORTS * sizeof(stormPrevention_t));

    /*Base address initialization*/
    if(NULL == ((ICSS_EmacHwAttrs *)handle->hwAttrs)->emacBaseAddrCfg)
    {
        ((ICSS_EmacHwAttrs *)handle->hwAttrs)->emacBaseAddrCfg =
            (ICSS_EmacBaseAddressHandle_T)malloc(sizeof(ICSS_EmacBaseAddrCfgParams));
    }

    emacBaseAddr = ((ICSS_EmacHwAttrs *)
                    handle->hwAttrs)->emacBaseAddrCfg;
    ICSS_EmacSocGetInitCfg((instance - 1), emacBaseAddr);


#if defined(SOC_AM572x) || defined(SOC_AM571x)
    EIP_crossbarConfig(instance);
#endif

    return 0;
}
