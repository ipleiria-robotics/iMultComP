/*
 * Copyright (c) 2014-2019, Texas Instruments Incorporated
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
 */
/** ============================================================================*/

#include <ti/csl/csl_utils.h>
#include <ti/drv/uart/UART.h>
#include <ti/starterware/include/types.h>
#include <ti/starterware/include/hw/soc_am335x.h>
#include <ti/drv/uart/soc/UART_soc.h>

#define CSL_UART_PER_CNT  (6U)

#define CSL_EDMA3_CHA_UART0_RX    (27U)
#define CSL_EDMA3_CHA_UART0_TX    (26U)
#define CSL_EDMA3_CHA_UART1_RX    (29U)
#define CSL_EDMA3_CHA_UART1_TX    (28U)
#define CSL_EDMA3_CHA_UART2_RX    (31U)
#define CSL_EDMA3_CHA_UART2_TX    (30U)
#define CSL_EDMA3_CHA_UART3_RX    (8U)     /* Cross bar mapped */
#define CSL_EDMA3_CHA_UART3_TX    (7U)     /* Cross bar mapped */
#define CSL_EDMA3_CHA_UART4_RX    (10U)    /* Cross bar mapped */
#define CSL_EDMA3_CHA_UART4_TX    (9U)     /* Cross bar mapped */
#define CSL_EDMA3_CHA_UART5_RX    (12U)    /* Cross bar mapped */
#define CSL_EDMA3_CHA_UART5_TX    (11U)    /* Cross bar mapped */

#ifdef PRU_ICSS_FW
#define UART_SWIP_MAX_CNT   ( 6U ) // maximum number of UART PRU FW Software IPs
#endif

/* UART configuration structure */
UART_HwAttrs uartInitCfg[CSL_UART_PER_CNT] =
{
    {
        SOC_UART_0_REGS,
        72,
        0,
        48000000U,
        CSL_EDMA3_CHA_UART0_RX,
        CSL_EDMA3_CHA_UART0_TX,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        TRUE, /* default DMA mode */
        FALSE, /* Loopback disabled by default */
		TRUE, /* Interrupt enabled by default */
    },
    {
        SOC_UART_1_REGS,
        73,
        0,
        48000000U,
        CSL_EDMA3_CHA_UART1_RX,
        CSL_EDMA3_CHA_UART1_TX,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        TRUE, /* default DMA mode */
        FALSE, /* Loopback disabled by default */
		TRUE, /* Interrupt enabled by default */
        UART16x_OPER_MODE, /* operMode, 16x over sampling mode by default */
    },
    {
        SOC_UART_2_REGS,
        74,
        69,
        48000000U,
        CSL_EDMA3_CHA_UART2_RX,
        CSL_EDMA3_CHA_UART2_TX,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        TRUE, /* default DMA mode */
        FALSE, /* Loopback disabled by default */
		TRUE, /* Interrupt enabled by default */
        UART16x_OPER_MODE,
    },
    {
        SOC_UART_3_REGS,
        44,
        0,
        48000000U,
        CSL_EDMA3_CHA_UART3_RX,
        CSL_EDMA3_CHA_UART3_TX,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        TRUE, /* default DMA mode */
        FALSE, /* Loopback disabled by default */
		TRUE, /* Interrupt enabled by default */
        UART16x_OPER_MODE,
    },
    {
        SOC_UART_4_REGS,
        45,
        0,
        48000000U,
        CSL_EDMA3_CHA_UART4_RX,
        CSL_EDMA3_CHA_UART4_TX,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        TRUE, /* default DMA mode */
        FALSE, /* Loopback disabled by default */
		TRUE, /* Interrupt enabled by default */
        UART16x_OPER_MODE,
    },
    {
        SOC_UART_5_REGS,
        46,
        0,
        48000000U,
        CSL_EDMA3_CHA_UART5_RX,
        CSL_EDMA3_CHA_UART5_TX,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        TRUE, /* default DMA mode */
        FALSE, /* Loopback disabled by default */
		TRUE, /* Interrupt enabled by default */
        UART16x_OPER_MODE,
    },
};

/* UART objects */
UART_V1_Object UartObjects[CSL_UART_PER_CNT];

#ifdef PRU_ICSS_FW
/* UART FW Memory Ptr */
const UART_ICSS_Mem_Ptr uartFwMemPtr = {
    (uint32_t *)&pru_dmem0_rev1_start,
    (uint32_t *)&pru_imem0_rev1_start,
    (uint32_t *)&pru_dmem0_rev1_end,
    (uint32_t *)&pru_imem0_rev1_end,
    (uint32_t *)&pru_dmem1_rev1_start,
    (uint32_t *)&pru_imem1_rev1_start,
    (uint32_t *)&pru_dmem1_rev1_end,
    (uint32_t *)&pru_imem1_rev1_end
};

/* UART Software IP configuration structure */
UART_SWIPAttrs uartInitCfg2[UART_SWIP_MAX_CNT] __attribute__ ((section(".shdata"))) =
{
    {
        (uint32_t)SOC_PRU_ICSS_DATA_RAM0 + (uint32_t)ICSS_UART_GCFG_ADDR,       // UART global configuration address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM0 + (uint32_t)ICSS_UART0_CFG_ADDR,       // UART instance base address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM0 + (uint32_t)UART0_TX_BUFFER,           // UART instance Tx buffer base address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM0 + (uint32_t)UART0_RX_BUFFER,           // UART instance Rx buffer base address
        20,                                                                     // UART instance Host interrupt number
        UART_ICSS_INSTANCE1,                                                    // UART ICSS instance ID
        PRUICCSS_PRU0,                                                          // UART PRU core ID
        &uartFwMemPtr,                                                          // Pointer to PRU UART FW ICSS IMEM/DMEM addresses
        
        //0X44e10990: SOC_CONTROL_REGS + PIN_MCASP0_ACLKX
        //0x44E10994: SOC_CONTROL_REGS + PIN_MCASP0_FSX
        {0X44e10990, 0, 0x6, 0x5},                                              // UART instance Tx output pin mux register
        {0x44E10994, 1, 0x6, 0x5},                                              // UART instance Rx input pin mux register
        {0xFFFFFFFF, 0xFF, 0xFF, 0xFF},                                         // UART instance CTS input pin mux register
        {0xFFFFFFFF, 0xFF, 0xFF, 0xFF},                                         // UART instance RTS output pin mux register
        
        UART_INSTANCE_ID0,                                                      // UART instance ID on PRU
        PRU_ARM_EVENT05,                                                        // UART instance Tx ICSS INTC System Event
        PRU_ARM_EVENT02,                                                        // UART instance Rx ICSS INTC System Event
        true,                                                                   // enable interrupts
        false                                                                   // enable hardware flow control pins
    }, 
    {
        (uint32_t)SOC_PRU_ICSS_DATA_RAM0 + (uint32_t)ICSS_UART_GCFG_ADDR,       // UART global configuration address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM0 + (uint32_t)ICSS_UART1_CFG_ADDR,       // UART instance base address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM0 + (uint32_t)UART1_TX_BUFFER,           // UART instance Tx buffer base address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM0 + (uint32_t)UART1_RX_BUFFER,           // UART instance Rx buffer base address
        21,                                                                     // UART instance Host interrupt number
        UART_ICSS_INSTANCE1,                                                    // UART ICSS instance ID
        PRUICCSS_PRU0,                                                          // UART PRU core ID
        &uartFwMemPtr,                                                          // Pointer to PRU UART FW ICSS IMEM/DMEM addresses
        
        //0X44e10998: SOC_CONTROL_REGS + PIN_MCASP0_AXR0
        //0x44E10994: SOC_CONTROL_REGS + PIN_MCASP0_AHCLKR
        {0X44e10998, 2, 0x6, 0x5},                                              // UART instance Tx output pin mux register
        {0x44E1099C, 3, 0x6, 0x5},                                              // UART instance Rx input pin mux register
        {0xFFFFFFFF, 0xFF, 0xFF, 0xFF},                                         // UART instance CTS input pin mux register
        {0xFFFFFFFF, 0xFF, 0xFF, 0xFF},                                         // UART instance RTS output pin mux register
        
        UART_INSTANCE_ID1,                                                      // UART instance ID on PRU
        PRU_ARM_EVENT06,                                                        // UART instance Tx ICSS INTC System Event
        PRU_ARM_EVENT03,                                                        // UART instance Rx ICSS INTC System Event
        true,                                                                   // enable interrupts
        false                                                                   // enable hardware flow control pins
    }, 
    {
        (uint32_t)SOC_PRU_ICSS_DATA_RAM0 + (uint32_t)ICSS_UART_GCFG_ADDR,       // UART global configuration address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM0 + (uint32_t)ICSS_UART2_CFG_ADDR,       // UART instance base address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM0 + (uint32_t)UART2_TX_BUFFER,           // UART instance Tx buffer base address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM0 + (uint32_t)UART2_RX_BUFFER,           // UART instance Rx buffer base address
        22,                                                                     // UART instance Host interrupt number
        UART_ICSS_INSTANCE1,                                                    // UART ICSS instance ID
        PRUICCSS_PRU0,                                                          // UART PRU core ID
        &uartFwMemPtr,                                                          // Pointer to PRU UART FW ICSS IMEM/DMEM addresses
        
        //0X44e109A4: SOC_CONTROL_REGS + PIN_MCASP0_FSR
        //0x44E10994: SOC_CONTROL_REGS + PIN_MCASP0_AHCLKX
        {0X44e109A4, 5, 0x6, 0x5},                                              // UART instance Tx output pin mux register
        {0x44E109AC, 7, 0x6, 0x5},                                              // UART instance Rx input pin mux register
        {0xFFFFFFFF, 0xFF, 0xFF, 0xFF},                                         // UART instance CTS input pin mux register
        {0xFFFFFFFF, 0xFF, 0xFF, 0xFF},                                         // UART instance RTS output pin mux register
        
        UART_INSTANCE_ID2,                                                      // UART instance ID on PRU
        PRU_ARM_EVENT07,                                                        // UART instance Tx ICSS INTC System Event
        PRU_ARM_EVENT04,                                                        // UART instance Rx ICSS INTC System Event
        true,                                                                   // enable interrupts
        false                                                                   // enable hardware flow control pins
    }, 
    {
        (uint32_t)SOC_PRU_ICSS_DATA_RAM1 + (uint32_t)ICSS_UART_GCFG_ADDR,       // UART global configuration address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM1 + (uint32_t)ICSS_UART0_CFG_ADDR,       // UART instance base address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM1 + (uint32_t)UART0_TX_BUFFER,           // UART instance Tx buffer base address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM1 + (uint32_t)UART0_RX_BUFFER,           // UART instance Rx buffer base address
        23,                                                                     // UART instance Host interrupt number
        UART_ICSS_INSTANCE1,                                                    // UART ICSS instance ID
        PRUICCSS_PRU1,                                                          // UART PRU core ID
        &uartFwMemPtr,                                                          // Pointer to PRU UART FW ICSS IMEM/DMEM addresses
        
        //0x44E108A0: SOC_CONTROL_REGS + PIN_LCD_DATA0
        //0x44E108A4: SOC_CONTROL_REGS + PIN_LCD_DATA1
        //0x44E108A8: SOC_CONTROL_REGS + PIN_LCD_DATA2
        //0x44E108AC: SOC_CONTROL_REGS + PIN_LCD_DATA3
        {0X44e108A0, 0, 0x6, 0x5},                                              // UART instance Tx output pin mux register
        {0x44E108A4, 1, 0x6, 0x5},                                              // UART instance Rx input pin mux register
        {0x44E108A8, 2, 0x6, 0x5},                                              // UART instance CTS input pin mux register
        {0x44E108AC, 3, 0x6, 0x5},                                              // UART instance RTS output pin mux register
        
        UART_INSTANCE_ID0,                                                      // UART instance ID on PRU
        PRU_ARM_EVENT11,                                                        // UART instance Tx ICSS INTC System Event
        PRU_ARM_EVENT08,                                                        // UART instance Rx ICSS INTC System Event
        true,                                                                   // enable interrupts
        true                                                                    // enable hardware flow control pins
    }, 
    {
        (uint32_t)SOC_PRU_ICSS_DATA_RAM1 + (uint32_t)ICSS_UART_GCFG_ADDR,       // UART global configuration address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM1 + (uint32_t)ICSS_UART1_CFG_ADDR,       // UART instance base address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM1 + (uint32_t)UART1_TX_BUFFER,           // UART instance Tx buffer base address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM1 + (uint32_t)UART1_RX_BUFFER,           // UART instance Rx buffer base address
        24,                                                                     // UART instance Host interrupt number
        UART_ICSS_INSTANCE1,                                                    // UART ICSS instance ID
        PRUICCSS_PRU1,                                                          // UART PRU core ID
        &uartFwMemPtr,                                                          // Pointer to PRU UART FW ICSS IMEM/DMEM addresses
        
        //0x44E108B0: SOC_CONTROL_REGS + PIN_LCD_DATA4
        //0x44E108B4: SOC_CONTROL_REGS + PIN_LCD_DATA5
        //0x44E108B8: SOC_CONTROL_REGS + PIN_LCD_DATA6
        //0x44E108BC: SOC_CONTROL_REGS + PIN_LCD_DATA7
        {0X44e108B0, 4, 0x6, 0x5},                                              // UART instance Tx output pin mux register
        {0x44E108B4, 5, 0x6, 0x5},                                              // UART instance Rx input pin mux register
        {0x44E108B8, 6, 0x6, 0x5},                                              // UART instance CTS input pin mux register
        {0x44E108BC, 7, 0x6, 0x5},                                              // UART instance RTS output pin mux register
        
        UART_INSTANCE_ID1,                                                      // UART instance ID on PRU
        PRU_ARM_EVENT12,                                                        // UART instance Tx ICSS INTC System Event
        PRU_ARM_EVENT09,                                                        // UART instance Rx ICSS INTC System Event
        true,                                                                   // enable interrupts
        true                                                                    // enable hardware flow control pins
    }, 
    {
        (uint32_t)SOC_PRU_ICSS_DATA_RAM1 + (uint32_t)ICSS_UART_GCFG_ADDR,       // UART global configuration address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM1 + (uint32_t)ICSS_UART2_CFG_ADDR,       // UART instance base address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM1 + (uint32_t)UART2_TX_BUFFER,           // UART instance Tx buffer base address
        (uint32_t)SOC_PRU_ICSS_DATA_RAM1 + (uint32_t)UART2_RX_BUFFER,           // UART instance Rx buffer base address
        25,                                                                     // UART instance Host interrupt number
        UART_ICSS_INSTANCE1,                                                    // UART ICSS instance ID
        PRUICCSS_PRU1,                                                          // UART PRU core ID
        &uartFwMemPtr,                                                          // Pointer to PRU UART FW ICSS IMEM/DMEM addresses
        
        //0x44E108E0: SOC_CONTROL_REGS + PIN_LCD_VSYNC
        //0x44E108E4: SOC_CONTROL_REGS + PIN_LCD_HSYNC
        //0x44E108E8: SOC_CONTROL_REGS + PIN_LCD_PCLK
        //0x44E108EC: SOC_CONTROL_REGS + PIN_LCD_AC_BIAS_EN
        {0X44e108E0, 8, 0x6, 0x5},                                              // UART instance Tx output pin mux register
        {0x44E108E4, 9, 0x6, 0x5},                                              // UART instance Rx input pin mux register
        {0x44E108E8, 10, 0x6, 0x5},                                             // UART instance CTS input pin mux register
        {0x44E108EC, 11, 0x6, 0x5},                                             // UART instance RTS output pin mux register
        
        UART_INSTANCE_ID2,                                                      // UART instance ID on PRU
        PRU_ARM_EVENT13,                                                        // UART instance Tx ICSS INTC System Event
        PRU_ARM_EVENT10,                                                        // UART instance Rx ICSS INTC System Event
        true,                                                                   // enable interrupts
        true                                                                    // enable hardware flow control pins
    }
};

/* UART objects */
UART_V2_Object UartObjects2[UART_SWIP_MAX_CNT];
#endif

/* UART configuration structure */
UART_config_list UART_config = {
    {
        &UART_FxnTable_v1,
        &UartObjects[0],
        &uartInitCfg[0]
    },

    {
         &UART_FxnTable_v1,
         &UartObjects[1],
         &uartInitCfg[1]
    },

    {
         &UART_FxnTable_v1,
         &UartObjects[2],
         &uartInitCfg[2]
    },

    {
         &UART_FxnTable_v1,
         &UartObjects[3],
         &uartInitCfg[3]
    },

    {
         &UART_FxnTable_v1,
         &UartObjects[4],
         &uartInitCfg[4]
     },

     {
          &UART_FxnTable_v1,
          &UartObjects[5],
          &uartInitCfg[5]
     },
     /* pad to full predefined length of array */
     {NULL, NULL, NULL},
     {NULL, NULL, NULL},
     {NULL, NULL, NULL},
     {NULL, NULL, NULL},
     {NULL, NULL, NULL},
     {NULL, NULL, NULL},
     {NULL, NULL, NULL},
     {NULL, NULL, NULL}
};

/**
 * \brief  This API gets the SoC level of UART intial configuration
 *
 * \param  index     UART instance index.
 * \param  cfg       Pointer to UART SOC initial config.
 *
 * \return 0 success: -1: error
 *
 */
int32_t UART_socGetInitCfg(uint32_t index, UART_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (index < CSL_UART_PER_CNT)
    {
        *cfg = uartInitCfg[index];
    }
    else
    {
        ret = -1;
    }

    return ret;
}

/**
 * \brief  This API sets the SoC level of UART intial configuration
 *
 * \param  index     UART instance index.
 * \param  cfg       Pointer to UART SOC initial config.
 *
 * \return           0 success: -1: error
 *
 */
int32_t UART_socSetInitCfg(uint32_t index, const UART_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (index < CSL_UART_PER_CNT)
    {
        uartInitCfg[index] = *cfg;
    }
    else
    {
        ret = -1;
    }

    return ret;
}

#ifdef PRU_ICSS_FW
/**
 * \brief  This API gets the UART FW configuration
 *
 * \param  index     UART instance index.
 * \param  cfg       Pointer to UART FW config.
 *
 * \return 0 success: -1: error
 *
 */
int32_t UART_socGetFwCfg(uint32_t index, UART_SWIPAttrs *cfg)
{
    int32_t ret = 0;
    
    index = index - CSL_UART_PER_CNT;
    if (index < UART_SWIP_MAX_CNT)
    {
        *cfg = uartInitCfg2[index];
    }
    else
    {
        ret = (-((int32_t)1));
    }

    return ret;
}

/**
 * \brief  This API sets the UART FW configuration
 *
 * \param  index     UART instance index.
 * \param  cfg       Pointer to UART FW config.
 *
 * \return           0 success: -1: error
 *
 */
int32_t UART_socSetFwCfg(uint32_t index, const UART_SWIPAttrs *cfg)
{
    int32_t ret = 0;

    index = index - CSL_UART_PER_CNT;
    if (index < UART_SWIP_MAX_CNT)
    {
        uartInitCfg2[index] = *cfg;
    }
    else
    {
        ret = (-((int32_t)1));
    }

    return ret;
}

/**
 * \brief  This API initialize the UART FW configuration
 *
 * \param     none
 *
 * \return    none
 *
 */
void UART_socInitFwCfg(void)
{
    uint32_t i;

    for (i = 0; i < UART_SWIP_MAX_CNT; i++)
    {
        UART_config[CSL_UART_PER_CNT + i].fxnTablePtr = &UART_v2_FxnTable;
        UART_config[CSL_UART_PER_CNT + i].object      = (void *)(&UartObjects2[i]);
        UART_config[CSL_UART_PER_CNT + i].hwAttrs     = &uartInitCfg2[i];
    }

    return;
}
#endif
