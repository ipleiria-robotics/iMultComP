/*
 * @file    AM335x_bsp.c
 * @brief  This file contains the hardware abstraction functions for the
 *         AM335x PRU-ICSS Profibus implementation
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
#include <string.h>
#include "bsp.h"                                         /* this modules interface            */
#include <ti/starterware/include/hw/soc_am335x.h>
#include <ti/starterware/include/hw/hw_control_am335x.h>
#include <ti/starterware/include/edma.h>
#include <ti/csl/src/ip/icss/V0/cslr_icssm_iep.h>
#include <ti/csl/src/ip/icss/V0/cslr_icssm_intc.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_v1.h>

#include <ti/csl/src/ip/gpio/V1/gpio_v2.h>

#include "profi_soc.h"
#include "profi_misc.h"
#include <profi_edma.h>

#include <AM335x/Board/board_i2cLed.h>

#include <ti/drv/pruss/soc/pruicss_v1.h>

extern PRUICSS_Handle pruIcssHandle;

/******************************************************************************************
 *  name        : profi_soc_bsp_init
 *  description : SOC/Board specific configuration of Shared structure
 *
 ******************************************************************************************/
void profi_soc_bsp_init()
{
    g_pL2GlobVar = (TL2GlobVar *)(((PRUICSS_HwAttrs *)(
                                           pruIcssHandle->hwAttrs))->baseAddr +
                                      PRU_ICSS_SHARED_RAM);       /* Pointing g_pL2GlobVarto Shared RAM */

    memset(g_pL2GlobVar, 0, sizeof(TL2GlobVar));
    
    g_pL2GlobVar->PruTxGpioNum =
        5; /*pr1_pru0_pru_r30[5] is used as TX Enable GPIO for PROFIBUS*/
    g_pL2GlobVar->sel_GPO_GPIO = 0 ; /*PRUSS GPO for TX Enable*/

    g_pL2GlobVar->gpio_address =  0x0; /*Not used as PRUSS GPO is used for TX Enable*/
    g_pL2GlobVar->L3_mem_start_addr = SOC_PRU_ICSS_SHARED_RAM;
    g_pL2GlobVar->pru_base_addr = SOC_PRU_ICSS_DATA_RAM0;
    g_pL2GlobVar->edma_cc_base_addr = SOC_EDMA30CC_0_REGS;
    
    /*Configure the L3_mem_start_addr in DRAM1. Firmware picks the address from here
    Configure PRUSS Local memory address for faster access*/
    HWREG(SOC_PRU_ICSS_DATA_RAM1 + DRAM_L3BASEADDRESS_OFFSET) = PRU_ICSS_SHARED_RAM;
}


/****************************************************************************
 *  name        : Any SOC/Board specific initializations
 *  description :
 *                TX and RX pin muxing
 *                TX enable pin muxing
 *
 *
 *
 ***************************************************************************/
void profi_soc_hw_init(void)
{
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_FSR) = 0x00000005;
    HWREG(SOC_CONTROL_REGS + 0x980) = 0x0000002D;
    HWREG(SOC_CONTROL_REGS + 0x984) = 0x00000005;

#ifdef iceAMIC11x
    /*Bus Enable GPIO for AMIC110ICE*/
    GPIOModuleEnable(SOC_GPIO_2_REGS);
    GPIODirModeSet(SOC_GPIO_2_REGS, 21, GPIO_DIR_OUTPUT);
    GPIOPinWrite(SOC_GPIO_2_REGS, 21, GPIO_PIN_LOW);
#else
    Board_i2cLedInit();
#endif
}

void profi_soc_set_digoutput(uint8_t outData)
{
#ifndef iceAMIC11x
    Board_setDigOutput(outData);
#endif
}

/******************************************************************************************
 *  name        : profi_soc_register_interrupts
 *  description : This function registers PRUSS host interrupt 2 (SYS_INT_PRUSS1_EVTOUT0)
 *                in ARM INTC
 ******************************************************************************************/
void profi_soc_register_interrupts(void * irqHandler)
{                                              // 2
    PRUICSS_registerIrqHandler(pruIcssHandle, PRU_EVTOUT0, 20,
                               1, 1, (PRUICSSDRV_IRQ_HANDLER)irqHandler);
}


void profi_soc_edma_init(void)
{
    /*Configure Shadow region 2 for PRU Channel*/
    EDMAShdwRegionChEnable(SOC_EDMA30CC_0_REGS,PRU_EDMA_REGION,EDMA_CH_TYPE_DMA,PRU_EDMA_CHANNEL);
    /*Channel to param mapping for PRU Channel*/
    EDMAChToParamSetMap(SOC_EDMA30CC_0_REGS,EDMA_CH_TYPE_DMA,PRU_EDMA_CHANNEL,PRU_EDMA_CHANNEL);
}


void profi_soc_watchdog_init(uint32_t expiryDuration)
{
    /*Configure timer*/
    HWREG(((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->prussIepRegBase + CSL_ICSSM_IEP_PDI_WD_TIM) = expiryDuration *10; /*100 micro sec tik*/
    /*Enable the  timer*/
    HWREG(((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->prussIepRegBase + CSL_ICSSM_IEP_WD_CTRL)   = (1<<CSL_ICSSM_IEP_WD_STATUS_PDI_WD_STAT_SHIFT);
}

void profi_soc_watchdog_reset(uint32_t expiryDuration)
{
    /*Clear the Watchdog event from the INTC*/
    HWREGH(((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->prussIntcRegBase + CSL_ICSSM_INTC_SECR0) = 1<<10; /*PDI WD event is 10*/
    /*Reconfigure the timer*/
    HWREGH(((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->prussIepRegBase + CSL_ICSSM_IEP_PDI_WD_TIM) = expiryDuration *10;
}
