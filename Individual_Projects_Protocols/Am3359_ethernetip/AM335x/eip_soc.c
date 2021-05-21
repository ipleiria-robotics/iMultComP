/**
* @file eip_soc.c
*
* @brief Contains AM335x SoC specific intialization functions for HSR PRP protocol
*
* \par
* Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
* \par
*
*/
#include <Include/protocol/eip_soc.h>

#include <Include/board/board_spi.h>

#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_v1.h>
#include <ti/csl/src/ip/gpio/V1/gpio_v2.h>
#include <ti/drv/spi/SPI.h>
#include <ti/board/board.h>
#include <Include/board/oled_drv.h>
#include <Include/board/board_tlkphy.h>
#include <ti/drv/pruss/soc/pruicss_v1.h>
#include <ti/drv/icss_emac/icss_emacDrv.h>
#include "ti/starterware/include/soc_control.h"
#include "ti/starterware/include/chipdb.h"
#include "ti/starterware/include/prcm.h"
#include <Include/board/board_i2cLed.h>

extern ICSS_EmacHandle emachandle;
extern PRUICSS_Handle prusshandle;
extern TimeSync_ParamsHandle_t timeSyncHandle;
extern EIP_DLRHandle dlrHandle;

#define MCSPI_INSTANCE         (3U)

extern Board_flashHandle flashHandle;
extern SPI_Handle handle;

void EIP_configureInterrupts(ICSSEMAC_InitConfig *switchEmacCfg)
{
    /* Interrupt configuration */
    /*ARM interrupt number 20 on AM3x, 20+32 on AM4x, 20+100+32 on AM5x*/
    switchEmacCfg->rxIntNum = EIP_RX_INT_NUM;
    /*ARM interrupt number 26 on AM3x, 26+32 on AM4x, 26+100+32 on AM5x*/
    switchEmacCfg->linkIntNum = EIP_LINK_INT_NUM;

    /*Configure Time Sync interrupts*/
    timeSyncHandle->timeSyncConfig.txIntNum = EIP_PTP_INT_NUM;

    /*Configure DLR*/
    dlrHandle->dlrObj->port0IntNum = EIP_DLR_P0_INT_NUM;
    dlrHandle->dlrObj->port1IntNum = EIP_DLR_P1_INT_NUM;
    dlrHandle->dlrObj->beaconTimeoutIntNum_P0 = EIP_BEACON_TIMEOUT_INT_NUM_P0;
    dlrHandle->dlrObj->beaconTimeoutIntNum_P1 = EIP_BEACON_TIMEOUT_INT_NUM_P1;
}


void EIP_initICSSmode()
{
    GPIOModuleEnable(SOC_GPIO_0_REGS);
    GPIOModuleEnable(SOC_GPIO_3_REGS);
    GPIODirModeSet(SOC_GPIO_3_REGS, 10, GPIO_DIR_OUTPUT);
    GPIODirModeSet(SOC_GPIO_3_REGS, 4, GPIO_DIR_OUTPUT);
    GPIODirModeSet(SOC_GPIO_0_REGS, 7, GPIO_DIR_OUTPUT);
    GPIOPinWrite(SOC_GPIO_3_REGS, 4, 1);
    GPIOPinWrite(SOC_GPIO_3_REGS, 10, 0);
    GPIOPinWrite(SOC_GPIO_0_REGS, 7, 0);
}

void EIP_socHwinit(uint8_t instance)
{
#ifdef iceAMIC11x
    Board_init(BOARD_INIT_MODULE_CLOCK);
    Board_phyReset(2);
    /* mux PRU MII after PHY reset in case PRU drives signals
     * on the pins and interfere with the PHY bootstrap configurations
    */
    Board_init(BOARD_INIT_ICSS_PINMUX | BOARD_INIT_UART_STDIO |
               BOARD_INIT_ICSS_ETH_PHY);
#else
    Board_init(BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_ICSS_PINMUX | BOARD_INIT_UART_STDIO | BOARD_INIT_ICSS_ETH_PHY);
#endif

    PRCMModuleEnable(CHIPDB_MOD_ID_PWMSS, 0, 0);
    /* Clock source selection */
    SOCCtrlTimerClkSrcSelect(EIP_TIMER_ID + 2,
                             SOC_CTRL_DMTIMER_CLK_SRC_M_OSC_24M);
    PRCMModuleEnable(CHIPDB_MOD_ID_DMTIMER, EIP_TIMER_ID + 2, FALSE);
    /*Pinmux to configure UART1_CTSn pin in Latch0 Mode. AM335x ICE REV2.1*/
    HWREG(0x44E10978) = 0x0000002E;

    EIP_initICSSmode();

#ifndef iceAMIC11x
    Board_phyReset(2);
    
    Board_i2cLedInit();
#endif
}

void EIP_socInit()
{
    SPI_init();
    SPI_Params spiParams;                /* SPI params structure */
    SPI_Params_init(&spiParams);
    spiParams.frameFormat  = SPI_POL1_PHA0;
    handle = SPI_open(0, &spiParams);

    /* Always call Flash Init for AM335x after HVS Init as HVS init resets the SPI settings */
    /* Flash Init */

    flashHandle = Board_flashOpen((uint32_t)NULL, (uint32_t)NULL, (void *)NULL);

    gpioLedPinmuxConfig();
}

void EIP_socgetMACAddress(uint8_t *lclMac)
{
    SOCCtrlGetPortMacAddr(1, lclMac);
}

void EIP_configurePHYLeds()
{
#ifndef iceAMIC11x
    Board_phyLedConfig((((PRUICSS_HwAttrs *)(
                             prusshandle->hwAttrs))->prussMiiMdioRegBase),
                       ((ICSS_EmacObject *)emachandle->object)->emacInitcfg->phyAddr[0],
                       LED_CFG_MODE2);
    Board_phyLedConfig((((PRUICSS_HwAttrs *)(
                             prusshandle->hwAttrs))->prussMiiMdioRegBase),
                       ((ICSS_EmacObject *)emachandle->object)->emacInitcfg->phyAddr[1],
                       LED_CFG_MODE2);
#endif
}
