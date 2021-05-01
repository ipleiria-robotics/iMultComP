/*
* sel_app.c
*
* main() application
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
#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <xdc/cfg/global.h>
#include "pruss_intc_mapping.h"
#include "startup.h"
#include "sel_app_cnfg.h"
#include "osdrv_uart.h"
#include "plat_led.h"
#include "osdrv_mmu.h"
#include "plat_utils.h"
#include "startup.h"
#include "plat_mux.h"
#include "plat_gpio.h"
#include "osdrv_pruss.h"
#include "pruss_register.h"
#include "hw/hw_types.h"
#include "protocol.h"
#include "osdrv_mcspi.h"
#include "soc_AM335x.h"
#include "plat_gpio.h"
#include "osdrv_oledlcd.h"
void ownReset(UArg arg0, UArg arg1);

//==========================================================================
//                               prototypes
//==========================================================================
void UsrIntHandle(void);
void irq_create(int irqNum, void * isrFunc);
void task_reset(UArg arg0, UArg arg1);
//==========================================================================
//                               global variable
//==========================================================================
volatile uint8_t g_reset = 0;
unsigned char boardType = 0;
volatile unsigned short *protocol_number = (volatile unsigned short*) ADDR_PROTOCOL_TYPE;
unsigned char uartInstance = 0;
unsigned long baseAddr;

#define PRU_IRQ 20			// PRUSS EVENT_0

/*
 *  ======== main ========
 */
int main()
{ 
    Task_Params taskParams;
    Task_Handle task;
    Error_Block eb;

    MMUInit(applMmuEntries);

    UTILsTimer2Init();
    boardType = UTILsGetBoardType();
    //UTILsSetBoardType(boardType);

   if(AM335X_BOARD_TYPE_IDK == boardType)
   {
	   PinMuxConfig(idkMux);
	   /* For Input reading - HVS*/
	   //McSPIInit(MCSPI_INSTANCE_1, 12, 0 ,MCSPI_SINGLE_CH,MCSPI_CHANNEL_1);
   }
   else if( AM335X_BOARD_TYPE_ICE_V2 == boardType)
   {
	   PinMuxConfig(iceMux);
	   PinMuxConfig(ice_v2Mux);
	   McSPIInit(MCSPI_INSTANCE_0, 12, 0 ,MCSPI_SINGLE_CH,MCSPI_CHANNEL_1);

	   /* For Input reading - HVS*/
	   //McSPIInit(MCSPI_INSTANCE_0, 12, 0 ,MCSPI_SINGLE_CH,MCSPI_CHANNEL_1);
   }
   else
   {
	   PinMuxConfig(iceMux);
	   PinMuxConfig(ice_v1Mux);

	   /* For Input reading - HVS*/
	   //McSPIInit(MCSPI_INSTANCE_1, 12, 1 ,MCSPI_SINGLE_CH,MCSPI_CHANNEL_0);
   }

    GPIOInit();
	LEDInit();

    if( AM335X_BOARD_TYPE_ICE == boardType )
    {
    	uartInstance = 5;
    	baseAddr = SOC_UART_5_REGS;
    }else if((AM335X_BOARD_TYPE_IDK == boardType) || (AM335X_BOARD_TYPE_ICE_V2 == boardType))
    {
    	uartInstance = 3;
    	baseAddr = SOC_UART_3_REGS;
    }



	UartOpen(uartInstance,NULL);
	prussInit();
	if (AM335X_BOARD_TYPE_ICE_V2 == boardType)
	    {
	        PRUSSPinMuxConfig(0x0); // PRUSS pinmuxing
	        UTILsInitCpswIcssPorts(ICSS_SWITCH);   // calling UTILsResetPhys
	    }
	    else
	    {
	        PRUSSPinMuxConfig(0x1); // PRUSS pinmuxing
	        UTILsResetPhys();
	    }


	irq_create(PRU_IRQ, (void *)UsrIntHandle);

    Error_init(&eb);
    Task_Params_init(&taskParams);
    taskParams.priority = 1;
    task = Task_create(task_reset, &taskParams, &eb);	// this task controls reset
    if (task == NULL) {
        System_printf("Task_create() failed!\n");
        BIOS_exit(0);
    }

    if(AM335X_BOARD_TYPE_ICE_V2 == boardType)
    {
    	OLEDInit();
    	OLEDDeactivateScroll();
    	OLEDShowString(1,(unsigned char *)"Protocol",0,0);
    	OLEDShowString(1,(unsigned char *)"Detection Firmware",1,0);
    	OLEDContinuousScroll(0x01,0x01,Max_Column,0x00,0x00,0x01,0x06,0x01);
    }

	UARTPutString(5, "PRUSS Packet Processing Demo Application Started\n\r");
    BIOS_start();
    return 0;
}

/*
 * this runs just after reset before any BIOS stuff...
 * so here is the right place to enable our hardware.
 * But we can't use any Bios functions yet!
 *
 */

void ownReset(UArg arg0, UArg arg1)
{
    GPIOClockEnable(GPIO_INSTANCE_ALL);
	HWREG(SOC_GPIO_1_REGS + GPIO_OE)= 0xFFFF8F00;// GPIO1_0-7/12/13/14 pins Enable
	HWREG(SOC_GPIO_0_REGS + GPIO_OE)= 0xF33FFFFF;// GPIO0_22,23,26,27 pins Enable

    // enable dmTimer 5 (Bios Timer3!) - used for RGB Panel timing
	HWREG( SOC_PRCM_REGS + CM_PER_TIMER5_CLKCTRL) |= 0x2;
}

void UsrIntHandle(void)
{
	HWREG(PRUSS_BASE + PRUSS_INTC + INTC_SECR0) = 0 | (1 << /*WATCHDOG1_EVENT*/ PRU_ARM_EVENT0); /* clear event in PRUSS INTC */
	UNLOCK_RTC_SS();
	switch(*protocol_number) {
		case PROTOCOL_POWERLINK:
			HWREG(RTC_SCRATCH0_REG) = POWERLINK;
			UARTPutString(5, "PRUSS Packet Processing Demo Application StartPW\n\r");
			g_reset = 1;

			break;
		case PROTOCOL_PROFINET:
			HWREG(RTC_SCRATCH0_REG) = PROFINET;
			UARTPutString(5, "PRUSS Packet Processing Demo Application StartPP\n\r");
			g_reset = 1;
			break;
		case PROTOCOL_ETHERCAT:
			HWREG(RTC_SCRATCH0_REG) = ETHERCAT;
			UARTPutString(5, "PRUSS Packet Processing Demo Application StartPE\n\r");
			g_reset = 1;
			break;
		case PROTOCOL_ENIP:
			HWREG(RTC_SCRATCH0_REG) = ETHERNETIP;
			UARTPutString(5, "PRUSS Packet Processing Demo Application StartEP\n\r");
			g_reset = 1;
			break;
		case PROTOCOL_SERCOS3:
			HWREG(RTC_SCRATCH0_REG) = SERCOS3;
			UARTPutString(5, "PRUSS Packet Processing Demo Application StartPS\n\r");
			g_reset = 1;
			break;
		case LINK_STATUS_DOWN:
			break;
		case LINK_STATUS_UP:
			break;
		default:
			break;
	}
	LOCK_RTC_SS();
 }


void irq_create(int irqNum, void * isrFunc)
{
	switch(irqNum)
	{
		case PRU_IRQ:
			PRUSSDRVRegisterIrqHandler(20, 1, (PRUSSDRV_IRQ_HANDLER)isrFunc);
			break;
	}
}

void task_reset(UArg arg0, UArg arg1) {

	while(1) {
		if(g_reset) {
			HWREG(0x44E00F00) = 0x2;
		}
		Task_sleep(100);
	}
}
