/**
 *  \file   tiesc_soc.h
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
#ifndef _TIESC_SOC_H_
#define _TIESC_SOC_H_

#ifdef SOC_AM335x
#include <ti/starterware/include/board.h>
#endif
#ifdef SOC_AM437x
#include <ti/starterware/include/hw/am437x.h>
#endif


#define PRUICSS_INSTANCE_ONE   1
#define PRUICSS_INSTANCE_TWO   2

#if defined(SOC_AM571x)
/* GPCFG MUX settings for MII  */
#define GPCFG_GP_MUX_SEL_MII    4
/* EEPROM data offset in SPI/QSPI Flash */
#define SPI_EEPROM_DATA_OFFSET 0x100000
#define DEFAULT_PRUICSS_INSTANCE    PRUICSS_INSTANCE_TWO
#define TIESC_TASK_STACK_SIZE_MUL          1
#elif defined(SOC_AM572x)
/* EEPROM data offset in SPI/QSPI Flash */
#define SPI_EEPROM_DATA_OFFSET 0x100000
#define DEFAULT_PRUICSS_INSTANCE    PRUICSS_INSTANCE_TWO
#define TIESC_TASK_STACK_SIZE_MUL          1
#elif defined(SOC_K2G)
/* EEPROM data offset in SPI/QSPI Flash */
#define SPI_EEPROM_DATA_OFFSET 0x100000
#define DEFAULT_PRUICSS_INSTANCE    PRUICSS_INSTANCE_TWO
#define TIESC_TASK_STACK_SIZE_MUL          1
#elif defined(SOC_AM65XX)
/* EEPROM data offset in SPI/QSPI Flash */
#define SPI_EEPROM_DATA_OFFSET 0x100000
#define DEFAULT_PRUICSS_INSTANCE    PRUICSS_INSTANCE_ONE
#define TIESC_TASK_STACK_SIZE_MUL          2
#elif defined(SOC_AM335x)
/* EEPROM data offset in SPI/QSPI Flash */
#ifdef AMIC11X_ONCHIP_MEM_MODE
#define SPI_EEPROM_DATA_OFFSET 0x10000
#else
#define SPI_EEPROM_DATA_OFFSET 0x100000
#endif
#define DEFAULT_PRUICSS_INSTANCE    PRUICSS_INSTANCE_ONE
#define TIESC_TASK_STACK_SIZE_MUL          1
#elif defined(SOC_AM437x)
/* Mapping offset to OCMC ram  */
#define SOC_ON_CHIP_MEM_OFFSET  SOC_OCMCRAM_REG
/* EEPROM data offset in SPI/QSPI Flash */
#define SPI_EEPROM_DATA_OFFSET 0x100000
#define DEFAULT_PRUICSS_INSTANCE    PRUICSS_INSTANCE_TWO
#define TIESC_TASK_STACK_SIZE_MUL          1
#else
#define DEFAULT_PRUICSS_INSTANCE    PRUICSS_INSTANCE_TWO
#define TIESC_TASK_STACK_SIZE_MUL          1
#endif

/* Change this define to switch between PRUICSS for AM571x */
#define PRUICSS_INSTANCE        DEFAULT_PRUICSS_INSTANCE

#if defined(SOC_AM571x)
#if (PRUICSS_INSTANCE == PRUICSS_INSTANCE_ONE)
#define TIESC_LINK0_POL   TIESC_LINK_POL_ACTIVE_LOW
#define TIESC_LINK1_POL   TIESC_LINK_POL_ACTIVE_LOW
#else
#define TIESC_LINK0_POL   TIESC_LINK_POL_ACTIVE_HIGH
#define TIESC_LINK1_POL   TIESC_LINK_POL_ACTIVE_LOW
#endif
#elif defined(SOC_K2G)
#if (PRUICSS_INSTANCE == PRUICSS_INSTANCE_ONE)
#define TIESC_LINK0_POL   TIESC_LINK_POL_ACTIVE_LOW
#define TIESC_LINK1_POL   TIESC_LINK_POL_ACTIVE_LOW
#else
#define TIESC_LINK0_POL   TIESC_LINK_POL_ACTIVE_LOW
#define TIESC_LINK1_POL   TIESC_LINK_POL_ACTIVE_LOW
#endif
#elif defined(SOC_AM335x)
#if defined(iceAMIC11x)
#define TIESC_LINK0_POL   TIESC_LINK_POL_ACTIVE_HIGH
#define TIESC_LINK1_POL   TIESC_LINK_POL_ACTIVE_HIGH
#else
#define TIESC_LINK0_POL   TIESC_LINK_POL_ACTIVE_LOW
#define TIESC_LINK1_POL   TIESC_LINK_POL_ACTIVE_LOW
#endif
#elif defined(SOC_AM437x)
#define TIESC_LINK0_POL   TIESC_LINK_POL_ACTIVE_HIGH
#define TIESC_LINK1_POL   TIESC_LINK_POL_ACTIVE_LOW
#elif defined(SOC_AM572x)
#define TIESC_LINK0_POL   TIESC_LINK_POL_ACTIVE_HIGH
#define TIESC_LINK1_POL   TIESC_LINK_POL_ACTIVE_LOW
#elif defined(SOC_AM65XX)
#define TIESC_LINK0_POL   TIESC_LINK_POL_ACTIVE_HIGH
#define TIESC_LINK1_POL   TIESC_LINK_POL_ACTIVE_HIGH
#else
#define TIESC_LINK0_POL   TIESC_LINK_POL_ACTIVE_HIGH
#define TIESC_LINK1_POL   TIESC_LINK_POL_ACTIVE_LOW
#endif

#if defined(SOC_K2G)
#define SPINLOCK_GRANTED       1
#define SPINLOCK_UNLOCK        1
#else
#define SPINLOCK_GRANTED       0
#define SPINLOCK_UNLOCK        0
#endif


uint8_t isEtherCATDevice(void);

int16_t getARMInterruptOffset();

uint32_t getSpinlockClkCtrlOffset();

uint32_t getSpinlockLockReg0Offset();

void bsp_soft_reset();

void bsp_soc_evm_init();

void display_esc_version(uint16_t revision, uint16_t build);

void initSpinlock();

#ifdef SOC_AM65XX
void * tiesc_memcpy(uint8_t *dst, const uint8_t *src, uint32_t size_bytes);
void * tiesc_memset(uint8_t *dst, int8_t val, uint32_t size_bytes);
#else
#define tiesc_memcpy    memcpy
#define tiesc_memset    memset
#endif
#endif /* _TIESC_SOC_H_*/
