/**
 * @file   pruicss_soc.c
 *
 * @brief  This is soc specific configuration file .
 */
/*
 * Copyright (c) 2015-2018, Texas Instruments Incorporated
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


#include <stdint.h>
#include <ti/drv/pruss/pruicss.h>
#include <ti/drv/pruss/soc/pruicss_v1.h>

#include "ti/starterware/include/hw/soc_am335x.h"


const PRUICSS_HwAttrs prussInitCfg =
{
    0x4a300000,
    0,
    SOC_PRU_ICSS_PRU0_CTRL_REG,
    SOC_PRU_ICSS_PRU1_CTRL_REG,
    SOC_PRU_ICSS_INTC_REG,
    SOC_PRU_ICSS_CFG_REG,
    SOC_PRU_ICSS_UART_REG,
    SOC_PRU_ICSS_IEP_REG,
    SOC_PRU_ICSS_ECAP_REG,
    SOC_PRU_ICSS_MII_RT_CFG_REG,
    SOC_PRU_ICSS_MII_MDIO_REG,
    SOC_PRU_ICSS_DATA_RAM0,
    SOC_PRU_ICSS_DATA_RAM1,
    SOC_PRU_ICSS_INST_RAM0,
    SOC_PRU_ICSS_INST_RAM1,
    SOC_PRU_ICSS_SHARED_RAM,
    0U,
    0U,
    0U,
    0U,
    0U,
    0U,
    0U,
    0U,
    SOC_PRU_ICSS_DATA_RAM0_SIZE,
    SOC_PRU_ICSS_DATA_RAM1_SIZE,
    SOC_PRU_ICSS_INST_RAM0_SIZE,
    SOC_PRU_ICSS_INST_RAM1_SIZE,
    SOC_PRU_ICSS_SHARED_RAM_SIZE,
    0U,
    0U,
    0U,
    0U
};

/* PRUICSS objects */
PRUICSS_V1_Object prussObjects;

/* PRUICSS configuration structure */
PRUICSS_Config pruss_config[PRUICCSS_INSTANCE_MAX] = {
    {

        &prussObjects,
        &prussInitCfg
    },

    {NULL, NULL}
};



