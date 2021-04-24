/**
 * tiesc_soc.c
 *
*/
/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/

#include <stdio.h>
#include <string.h>

#include <ti/starterware/include/hw/soc_am335x.h>
#include <ti/starterware/include/am335x/hw_prm_device.h>
#include <ti/starterware/include/am335x/hw_cm_per.h>

/* Define types used by cslr_spinlock.h */
#define Uint8 uint8_t
#define Uint32 uint32_t
#include <ti/csl/src/ip/spinlock/V0/cslr_spinlock.h>


#include <Ethercat/Include_ethercat/tiescbsp.h>
#include <Ethercat/Include_ethercat/tiesc_soc.h>

#include <ti/drv/i2c/I2C.h>
#include <ti/drv/spi/SPI.h>


#ifndef DISABLE_UART_PRINT
#include <ti/drv/uart/UART_stdio.h>
#endif
#include <ti/board/board.h>
#include <Include/Board_am335x/board_misc.h>
#include <ti/csl/src/ip/mdio/V2/cslr_mdio.h>
#include <ti/csl/src/ip/mdio/V2/csl_mdio.h>

#include <ti/drv/gpio/GPIO.h>
#include <Include/Board_am335x/board_gpioLed.h>
#include <Include/Board_am335x/board_spi.h>
#include <Include/Board_am335x/board_i2cLed.h>
#include <Include/Board_am335x/board_mcspi.h>
#include <Include/Board_am335x/board_phy.h>

#ifndef iceAMIC11x
#include <Include/Board_am335x/oled_drv.h>
#include <Include/Board_am335x/board_rotary_switch.h>
#endif
#include <Ethercat/Include_ethercat/version.h>

#include <ti/drv/spi/soc/QSPI_v1.h>

#include <ti/csl/soc.h>
#include <ti/csl/src/ip/icss/V1/cslr_icss_cfg.h>
#include <ti/csl/csl_gpio.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>
#include <ti/drv/spi/soc/SPI_soc.h>

/* Board specific definitions */
#define ARM_INTERRUPT_OFFSET    0
//Not available via CSLs
#define  CONTROL_DEV_FEATURE 0x604   // ---> include hw_control_am335x.h
#define  SOC_SPINLOCK_REGS  (0x480ca000U)

Board_flashHandle flashHandle;

SPI_Handle handle;                   /* HVS SPI handle */

extern PRUICSS_Handle pruIcss1Handle;
extern PRUICSS_Config pruss_config[2 + 1];

/** \brief stores the board type */
uint8_t board_type = 0;


uint8_t isEtherCATDevice(void)
{
    volatile uint32_t temp;

    temp = *((uint32_t *)(SOC_CONTROL_REGS + CONTROL_DEV_FEATURE));

    if((temp & 0x01) && (temp & (1 << 16)) && (temp & (1 << 17)))
    {
        return 1;
    }

    return 0;
}



int16_t getARMInterruptOffset()
{
    return ARM_INTERRUPT_OFFSET;
}

uint32_t getSpinlockClkCtrlOffset()
{
    return (uint32_t)(SOC_CM_PER_REGS +
                      CM_PER_SPINLOCK_CLKCTRL);
}

uint32_t getSpinlockLockReg0Offset()
{
    return (uint32_t)(SOC_SPINLOCK_REGS + CSL_SPINLOCK_LOCK_REG(0));
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 *           \brief   This function resets the hardware
*////////////////////////////////////////////////////////////////////////////////////////
void bsp_soft_reset()
{

    HWREG(SOC_PRM_DEVICE_REGS + PRM_DEVICE_PRM_RSTCTRL) =
        PRM_DEVICE_PRM_RSTCTRL_RST_GLOBAL_WARM_SW;
    return;
}


void UTILsInitCpswIcssPorts(/*unsigned char swicthType*/)
{
    GPIOModuleEnable(SOC_GPIO_0_REGS);
    GPIOModuleEnable(SOC_GPIO_3_REGS);
    GPIODirModeSet(SOC_GPIO_3_REGS, 10, GPIO_DIR_OUTPUT);
    GPIODirModeSet(SOC_GPIO_3_REGS, 4, GPIO_DIR_OUTPUT);
    GPIODirModeSet(SOC_GPIO_0_REGS, 7, GPIO_DIR_OUTPUT);
    GPIOPinWrite(SOC_GPIO_3_REGS, 4, GPIO_PIN_HIGH);
    GPIOPinWrite(SOC_GPIO_3_REGS, 10, GPIO_PIN_LOW);
    GPIOPinWrite(SOC_GPIO_0_REGS, 7, GPIO_PIN_LOW);
}

/* TODO -GPIO settings need move to gpioLed.c */
void InitLedRunError()
{
    GPIOModuleEnable(SOC_GPIO_3_REGS);
    GPIODirModeSet(SOC_GPIO_3_REGS, 18, GPIO_DIR_OUTPUT);
    GPIODirModeSet(SOC_GPIO_3_REGS, 20, GPIO_DIR_OUTPUT);
    GPIOPinWrite(SOC_GPIO_3_REGS, 18, GPIO_PIN_LOW);
    GPIOPinWrite(SOC_GPIO_3_REGS, 20, GPIO_PIN_LOW);
}

void EnableBusSwitch()
{
    GPIOModuleEnable(SOC_GPIO_2_REGS);
    GPIODirModeSet(SOC_GPIO_2_REGS, 21, GPIO_DIR_OUTPUT);
    GPIOPinWrite(SOC_GPIO_2_REGS, 21, GPIO_PIN_LOW);
}

void bsp_soc_evm_init()
{

   // GPIO_init();

#ifdef TIESC_SPI_SLAVE_MODE
    GPIO_write((SPI_SLAVE_FIRMWARE_LOADED_PIN), GPIO_PIN_VAL_LOW);
#endif

    UTILsInitCpswIcssPorts();
    UART_printf("UTILsInitCpswIcssPorts\n");


#ifndef iceAMIC11x
    Board_phyReset(2);
    UART_printf("Board_phyReset\n");
#endif

    pruIcss1Handle = PRUICSS_create(pruss_config, PRUICSS_INSTANCE);
    PRUICSS_pinMuxConfig(pruIcss1Handle, 0x0);   // PRUSS pinmuxing
    //Disable PRUs - This is to ensure PRUs are not running when application is not initialized
    PRUICSS_pruDisable(pruIcss1Handle, 0);
    PRUICSS_pruDisable(pruIcss1Handle, 1);
    UART_printf("PRUICSS_pinMuxConfig and PRUICSS_pruDisable \n");

#ifdef iceAMIC11x
    /* clear LED_RUN and LED_ERROR */
    InitLedRunError();

#else
    /* I2C Init */
   // Board_i2cLedInit();


#endif
#ifndef iceAMIC11x
    /* Oled Init */
    const char oled_line1[] = OLED_DISPLAY_VERSION_STRING;
    const char oled_line2[] = "EtherCAT App";
   // Board_oledInit();

    clear();
    setline(0);
    setOrientation(1);
    printstr((int8_t *)oled_line1);
    setline(1);
    setOrientation(1);
    printstr((int8_t *)oled_line2);
    scrollDisplayRight();


    /* Rotary Switch Init */
    uint8_t switch_state = 0;
    Board_initRotarySwitch();
    Board_readRotarySwitch(&switch_state);
    UART_printf("Rotary switch state : %d\n", switch_state);
#endif

    /* HVS Init */
    SPI_init();
    SPI_Params spiParams;                /* SPI params structure */
    SPI_Params_init(&spiParams);
    spiParams.frameFormat  = SPI_POL1_PHA0;
    handle = SPI_open(0, &spiParams);

#ifdef iceAMIC11x
    /* enable the Bus Switch for signals to pass on the header J5 and J4 */
    EnableBusSwitch();
#endif
    /* Always call Flash Init for AM335x after HVS Init as HVS init resets the SPI settings */
    /* Flash Init */
    flashHandle = Board_flashOpen((uint32_t) NULL, (uint32_t) NULL, NULL);

}

void display_esc_version(uint16_t revision, uint16_t build)
{
#ifndef iceAMIC11x
    const char oled_line1[] = OLED_DISPLAY_VERSION_STRING;
    char oled_line2[26];

    sprintf(oled_line2, "Rev %04X | Bld %04X", revision, build);
#endif
#ifndef DISABLE_UART_PRINT
    UART_printf("\n\rRevision/Type : x%04X ", revision);
    UART_printf(" Build : x%04X", build);
    UART_printf("\n\rFirmware Version : %d.%d.%d\n\r", (revision >> 8), (build >> 8), (build & 0xFF));
#endif
#ifndef iceAMIC11x
    clear();
    setline(0);
    setOrientation(1);
    printstr((int8_t *)oled_line1);
    setline(1);
    setOrientation(1);
    printstr((int8_t *)oled_line2);
    scrollDisplayLeft();
#endif
}
