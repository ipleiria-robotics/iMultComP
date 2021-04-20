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


/** \brief PRU-ICSS LLD handle */
PRUICSS_Handle prusshandle;
/** \brief ICSSEmac LLD handle */
ICSS_EmacHandle appEmacHandle;
/** \brief Profinet handle */
PN_Handle appPnHandle;
APP_BOOL gBlinkActive = APP_FALSE;


extern PRUICSS_Config pruss_config[2 + 1];
uint8_t lclMac[6];

/******************************************************************************************************************
 *                              AUX Function                                                                *
 **************************************************************************************************************** */
uint8_t ICSSEmacDRVInit(ICSS_EmacHandle handle, uint8_t instance); // ICSS EMAC LLD handle init   --- return E_FAIL on failure, S_PASS on success



/******************************************************************************************************************
 *                              TASK MAIN PROFINET                                                                 *
 **************************************************************************************************************** */
void task_main_profinet(){

    TaskP_Params taskParams;
    SemaphoreP_Params semParams;

    ICSS_EmacFwStaticMmap *pIcssEmacStaticMMap;
    ICSS_EmacFwDynamicMmap *pIcssEmacDynamicMMap;

    PN_socHwinit(PRUICSS_INSTANCE);  // enable hw interrupt
    prusshandle = PRUICSS_create(pruss_config, PRUICSS_INSTANCE);
    appEmacHandle = (ICSS_EmacHandle)malloc(sizeof(ICSS_EmacConfig));
    appPnHandle = (PN_Handle)malloc(sizeof(PN_Config));

    ICSSEMAC_InitConfig *switchEmacCfg;
    switchEmacCfg = (ICSSEMAC_InitConfig *)malloc(sizeof(ICSSEMAC_InitConfig));

    /* PRUICSS_INSTANCE == PRUICSSx_PHY_ADDRESS */
    switchEmacCfg->phyAddr[0] = Board_getPhyAddress(PRUICSS_INSTANCE, 1);
    switchEmacCfg->phyAddr[1] = Board_getPhyAddress(PRUICSS_INSTANCE, 2);

    switchEmacCfg->portMask = ICSS_EMAC_MODE_SWITCH;  // modswitch profinet  <<<<<<<<<<<<<<<<<<<<<<<--------------------------->>>>>>>>>>>>>>>>>>>>>>>>>>
    switchEmacCfg->ethPrioQueue = ICSS_EMAC_QUEUE3;
    switchEmacCfg->halfDuplexEnable = 0;
    switchEmacCfg->enableIntrPacing = ICSS_EMAC_ENABLE_PACING;
    switchEmacCfg->ICSS_EmacIntrPacingMode = ICSS_EMAC_INTR_PACING_MODE1;
    switchEmacCfg->pacingThreshold = 100;

    switchEmacCfg->learningEn = 1;
    switchEmacCfg->macId = lclMac;
    switchEmacCfg->rxIntNum = PN_RX_INT_NUM; /*20*/
    switchEmacCfg->linkIntNum = PN_LINK_INT_NUM; /*26*/












}




