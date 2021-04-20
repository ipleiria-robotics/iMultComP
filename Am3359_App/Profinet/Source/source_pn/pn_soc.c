/**
* @file pn_soc.c
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

#include <Profinet/Include_profinet/pn_soc.h>
#include <Include/Board_am335x/board_spi.h>
#include <Profinet/Include_profinet/PN_Handle.h>
#include <Profinet/Include_profinet/PN_HandleDef.h>
#include <Profinet/Include_profinet/protocol_include/os.h>
#include <Profinet/Include_profinet/iPNLegacy.h>


#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_v1.h>
#include <ti/csl/src/ip/gpio/V1/gpio_v2.h>
#include <ti/drv/spi/SPI.h>
#include <ti/board/board.h>
#include <ti/starterware/include/prcm.h>

#include <examples/board/include/oled_drv.h>
#include <examples/board/include/board_tlkphy.h>
#include <examples/board/include/board_i2cLed.h>
#include <examples/board/include/board_phy.h>
#include "pnDrvConfig.h"
#include "firmware_version.h"


#include <ti/drv/pruss/soc/pruicss_v1.h>

#define SEMP_BLOCK_SIZE (80 * OSAL_TIRTOS_SEMAPHOREP_SIZE_BYTES)
#define MCSPI_INSTANCE         (3U)

extern ICSS_EmacHandle appEmacHandle;
extern PRUICSS_Handle prusshandle;
extern Board_IDInfo boardInfo;
extern void gpioLedPinmuxConfig();

tlkMDIXTaskParam_t tlkTaskparam;
Board_flashHandle flashHandle;
SPI_Handle handle;

uint8_t semPMemBlock[SEMP_BLOCK_SIZE];

void PN_socinitIRTHandle(PN_Handle pnHandle)
{
    /* Interrupt configuration */
    (pnHandle->pnIntConfig)->ppmIntConfig->pruIntNum = 21;
    (pnHandle->pnIntConfig)->ppmIntConfig->coreIntNum = 21;
    (pnHandle->pnIntConfig)->ppmIntConfig->socEvId = 21;
    (pnHandle->pnIntConfig)->ppmIntConfig->intPrio = 21;
    (pnHandle->pnIntConfig)->cpmIntConfig->pruIntNum = 22;
    (pnHandle->pnIntConfig)->cpmIntConfig->coreIntNum = 22;
    (pnHandle->pnIntConfig)->cpmIntConfig->socEvId = 22;
    (pnHandle->pnIntConfig)->cpmIntConfig->intPrio = 22;
    (pnHandle->pnIntConfig)->dhtIntConfig->pruIntNum = 23;
    (pnHandle->pnIntConfig)->dhtIntConfig->coreIntNum = 23;
    (pnHandle->pnIntConfig)->dhtIntConfig->socEvId = 23;
    (pnHandle->pnIntConfig)->dhtIntConfig->intPrio = 19;
#ifdef PTCP_SUPPORT
    (pnHandle->pnIntConfig)->ptcpIntConfig->pruIntNum = 24;
    (pnHandle->pnIntConfig)->ptcpIntConfig->coreIntNum = 24;
    (pnHandle->pnIntConfig)->ptcpIntConfig->socEvId = 24;
    (pnHandle->pnIntConfig)->ptcpIntConfig->intPrio = 24; /* don't care */
    
    (pnHandle->pnPtcpConfig)->ptcpEnableSlowCompensation = 0;
    (pnHandle->pnPtcpConfig)->ptcpTimer->ptcpTimerID = PN_PTCP_TIMERID;  /*DMTIMER 5. SYSBIOS Timer Id is 3*/
#endif

    ((pnHandle->pnIsoMConfig)->isoMIntConfig)->pruIntNum = 27;
    ((pnHandle->pnIsoMConfig)->isoMIntConfig)->coreIntNum = 27;
    ((pnHandle->pnIsoMConfig)->isoMIntConfig)->socEvId = 27;
    ((pnHandle->pnIsoMConfig)->isoMIntConfig)->intPrio = 27;

}


void PN_initICSSmode()
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

void PN_socHwinit(uint8_t instance)
{
    uint32_t  ctrlBitMap;
    Osal_HwAttrs      hwAttrs;
    int32_t           osal_ret;
#ifdef iceAMIC11x
    Board_init(BOARD_INIT_PLL| BOARD_INIT_MODULE_CLOCK |  BOARD_INIT_DDR | BOARD_INIT_ICSS_PINMUX | BOARD_INIT_UART_STDIO | BOARD_INIT_ICSS_ETH_PHY);
#else
    Board_init(BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_ICSS_PINMUX | BOARD_INIT_UART_STDIO | BOARD_INIT_ICSS_ETH_PHY);
#endif

    PRCMModuleEnable(CHIPDB_MOD_ID_PWMSS, 0, 0);
    /* Clock source selection */
    SOCCtrlTimerClkSrcSelect(PN_TIMER_ID + 2,
                             SOC_CTRL_DMTIMER_CLK_SRC_M_OSC_24M);
    PRCMModuleEnable(CHIPDB_MOD_ID_DMTIMER, PN_TIMER_ID + 2, FALSE);

    /*Pinmux to configure UART1_CTSn pin in Latch0 Mode. AM335x ICE REV2.1*/
    HWREG(0x44E10978) = 0x0000002E;

    PN_initICSSmode();

#ifndef iceAMIC11x
    Board_phyReset(2);
#endif
    memset(&boardInfo, 0, sizeof(Board_IDInfo));
    Board_getIDInfo(&boardInfo);

    /*Code to set number of Semaphore supported. By default PDK supports only 20 for AM335x.*/
    ctrlBitMap = OSAL_HWATTR_SET_SEMP_EXT_BASE;
    osal_ret = Osal_getHwAttrs(&hwAttrs);
    if (osal_ret != osal_OK)
    {
        return;
    }
    hwAttrs.extSemaphorePBlock.base = (uintptr_t) &semPMemBlock[0];
    hwAttrs.extSemaphorePBlock.size = SEMP_BLOCK_SIZE;

    osal_ret = Osal_setHwAttrs(ctrlBitMap, &hwAttrs);
    if (osal_ret != osal_OK)
    {
        return;
    }
}


void PN_socInit()
{
    

    SPI_init();
    SPI_Params spiParams;                /* SPI params structure */
    SPI_Params_init(&spiParams);
    spiParams.frameFormat  = SPI_POL1_PHA0;
    handle = SPI_open(0, &spiParams);
    /* Always call Flash Init for AM335x after HVS Init as HVS init resets the SPI settings */
    /* Flash Init */
    flashHandle = Board_flashOpen(0, 0, NULL);
#ifndef iceAMIC11x
    char appName[100];
    char appVer[100];
    sprintf(appName, "%s:v%s", APP_NAME, APP_VERSION);
    sprintf(appVer, "Firmware:v%d.%d.%d-%d", FIRMWARE_VERSION_MAJOR,
                FIRMWARE_VERSION_MINOR,
                FIRMWARE_VERSION_BUILD, FIRMWARE_RELEASE_TYPE);


    /* I2C Init */
    Board_i2cLedInit();

    gpioLedPinmuxConfig();
    
    Board_oledInit();
    clear();
    setline(0);
    setOrientation(1);
    printstr((int8_t *)appName);
    setline(1);
    setOrientation(1);
    printstr((int8_t *)appVer);
    scrollDisplayRight();
#endif
    tlkTaskparam.mdioBaseAddress=((ICSS_EmacHwAttrs*)appEmacHandle->hwAttrs)->emacBaseAddrCfg->prussMiiMdioRegs;
    tlkTaskparam.numPorts = 2;
    tlkTaskparam.phyAddress[0] = ((ICSS_EmacObject*)appEmacHandle->object)->emacInitcfg->phyAddr[0];
    tlkTaskparam.phyAddress[1]= ((ICSS_EmacObject*)appEmacHandle->object)->emacInitcfg->phyAddr[1];
    Board_phyMDIXFixInit(&tlkTaskparam);

}


void PN_socgetMACAddress(uint8_t index, uint8_t *lclMac)
{
    uint8_t lclEndMac[6] = {0};
    /*Get the first mac from registers*/
    SOCCtrlGetPortMacAddr(1, lclMac);
    /*Get the last/third mac from registers*/
    SOCCtrlGetPortMacAddr(2, lclEndMac);
    /*The below logic is used to identify non continuous MAC address saved in the registers. In this case
     * the first and last macaddress need to be compared and proper value should be used*/
    if((lclMac[5]+2) == lclEndMac[5])
    {
        switch(index)
        {
            case INTERFACE_MAC:
                break;
            case PORT1_MAC:
                lclMac[5] += 1;
                break;
            case PORT2_MAC:
                lclMac[5] += 2;
                break;
            default:
                break;
        }
    }
    else
    {
        if((lclMac[5] & 0xF) == 0xF)
        {
            switch(index)
            {
                case INTERFACE_MAC:
                    break;
                case PORT1_MAC:
                    memcpy(lclMac,lclEndMac,6);
                    lclMac[5] -= 1;
                    break;
                case PORT2_MAC:
                    memcpy(lclMac,lclEndMac,6);
                    break;
                default:
                   break;
            }
        }
        else if ((lclMac[5] & 0xF) == 0xE)
        {
            switch(index)
            {
                case INTERFACE_MAC:
                    break;
                case PORT1_MAC:
                    lclMac[5] += 1;
                    break;
                case PORT2_MAC:
                    memcpy(lclMac,lclEndMac,6);
                    break;
                default:
                   break;
            }
        }

    }
}

void PN_configurePHYLeds()
{
#ifndef iceAMIC11x
    Board_phyLedConfig((((PRUICSS_HwAttrs *)(
            prusshandle->hwAttrs))->prussMiiMdioRegBase),
                       ((ICSS_EmacObject *)appEmacHandle->object)->emacInitcfg->phyAddr[0],
                       LED_CFG_MODE2);
    Board_phyLedConfig((((PRUICSS_HwAttrs *)(
            prusshandle->hwAttrs))->prussMiiMdioRegBase),
                       ((ICSS_EmacObject *)appEmacHandle->object)->emacInitcfg->phyAddr[1],
                       LED_CFG_MODE2);
#endif
}
