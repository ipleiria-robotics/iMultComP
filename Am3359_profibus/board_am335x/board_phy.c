/**
 *  \file   board_phy.c
 *
 *  \brief  AM335x ICE Board specific phy parameters.
 *
 *   This file contains the phy hardware parameters specific to board.
 */

/*
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <xdc/std.h>
#include <examples/board/include/board_phy.h>
#include <ti/starterware/include/hw/soc_am335x.h>
#include <ti/csl/soc.h>
#include <ti/csl/src/ip/icss/V1/cslr_icss_cfg.h>
#include <ti/csl/csl_gpio.h>

#include <ti/drv/gpio/soc/GPIO_soc.h>
#include <examples/board/include/board_misc.h>
#include <examples/board/include/delay_us.h>

#define AM335X_ICSS1_PORT1_PHY_ADDR 1
#ifndef iceAMIC11x
#define AM335X_ICSS1_PORT2_PHY_ADDR 3
#else
#define AM335X_ICSS1_PORT2_PHY_ADDR 13
#endif

int8_t Board_getPhyAddress(uint8_t instance, uint8_t portNumber)
{
    if(PRUICSS1_PHY_ADDRESS == instance)
    {
        if(1u == portNumber)
        {
            return AM335X_ICSS1_PORT1_PHY_ADDR;
        }

        else if(2u == portNumber)
        {
            return AM335X_ICSS1_PORT2_PHY_ADDR;
        }
    }

    return -1;
}


typedef struct GPIO_INFO
{
    uint32_t pin;
    uint32_t baseAddr;

} GPIO_INFO;

#ifndef iceAMIC11x
GPIO_INFO PhyResetInfo[2];
#else
GPIO_INFO PhyResetInfo[6];
#endif

void Board_phyManualReset(uint8_t numPorts)
{
#ifndef iceAMIC11x
    GPIOModuleEnable(PhyResetInfo[0].baseAddr);
    GPIODirModeSet(PhyResetInfo[0].baseAddr, PhyResetInfo[0].pin, GPIO_DIR_OUTPUT);

    GPIOPinWrite(PhyResetInfo[0].baseAddr, PhyResetInfo[0].pin, GPIO_PIN_LOW);
    Board_delay(1000);

    GPIOPinWrite(PhyResetInfo[0].baseAddr, PhyResetInfo[0].pin, GPIO_PIN_HIGH);
    Board_delay(100000);
#else

    /* drive BootConfig_Disable to high */
    GPIOModuleEnable(PhyResetInfo[0].baseAddr);
    GPIODirModeSet(PhyResetInfo[0].baseAddr, PhyResetInfo[0].pin, GPIO_DIR_OUTPUT);
    GPIOPinWrite(PhyResetInfo[0].baseAddr, PhyResetInfo[0].pin, GPIO_PIN_HIGH);

    delay_us(1000);

    /* drive phy1 (& phy2) reset to high; Both PHY resets are sourced from one GPIO */
    GPIOModuleEnable(PhyResetInfo[1].baseAddr);
    GPIODirModeSet(PhyResetInfo[1].baseAddr, PhyResetInfo[1].pin, GPIO_DIR_OUTPUT);
    GPIOPinWrite(PhyResetInfo[1].baseAddr, PhyResetInfo[1].pin, GPIO_PIN_HIGH);
    delay_us(20);
    GPIOPinWrite(PhyResetInfo[1].baseAddr, PhyResetInfo[1].pin, GPIO_PIN_LOW);
    /* T1 - RESET pulse width, min. 10us, see DP83822 data sheet */
    delay_us(20);
    GPIOPinWrite(PhyResetInfo[1].baseAddr, PhyResetInfo[1].pin, GPIO_PIN_HIGH);
#endif
}

/* ========================================================================== */
/*                          Function Definitions                             */
/* ========================================================================== */

int32_t Board_phyGPIOsetup(uint8_t numPorts)
{
    if((numPorts != 4) && (numPorts != 2))
    {
        return -1;
    }

#ifndef iceAMIC11x
    /*PRUETH0_RESETn */
    PhyResetInfo[0].pin = 5;
    PhyResetInfo[0].baseAddr = SOC_GPIO_2_REGS;
#else

    /* BootConfigMux (BootConfig_Disable) - drive high */
    PhyResetInfo[0].pin = 19;
    PhyResetInfo[0].baseAddr = SOC_GPIO_2_REGS;

    /* phy1 reset - drive high */
    PhyResetInfo[1].pin = 13;
    PhyResetInfo[1].baseAddr = SOC_GPIO_1_REGS;

    /* phy 2 reset - drive high */
    PhyResetInfo[2].pin = 27;
    PhyResetInfo[2].baseAddr = SOC_GPIO_0_REGS;

    /* AM_PHY1_LED1 - input */
    PhyResetInfo[3].pin = 8;
    PhyResetInfo[3].baseAddr = SOC_GPIO_0_REGS;

    /* AM_PHY1_RXCK - input */
    PhyResetInfo[4].pin = 10;
    PhyResetInfo[4].baseAddr = SOC_GPIO_0_REGS;

    /* BootConfigFET - drive high */
    PhyResetInfo[5].pin = 18;
    PhyResetInfo[5].baseAddr = SOC_GPIO_2_REGS;
#endif

    return (0);
}


int32_t Board_phyReset(uint8_t numPorts)
{
    int32_t retval = 0;
    /*Configure the GPIO pins */
    retval = Board_phyGPIOsetup(numPorts);
    /*Reset the phys */
    Board_phyManualReset(numPorts);
    return retval;
}

#ifdef iceAMIC11x
void Board_phySetGPIORxclkLedInput()
{
    /* set AM_PHY1_RXCLK and AM_PHY1_LED1 as inputs */
    GPIOModuleEnable(PhyResetInfo[3].baseAddr);
    GPIODirModeSet(PhyResetInfo[3].baseAddr, PhyResetInfo[3].pin, GPIO_DIR_INPUT);

    GPIOModuleEnable(PhyResetInfo[4].baseAddr);
    GPIODirModeSet(PhyResetInfo[4].baseAddr, PhyResetInfo[4].pin, GPIO_DIR_INPUT);
}


void Board_phyEnableFETswitch()
{
    /* drive BootConfig_FET_EN high */
    GPIOModuleEnable(PhyResetInfo[5].baseAddr);
    GPIODirModeSet(PhyResetInfo[5].baseAddr, PhyResetInfo[5].pin, GPIO_DIR_OUTPUT);

    GPIOPinWrite(PhyResetInfo[5].baseAddr, PhyResetInfo[5].pin, GPIO_PIN_HIGH);
}
#endif

