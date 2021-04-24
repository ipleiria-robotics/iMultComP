/* Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
*
*    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifndef ICEV2_BOARD_H_
#define ICEV2_BOARD_H_

#include "soc_AM335x.h"
#include <gpio_v2.h>

#define GPIO0 (0x00000001)
#define GPIO1 (0x00000002)
#define GPIO2 (0x00000004)
#define GPIO3 (0x00000008)
#define GPIO4 (0x00000010)
#define GPIO5 (0x00000020)
#define GPIO6 (0x00000040)
#define GPIO7 (0x00000080)
#define GPIO8 (0x00000100)
#define GPIO9 (0x00000200)
#define GPIO10 (0x00000400)
#define GPIO11 (0x00000800)
#define GPIO12 (0x00001000)
#define GPIO13 (0x00002000)
#define GPIO14 (0x00004000)
#define GPIO15 (0x00008000)
#define GPIO16 (0x00010000)
#define GPIO17 (0x00020000)
#define GPIO18 (0x00040000)
#define GPIO19 (0x00080000)
#define GPIO20 (0x00100000)
#define GPIO21 (0x00200000)
#define GPIO22 (0x00400000)
#define GPIO23 (0x00800000)
#define GPIO24 (0x01000000)
#define GPIO25 (0x02000000)
#define GPIO26 (0x04000000)
#define GPIO27 (0x08000000)
#define GPIO28 (0x10000000)
#define GPIO29 (0x20000000)
#define GPIO30 (0x40000000)
#define GPIO31 (0x80000000)

#define LED_D1_GREEN_0 GPIO16		// bank0
#define LED_D1_RED_0 GPIO17			// bank0
#define LED_D2_ORANGE_0 GPIO19		// bank0
#define LED_D2_GREEN_0 GPIO20		// bank0
#define LED_D2_RED_1 GPIO30			// bank1
#define LED_D1_ORANGE_3 GPIO9		// bank3

#define SET_LED_D1_GREEN (HWREG(SOC_GPIO_0_REGS + GPIO_SETDATAOUT) = LED_D1_GREEN_0)
#define SET_LED_D1_RED (HWREG(SOC_GPIO_0_REGS + GPIO_SETDATAOUT) = LED_D1_RED_0)
#define SET_LED_D2_ORANGE (HWREG(SOC_GPIO_0_REGS + GPIO_SETDATAOUT) = LED_D2_ORANGE_0)
#define SET_LED_D2_GREEN (HWREG(SOC_GPIO_0_REGS + GPIO_SETDATAOUT) = LED_D2_GREEN_0)
#define SET_LED_D2_RED (HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = LED_D2_RED_1)
#define SET_LED_D1_ORANGE (HWREG(SOC_GPIO_3_REGS + GPIO_SETDATAOUT) = LED_D1_ORANGE_3)

#define CLEAR_LED_D1_GREEN (HWREG(SOC_GPIO_0_REGS + GPIO_CLEARDATAOUT) = LED_D1_GREEN_0)
#define CLEAR_LED_D1_RED (HWREG(SOC_GPIO_0_REGS + GPIO_CLEARDATAOUT) = LED_D1_RED_0)
#define CLEAR_LED_D2_ORANGE (HWREG(SOC_GPIO_0_REGS + GPIO_CLEARDATAOUT) = LED_D2_ORANGE_0)
#define CLEAR_LED_D2_GREEN (HWREG(SOC_GPIO_0_REGS + GPIO_CLEARDATAOUT) = LED_D2_GREEN_0)
#define CLEAR_LED_D2_RED (HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = LED_D2_RED_1)
#define CLEAR_LED_D1_ORANGE (HWREG(SOC_GPIO_3_REGS + GPIO_CLEARDATAOUT) = LED_D1_ORANGE_3)

#endif /* ICEV2_BOARD_H_ */
