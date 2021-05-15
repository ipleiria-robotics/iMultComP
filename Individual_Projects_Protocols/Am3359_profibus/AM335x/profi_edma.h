/*
 * Copyright (c) 2014-2015, Texas Instruments Incorporated
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
/** ============================================================================
 * @file    profi_edma.h
 * @brief   EDMA support functions for profibus slave
 *
 */
#ifndef PROFI_EDMA_H
#define PROFI_EDMA_H

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/** @addtogroup PB_ARM_DMA_UTILITY
 @{ */
/**
 *
 * @brief Used to specify EDMA channel used by ARM
 */
#define ARM_EDMA_CHANNEL   2

/**
 *
 * @brief Used to specify EDMA channel used by PRU
 */
#define PRU_EDMA_CHANNEL   ARM_EDMA_CHANNEL+1

/**
 *
 * @brief Used to specify EDMA shadow region used by ARM subsystem
 */
#define ARM_EDMA_REGION    0
/**
 *
 * @brief Used to specify EDMA shadow region used by PRU subsystem
 */
#define PRU_EDMA_REGION    2
/**
 * @brief Initialize the EDMA DRV and RM
 *
 */
void profi_edma_init(void);


/**
 * @brief allocates EDMA channel and registers a callback function
 *                  with the channel
 * @param[in] channel number to be allocated
 * @retval  0 on success, -1 on failure
 */
int edma_allocate_channel(unsigned int *channel);

/**
 * @brief Frees the edma channel
 *
 * @param[in] channel number to be freed
 * @retval  0 on success, -1 on failure
 */
int edma_free_channel(unsigned int *channel);

/**
 * @brief Starts memcpy using EDMA channel
 *
 * @param[in] channel number to be used for memcpy
 *
 * @param[out] dst destination buffer address for memcpy
 *
 * @param[in] src source buffer address for memcpy
 *
 * @param[in] length number of bytes to be copied
 * @retval  0 on success, -1 on failure
 */
int edma_memcpy(int channel, unsigned int dst, unsigned int src,
                unsigned int length);

/**
 * @brief Starts memset using EDMA channel
 *
 * @param[in] channel number to be used for memcpy
 *
 * @param[out] dst destination buffer address for memset
 *
 * @param[in] src source buffer address for memset
 *
 * @param[in] length number of bytes to be memset
 * @retval  0 on success, -1 on failure
 */
int edma_memset(int channel, unsigned int dst,
                unsigned int src, unsigned int length);

/**
@}
*/
#endif
