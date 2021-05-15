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


#include <board_am335x/include/board_misc.h>
#include <ti/drv/i2c/I2C.h>
#include <ti/drv/spi/SPI.h>
#include <board_am335x/include/board_rotary_switch.h>
#include <board_am335x/include/board_spi.h>
#include <ti/csl/hw_types.h>
#include <board_am335x/include/board_mcspi.h>


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

void Board_delay(uint32_t delay)
{
    volatile uint32_t delay1 = delay * 2;

    while(delay1--) ;
}

void Board_getDigInput(uint8_t *switchs)
{

    Board_readHVS(switchs);

}




#define ASSERT_DMB() __asm("    dmb")
#define ASSERT_DSB() __asm("    dsb")

void * utils_memcpy(uint8_t *dst, const uint8_t *src, uint32_t size_bytes)
{
    uint32_t i;
    volatile uint8_t * dst_ptr = dst;

    for (i = 0; i < size_bytes; i++)
    {
      *dst_ptr++ = *src++;
    }

    ASSERT_DMB();
    ASSERT_DSB();
    return dst;
}

void * utils_memset(uint8_t *dst, int8_t val, uint32_t size_bytes)
{
    uint32_t i;
    volatile uint8_t * dst_ptr = dst;

    for (i = 0; i < size_bytes; i++)
    {
      *dst_ptr++ = val;
    }

    ASSERT_DMB();
    ASSERT_DSB();
    return dst;
}


