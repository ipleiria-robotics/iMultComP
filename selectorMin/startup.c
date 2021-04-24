/*
* startup.c
*
* PRU-ICSS related startup functions
*
* Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
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

// local includes
#include "startup.h"
#include "hw/hw_types.h"
#include "mcspi.h"
#include "hw/soc_AM335x.h"
#include "armv7a/am335x/evmAM335x.h"
#include "hw/hw_cm_per.h"
#include "armv7a/am335x/interrupt.h"
#include "hsi2c.h"
#include "mux.h"
#include "osdrv_pruss.h"
#include "osdrv_prussdefs.h"
#include "pruss_intc_mapping.h"
#include "plat_mux.h"
#include "plat_utils.h"
#include "pruss_register.h"


#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/family/arm/a8/Mmu.h>
#include <string.h>

#include "protocol_detection_bin.h"


/////////
#define SPI_CH_HVS882			0
//////////
/* am335x specific mux bit defines */
#define AM335X_SLEWCTRL_FAST            (0 << 6)
#define AM335X_SLEWCTRL_SLOW            (1 << 6)
#define AM335X_INPUT_EN                 (1 << 5)
#define AM335X_PULL_UP                  (1 << 4)
/* bit 3: 0 - enable, 1 - disable for pull enable */
#define AM335X_PULL_DISA                (1 << 3)

/* Definition of output pin could have pull disabled, but
 * this has not been done due to two reasons
 * 1. AM335X_MUX will take care of it
 * 2. If pull was disabled for out macro, combining out & in pull on macros
 *    would disable pull resistor and AM335X_MUX cannot take care of the
 *    correct pull setting and unintentionally pull would get disabled
 */
#define AM335X_PIN_OUTPUT               (0)
#define AM335X_PIN_OUTPUT_PULLUP        (AM335X_PULL_UP)
#define AM335X_PIN_INPUT                (AM335X_INPUT_EN | AM335X_PULL_DISA)
#define AM335X_PIN_INPUT_PULLUP         (AM335X_INPUT_EN | AM335X_PULL_UP)
#define AM335X_PIN_INPUT_PULLDOWN       (AM335X_INPUT_EN)


/************************************************************
* Local Macro Declarations                                  *
************************************************************/


/*============================================================================*/
/*                             GLOBAL VARIABLES                               */
/*============================================================================*/
extern unsigned char boardType;

/************************************************************
* Global Function Definitions                               *
************************************************************/
//void PowerGPIO(void);
void PINMUX_Config(void);
void PRUSSDRVPruCfgInit(void);
void PRUSSDRVPruMiiRtCfgInit(void);


/************************************************************
* implementation                                            *
************************************************************/
/////////////////////////////////////////
void prussInit(void)
{
    tpruss_intc_initdata pruss_intc_initdata = PRUSS_PRU0_INTC_INITDATA;

    UTILsICSSInit();

    //PRUSSPinMuxConfig(0x01); // PRUSS pinmuxing

	// clear ICSS PRU0 data RAM
	memset((void *)0x4a300000, 0, (8*1024));
	// clear ICSS PRU0 data RAM
	memset((void *)0x4a302000, 0, (8*1024));
	// clear ICSS shared RAM
	memset((void *)0x4a310000, 0, (12*1024));

	/* Set C28 to PRU0/1 shared memory aka register base address  at 0x1.0000*/
	HWREG(PRUSS_BASE + PRUSS_PRU0_CTRL_CTPPR0) = (0x4A310000 & 0x000FFFFF) >> 8;
	HWREG(PRUSS_BASE + PRUSS_PRU1_CTRL_CTPPR0) = (0x4A310000 & 0x000FFFFF) >> 8;
	/* Set in constant table C30 to shared RAM 0x40300000 */
	HWREG(PRUSS_BASE + PRUSS_PRU0_CTRL_CTPPR1) = (0x40300000 & 0x00FFFF00) >> 8;
	HWREG(PRUSS_BASE + PRUSS_PRU1_CTRL_CTPPR1) = (0x40300000 & 0x00FFFF00) >> 8;

	/* configure MDIO */
	/* PORT 0 has PHY address 1, PORT 1 has PHY address 2 */
	HWREG(PRUSS_BASE + PRUSS_MDIO + PRUSS_MDIO_USERPHYSEL0) = 0x01;// | (1<<6);
	HWREG(PRUSS_BASE + PRUSS_MDIO + PRUSS_MDIOCNTROL) = 0x40000000 | 0x9F; //(HWREG(PRUSS_BASE + PRUSS_MDIO + PRUSS_MDIOCNTROL) & 0xffff);//mdio_conf->clkdiv; //200/(clkdiv+1)

	/* configure PRUSS register CFG */
	PRUSSDRVPruCfgInit();

	/* configure PRUSS register MII_RT */
	PRUSSDRVPruMiiRtCfgInit();

	/* initialize PRUSS INTC with configured profile*/
	PRUSSDRVPruIntcInit(&pruss_intc_initdata);

    /* load PRU firmware into instruction RAM */
	//PRUSSDRVSetPRUBuffer(0, (Uint32*)EXAMPLEPRU0, sizeof(EXAMPLEPRU0));
	PRUSSDRVSetPRUBuffer(0, (Uint32*)detection_firmware_0, sizeof(detection_firmware_0));

    /* start PRU execution */
	PRUSSDRVPruExecProgram(0, "");
}

void PRUSSDRVPruCfgInit(void)
{
	if(AM335X_BOARD_TYPE_ICE == boardType)
	       {
	             HWREG(PRUSS_BASE + PRUSS_CFG_PIN_MX) = 0 | \
	                             (1 << PRUSS_PIN_MUX_SEL0_SHIFT);
	       } else if(AM335X_BOARD_TYPE_ICE_V2 == boardType)
	       {
	             HWREG(PRUSS_BASE + PRUSS_CFG_PIN_MX) = 0 | \
	                             (0 << PRUSS_PIN_MUX_SEL0_SHIFT);
	       }

	   // Enabling the master interface of ICSS
	  HWREG(PRUSS_BASE + PRUSS_CFG_SYSCFG) = 0  | 	\
			  (PRUSS_SYSCFG_STANDBY_INIT_ENABLE << PRUSS_SYSCFG_STANDBY_INIT_SHIFT) |  \
			  (PRUSS_SYSCFG_STANDBY_MODE_SMART << PRUSS_SYSCFG_STANDBY_MODE_SHIFT)  | \
			  (PRUSS_SYSCFG_IDLE_MODE_SMART << PRUSS_SYSCFG_IDLE_MODE_SHIFT);

	  HWREG(PRUSS_BASE + PRUSS_CFG_GPCFG0) = 0 | \
			  (PRUSS_GPCFG0_PRU0_GPI_MODE_MIIMODE << PRUSS_GPCFG0_PRU0_GPI_MODE_SHIFT);

	  HWREG(PRUSS_BASE + PRUSS_CFG_GPCFG1) = 0 | \
			  (PRUSS_GPCFG1_PRU1_GPI_MODE_MIIMODE << PRUSS_GPCFG1_PRU1_GPI_MODE_SHIFT);

	  HWREG(PRUSS_BASE + PRUSS_CFG_MII_RT) = 0 | \
			  (PRUSS_MII_RT_EVENT_EN_ENABLE << PRUSS_MII_RT_EVENT_EN_SHIFT);

#if 0
	  volatile unsigned int temp32;
      // PHY1 configure FD in parallel detect mode
      while(HWREG(PRUSS_BASE + PRUSS_MDIO + PRUSS_MDIOUSERACCESS0) & (1<<31));
      HWREG(PRUSS_BASE + PRUSS_MDIO + PRUSS_MDIOUSERACCESS0) = 0x00 |\
                   (1<<31) |\
                   (0<<30) |\
                   (0<<29) |\
                   (0xA<<21) |\
                   ((/*mdio_conf->addr0*/1)<<16) |\
                   (0x0<<0);
      while((temp32 = HWREG(PRUSS_BASE + PRUSS_MDIO + PRUSS_MDIOUSERACCESS0)) & (1<<31));
      temp32 |= 1<<5;     // set Force Full-Duplex while working with link partner in forced 100B-TX
      temp32 &= 0xFFFF;
      HWREG(PRUSS_BASE + PRUSS_MDIO + PRUSS_MDIOUSERACCESS0) = 0x00 |\
                   (1<<31) |\
                   (1<<30) |\
                   (0<<29) |\
                   (0xA<<21) |\
                   ((/*mdio_conf->addr0*/1)<<16) | (temp32<<0);

      // PHY2 configure FD in parallel detect mode
      while(HWREG(PRUSS_BASE + PRUSS_MDIO + PRUSS_MDIOUSERACCESS1) & (1<<31));
      HWREG(PRUSS_BASE + PRUSS_MDIO + PRUSS_MDIOUSERACCESS1) = 0x00 |\
                   (1<<31) |\
                   (0<<30) |\
                   (0<<29) |\
                   (0xA<<21) |\
                   ((/*mdio_conf->addr1*/2)<<16) |\
                   (0x0<<0);
      while((temp32 = HWREG(PRUSS_BASE + PRUSS_MDIO + PRUSS_MDIOUSERACCESS1)) & (1<<31));
      temp32 |= 1<<5;     // set Force Full-Duplex while working with link partner in forced 100B-TX
      temp32 &= 0xFFFF;
      HWREG(PRUSS_BASE + PRUSS_MDIO + PRUSS_MDIOUSERACCESS1) = 0x00 |\
                   (1<<31) |\
                   (1<<30) |\
                   (0<<29) |\
                   (0xA<<21) |\
                   ((/*mdio_conf->addr1*/2)<<16) |\
                   (temp32<<0);
#endif

	 // HWREG(PRUSS_BASE + PRUSS_CFG_PIN_MX) = 0 | \
			  (1 << PRUSS_PIN_MUX_SEL0_SHIFT);
}

void PRUSSDRVPruMiiRtCfgInit(void)
{
	/* Configuration of Port 0 */

	// no byte swap, no RX_CUT_PREAMBLE, MII data from port 0, no RX_L2_ENABLE
	HWREG(PRUSS_BASE + PRUSS_MII_RT_CFG_RXCFG0) = 0 | \
			(PRUSS_RXCFG0_RX_AUTO_FWD_PRE_ENABLE << PRUSS_RXCFG0_RX_AUTO_FWD_PRE_SHIFT ) | \
			(PRUSS_RXCFG0_RX_MUX_SELECT_PORT0 << PRUSS_RXCFG0_RX_MUX_SELECT_SHIFT) | \
			(PRUSS_RXCFG0_RX_L2_ENABLE << PRUSS_RXCFG0_RX_L2_SHIFT) | \
			(PRUSS_RXCFG0_RX_CUT_PREAMBLE_DISABLE << PRUSS_RXCFG0_RX_CUT_PREAMBLE_SHIFT) | \
			(PRUSS_RXCFG0_RX_ENABLE << PRUSS_RXCFG0_RX_ENABLE_SHIFT);

	/* Enable the output FIFO and set a delay of 0x40 in nibbles, route TX back to Port0 */
	HWREG(PRUSS_BASE + PRUSS_MII_RT_CFG_TXCFG0) = 0 | \
			(0x08 << PRUSS_TXCFG0_TX_START_DELAY_SHIFT) | \
			(PRUSS_TXCFG0_TX_AUTO_SEQUENCE_ENABLE << PRUSS_TXCFG0_TX_AUTO_SEQUENCE_SHIFT)	| \
			(PRUSS_TXCFG0_TX_MUX_SEL_PORT0 << PRUSS_TXCFG0_TX_MUX_SEL_SHIFT) | \
			(PRUSS_TXCFG0_TX_AUTO_PREAMBLE_DISABLE << PRUSS_TXCFG0_TX_AUTO_PREAMBLE_SHIFT) | \
			(/*PRUSS_TXCFG0_TX_ENABLE*/0 << PRUSS_TXCFG0_TX_ENABLE_SHIFT);

	/* Configuration of Port 1 */

	HWREG(PRUSS_BASE + PRUSS_MII_RT_CFG_RXCFG1) = 0 | \
			(PRUSS_RXCFG1_RX_AUTO_FWD_PRE_ENABLE << PRUSS_RXCFG1_RX_AUTO_FWD_PRE_SHIFT ) | \
			(PRUSS_RXCFG1_RX_L2_ENABLE << PRUSS_RXCFG1_RX_L2_SHIFT) | \
			(PRUSS_RXCFG1_RX_MUX_SELECT_PORT1 << PRUSS_RXCFG1_RX_MUX_SELECT_SHIFT) | \
			(PRUSS_RXCFG1_RX_CUT_PREAMBLE_DISABLE << PRUSS_RXCFG1_RX_CUT_PREAMBLE_SHIFT) | \
			(PRUSS_RXCFG1_RX_ENABLE << PRUSS_RXCFG1_RX_ENABLE_SHIFT);

	/* Enable the output FIFO and set a delay of 0x40 in nibbles, route TX back to Port0 */
	HWREG(PRUSS_BASE + PRUSS_MII_RT_CFG_TXCFG1) = 0 | \
			(0x08 << PRUSS_TXCFG1_TX_START_DELAY_SHIFT) | \
			(PRUSS_TXCFG1_TX_AUTO_SEQUENCE_ENABLE << PRUSS_TXCFG0_TX_AUTO_SEQUENCE_SHIFT)	| \
			(PRUSS_TXCFG1_TX_MUX_SEL_PORT1 << PRUSS_TXCFG1_TX_MUX_SEL_SHIFT) | \
			(PRUSS_TXCFG1_TX_AUTO_PREAMBLE_DISABLE << PRUSS_TXCFG1_TX_AUTO_PREAMBLE_SHIFT) | \
			(PRUSS_TXCFG1_TX_ENABLE << PRUSS_TXCFG1_TX_ENABLE_SHIFT);
}




