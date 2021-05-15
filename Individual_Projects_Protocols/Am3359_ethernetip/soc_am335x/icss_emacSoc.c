/******************************************************************************
 * FILE PURPOSE:   ICSS_EMAC Peripheral Device Configuration
 ******************************************************************************
 * FILE NAME:   icss_emacSoc.c
 *
 * DESCRIPTION: ICSS_EMAC Peripheral Device Configuration Mapping
 *
 * REVISION HISTORY:
 *
 */
 
/* Copyright (C) {2016} Texas Instruments Incorporated - http://www.ti.com/ 
*
*   Redistribution and use in source and binary forms, with or without 
*   modification, are permitted provided that the following conditions 
*   are met:
*
*     Redistributions of source code must retain the above copyright 
*     notice, this list of conditions and the following disclaimer.
*
*     Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the 
*     documentation and/or other materials provided with the   
*     distribution.
*
*     Neither the name of Texas Instruments Incorporated nor the names of
*     its contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#include <stdint.h>
#include <ti/starterware/include/hw/soc_am335x.h>
#include <ti/drv/icss_emac/soc/icss_emacSoc.h>


ICSS_EmacBaseAddrCfgParams icss_EmacBaseAddrCfgParams[2] =
{
    {
        SOC_PRU_ICSS_MII_MDIO_REG,
        SOC_PRU_ICSS_DATA_RAM0,
        SOC_PRU_ICSS_DATA_RAM1,
        SOC_PRU_ICSS_OCMC_RAM,                                  /* L3 OCMC Base Memory Address should be 256 byte aligned   */
        SOC_PRU_ICSS_SHARED_RAM,
        SOC_PRU_ICSS_INTC_REG,
        SOC_PRU_ICSS_PRU0_CTRL_REG,
        SOC_PRU_ICSS_PRU1_CTRL_REG,
        SOC_PRU_ICSS_IEP_REG,
        SOC_PRU_ICSS_CFG_REG,
        SOC_PRU_ICSS_MII_RT_CFG_REG,
        SOC_PRU_ICSS_DATA_RAM0_SIZE,
        SOC_PRU_ICSS_DATA_RAM0_SIZE,
        SOC_PRU_ICSS_OCMC_RAM_SIZE,
        SOC_PRU_ICSS_SHARED_RAM_SIZE
    },
    {
        (uint32_t)NULL,
        (uint32_t)NULL,
        (uint32_t)NULL,
        (uint32_t)NULL,
        (uint32_t)NULL,
        (uint32_t)NULL,
        (uint32_t)NULL,
        (uint32_t)NULL,
        (uint32_t)NULL,
        (uint32_t)NULL,
        (uint32_t)NULL,
        (uint32_t)NULL,
        (uint32_t)NULL,
        (uint32_t)NULL,
        (uint32_t)NULL
    }
};

/**
 * \brief  This API gets the SoC level of PRU-ICSS  intial configuration
 *
 * \param  index     PRU-ICSS  instance index.
 * \param  cfg       Pointer to PRU-ICSS  SOC initial config.
 *
 * \return 0 success: -1: error
 *
 */
int32_t ICSS_EmacSocGetInitCfg(uint32_t instance, ICSS_EmacBaseAddrCfgParams *cfg)
{
    int32_t ret = 0;

    if (instance < PRUICCSS_INSTANCE_MAX)
    {
        *cfg = icss_EmacBaseAddrCfgParams[instance];
    }
    else
    {
        ret = -((int32_t)1);
    }

    return ret;
}

/**
 * \brief  This API sets the SoC level of PRU-ICSS intial configuration
 *
 * \param  index     PRU-ICSS instance index.
 * \param  cfg       Pointer to PRU-ICSS SOC initial config.
 *
 * \return           0 success: -1: error
 *
 */
int32_t ICSS_EmacSocSetInitCfg(uint32_t instance, const ICSS_EmacBaseAddrCfgParams *cfg)
{
    int32_t ret = 0;

    if (instance < PRUICCSS_INSTANCE_MAX)
    {
        icss_EmacBaseAddrCfgParams[instance] = *cfg;
    }
    else
    {
        ret = -((int32_t)1);
    }

    return ret;
}






