/**
 * @file    pruss_intc_mapping.h
 * @brief   PRUSS INTC mapping for the application
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
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/**
* @internal
* @brief ECAP interrupt no..
*/
#define ECAP0_INTERRUPT     15
/**
* @internal
* @brief UART1 interrupt no..
*/
#define UART1_INTERRUPT     6
/**
* @internal
* @brief PRU0_PRU1 interrupt no..
*/
#define PRU0_PRU1_INTERRUPT 16
/**
* @internal
* @brief PRU1_ARM interrupt no..
*/
#define PRU1_ARM_INTERRUPT      17
/**
* @internal
* @brief ARM_PRU1 interrupt no..
*/
#define ARM_PRU1_INTERRUPT  18
/**
* @internal
* @brief PRU1_PRU0 interrupt no..
*/
#define PRU1_PRU0_INTERRUPT     19

/**
 * @internal
 * @brief output channel 0
 */
#define CHANNEL0        0
/**
 * @internal
 * @brief output channel 1
 */
#define CHANNEL1        1
/**
 * @internal
 * @brief output channel 2
 */
#define CHANNEL2        2

/**
 * @internal
 * @brief PRU instance 0
 */
#define PRU0                    0
/**
 * @internal
 * @brief PRU instance 1
 */
#define PRU1                    1


#define PRU_EVTOUT0         2

#define PRU0_HOSTEN_MASK        0x1
#define PRU1_HOSTEN_MASK        0x2
#define PRU_EVTOUT0__HOSTEN_MASK  0x4

#define SYS_EVT_POLARITY_LOW        0
#define SYS_EVT_POLARITY_HIGH       1

#define SYS_EVT_TYPE_PULSE          0
#define SYS_EVT_TYPE_EDGE           1


/**
 * @internal
 * @brief Interrupt to output channel mapping
 */
#define PRUSS_INTC_INITDATA {   \
        { ECAP0_INTERRUPT, UART1_INTERRUPT,   PRU0_PRU1_INTERRUPT, PRU1_ARM_INTERRUPT, ARM_PRU1_INTERRUPT, PRU1_PRU0_INTERRUPT, 0xFF  },  \
        { {ECAP0_INTERRUPT,CHANNEL0, SYS_EVT_POLARITY_HIGH, SYS_EVT_TYPE_PULSE}, \
            {UART1_INTERRUPT, CHANNEL0, SYS_EVT_POLARITY_HIGH, SYS_EVT_TYPE_PULSE}, \
            {PRU0_PRU1_INTERRUPT,CHANNEL1, SYS_EVT_POLARITY_HIGH, SYS_EVT_TYPE_PULSE}, \
            {PRU1_ARM_INTERRUPT, CHANNEL2, SYS_EVT_POLARITY_HIGH, SYS_EVT_TYPE_PULSE}, \
            {ARM_PRU1_INTERRUPT, CHANNEL1, SYS_EVT_POLARITY_HIGH, SYS_EVT_TYPE_PULSE}, \
            {PRU1_PRU0_INTERRUPT, CHANNEL0, SYS_EVT_POLARITY_HIGH, SYS_EVT_TYPE_PULSE}, \
            {0xFF,0xFF, 0xFF, 0xFF}}, \
        { {CHANNEL0,PRU0}, {CHANNEL1, PRU1}, {CHANNEL2, PRU_EVTOUT0}, {0xFF, 0xFF} },  \
        (PRU0_HOSTEN_MASK | PRU1_HOSTEN_MASK | PRU_EVTOUT0__HOSTEN_MASK ) \
    }
