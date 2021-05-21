/**
 *  \file   board_misc.c
 *
 *  \brief  Board specific misc APIs.
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


#include <Include/board/board_misc.h>
#ifdef SOC_AM571x
#include <ti/csl/soc/am571x/src/cslr_interrupt.h>
#include <ti/csl/soc/am571x/src/cslr_control_intr_dma.h>
#include <ti/csl/soc/am571x/src/cslr_l4per_cm_core_component.h>
#include <ti/csl/soc/am571x/src/cslr_soc_mpu_baseaddress.h>
#include <ti/csl/soc/am571x/src/cslr_control_core.h>
#endif

#include <ti/drv/i2c/I2C.h>
#include <ti/drv/spi/SPI.h>
#include <Include/board/board_spi.h>
#include <Include/board/board_rotary_switch.h>

#ifdef SOC_AM572x
#include <ti/csl/soc/am572x/src/cslr_control_intr_dma.h>
#include <ti/csl/soc/am572x/src/cslr_l4per_cm_core_component.h>
#include <ti/csl/soc/am572x/src/cslr_soc_mpu_baseaddress.h>
#endif
#include <ti/csl/hw_types.h>
#ifdef SOC_AM335x
#include <Include/board/board_mcspi.h>
#endif
#ifdef SOC_AM65XX
#include <protocols/common/include/debugtrace.h>
#include <ti/sysbios/family/arm/v8a/Mmu.h>
#include <examples/board/include/soc_icss_header.h>
#endif

volatile uint32_t ctrlCoreIO;

#define CONTROL_CORE_IO 0x4A002558

#define PRCM_MODULEMODE_ENABLE           (2U)
#define PRCM_MODULEMODE_MASK             (3U)
#define PRCM_IDLE_ST_MASK                (0x00030000U)
#define PRCM_IDLE_ST_SHIFT               (16U)

#define PRCM_MODULE_IDLEST_FUNC          (0U)

/*
 *  ======== CSL_xbarMpuIrqConfigure ========
 */
#if defined (SOC_AM572x) || defined (SOC_AM571x)
void Board_xbarMpuIrqConfigure(Uint32 irqNumIdx, CSL_XbarIrq xbarIrq)
{
    CSL_control_intr_dmaRegs *ctrlCoreIntrDmaReg =
        (CSL_control_intr_dmaRegs *) CSL_MPU_IRQ_DMARQ_CROSSBAR_REGISTERS_REGS;
    Uint32 regIdx = (irqNumIdx - 1U) / 2U;
    Uint32 regLsb = (((irqNumIdx - 1U) % 2U) * 16U);
    Uint32 regMsb = regLsb + 16U;

    if((CSL_XBAR_IRQ_MIN < xbarIrq) && (CSL_XBAR_IRQ_MAX > xbarIrq))
    {
        if((0U < irqNumIdx) && (CSL_MPU_IRQ_XBAR_COUNT >= irqNumIdx))
        {
            CSL_FINSR(ctrlCoreIntrDmaReg->MPU_IRQ[regIdx],
                      regMsb, regLsb, xbarIrq);
        }
    }
}
#endif
void Board_delay(uint32_t delay)
{
    volatile uint32_t delay1 = delay * 2;

    while(delay1--) ;
}

void Board_getDigInput(uint8_t *switchs)
{
#if !defined(SOC_K2G) && !defined(SOC_AM65XX) /* K2G ICE doesnt have the DigInputs over SPI instead using rotary switch*/
#if !defined(iceAMIC11x) /* AMIC11x ICE doesnt have the DigInputs over SPI */
    Board_readHVS(switchs);
#endif
#else
    Board_readRotarySwitch(switchs);
#endif
}

#if defined (SOC_AM572x) || defined (SOC_AM571x)
void Board_enablePWMSS1Module()
{


    /* Setting the Control Core IO2 to enable PWMSS1_TBCLKEN */
    ctrlCoreIO = HWREG(CONTROL_CORE_IO);

    ctrlCoreIO |= 0x100000;
    HWREG(CONTROL_CORE_IO) = ctrlCoreIO;
    /*Enable PWMSS1 module*/
    /* Enable the module */

    HW_WR_REG32(CSL_MPU_L4PER_CM_CORE_REGS +
                CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER2_PWMSS1_CLKCTRL_REG,
                PRCM_MODULEMODE_ENABLE);

    /* Check for module enable status */
    while(PRCM_MODULEMODE_ENABLE !=
            (HW_RD_REG32(CSL_MPU_L4PER_CM_CORE_REGS +
                         CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER2_PWMSS1_CLKCTRL_REG)
             & PRCM_MODULEMODE_MASK));

    /* Check clock activity - ungated */
    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER2_CLKSTCTRL_REG_CLKACTIVITY_L4PER2_L3_GICLK_MASK
            !=
            (HW_RD_REG32(CSL_MPU_L4PER_CM_CORE_REGS +
                         CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER2_CLKSTCTRL_REG)
             & CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER2_CLKSTCTRL_REG_CLKACTIVITY_L4PER2_L3_GICLK_MASK));

    /* Check idle status value - should be in functional state */
    while((PRCM_MODULE_IDLEST_FUNC << PRCM_IDLE_ST_SHIFT) !=
            (HW_RD_REG32(CSL_MPU_L4PER_CM_CORE_REGS +
                         CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER2_PWMSS1_CLKCTRL_REG)
             & PRCM_IDLE_ST_MASK));
}
#endif

#ifdef SOC_AM65XX
#if defined (__aarch64__)
struct MmuCfg_t {
    uint64_t    vaddr;
    uint64_t    paddr;
    size_t      size;
    int         attrInd;
} MmuCfgTbl[] = {
    { 0x00100000, 0x00100000, 0x00900000, 0 }, /* Main MMR0     */
    { 0x00400000, 0x00400000, 0x00001000, 0 }, /* PSC0          */
    { 0x00A40000, 0x00A40000, 0x00001000, 0 }, /* TimeSyncRouter     */
    { 0x01800000, 0x01800000, 0x00100000, 0 }, /* gicv3         */
    { 0x02400000, 0x02400000, 0x000c0000, 0 }, /* dmtimer       */
    { 0x02800000, 0x02800000, 0x00001000, 0 }, /* uart          */
    { 0x02000000, 0x02000000, 0x00100000, 0 }, /* I2C           */
    { 0x02100000, 0x02100000, 0x00080000, 0 }, /* McSPI         */
    { 0x40F00000, 0x40F00000, 0x00020000, 0 }, /* MCU MMR0      */
    { 0x40d00000, 0x40d00000, 0x00002000, 0 }, /* PLL0          */
    { 0x47000000, 0x47000000, 0x00100000, 0 }, /* FSS0 cfg          */
    { 0x400000000, 0x400000000, 0x400000000, 0 }, /* FSS0 data          */
    { 0x40f90000, 0x40f90000, 0x00001000, 0 }, /* RAT cfg          */
    { 0x43000000, 0x43000000, 0x00020000, 0 }, /* WKUP MMR0     */
    { 0x02C40000, 0x02C40000, 0x00100000, 0 }, /* pinmux ctrl   */
    { 0x30800000, 0x30800000, 0x0C000000, 0 }, /* main navss    */
    { 0x28380000, 0x28380000, 0x03880000, 0 }, /* mcu navss     */
    { 0x30000000, 0x30000000, 0x0F000000, 0 }, /* ctrcontrol0   */
    { CSL_MCU_CPSW0_NUSS_BASE, CSL_MCU_CPSW0_NUSS_BASE,
        (CSL_MCU_CPSW0_NUSS_SIZE*2), 0 },      /* for CPSW      */
    { 0x0b000000, 0x0b000000, 0x00100000, 0 }, /* ICSS-G 0      */
    { 0x0b100000, 0x0b100000, 0x00100000, 0 }, /* ICSS-G 1      */
    { 0x0b200000, 0x0b200000, 0x00100000, 0 }, /* ICSS-G 2      */
    { 0x42000000, 0x42000000, 0x00001000, 0 }, /* PSC WKUP      */
    { 0x70000000, 0x70000000, 0x04000000, 7 }, /* msmc          */
    { 0x41C00000, 0x41C00000, 0x00080000, 7 }, /* ocmc          */
    { 0x80000000, 0x80000000, 0x10000000, 7 }, /* ddr_0          */
    { 0, 0, 0, 8 } /* attrInd = 8 -> end of table */
};

void InitMmu(void)
{
    bool        retVal = FALSE;
    uint32_t    j = 0;
    Mmu_MapAttrs attrs;
    Mmu_initMapAttrs(&attrs);
    while (MmuCfgTbl[j].attrInd < 8) {
        attrs.attrIndx = MmuCfgTbl[j].attrInd;
        retVal = Mmu_map(MmuCfgTbl[j].vaddr, MmuCfgTbl[j].paddr, MmuCfgTbl[j].size, &attrs);
        if(retVal == FALSE)
            break;
        j++;
    }

    if(retVal == FALSE) {
        UART_printf("Mmu_map idx %d returned error %d", j, retVal);
        while(1);
    }
}
#endif // __aarch64__
#endif // SOC_AM65XX
