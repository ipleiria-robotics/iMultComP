
/** ============================================================================
 * @file    bsp.h
 * @brief   This file contains the hardware abstraction functions for the
 * 			PRU-ICSS Profibus implementation
 *
 */

/****************************************************************************
*
*     TECHNOLOGIE MANAGEMENT GRUPPE Technologie und Engineering GmbH
*
****************************************************************************/
/**
*       \author    Dirk Brauner
*       \date      08.10.2009
*       \file      BSP.H
*       \brief     Hardware Interface
*
* This file contains the hardware interface for the slave stack
*
* \weakgroup BSP Board Support Package
* Hardware abstraction layer
* @{
****************************************************************************/
#ifndef _BSPH_
#define _BSPH_

#include "targetdefs.h"         /* basic data types and defines */
#include "profi_hw_utils.h"
#include "profi_edma.h"

#include "L2.h"
#include "basictyp.h"
#include "pruss_intc_mapping.h"


/* 11 bit time counter values for min tsdr calculation */


#define ELEVEN_BIT_IN_CYCLES 184
#define ELEVEN_BIT_ECAP_TIMER_12M 200 //(ELEVEN_BIT_IN_CYCLES)
#define ELEVEN_BIT_ECAP_TIMER_6M (ELEVEN_BIT_IN_CYCLES*2)
#define ELEVEN_BIT_ECAP_TIMER_3M (ELEVEN_BIT_IN_CYCLES*4)
#define ELEVEN_BIT_ECAP_TIMER_1_5M (ELEVEN_BIT_IN_CYCLES*8)
#define ELEVEN_BIT_ECAP_TIMER_500K (ELEVEN_BIT_IN_CYCLES*24)
#define ELEVEN_BIT_ECAP_TIMER_187K (ELEVEN_BIT_IN_CYCLES*64)
#define ELEVEN_BIT_ECAP_TIMER_93K ((ELEVEN_BIT_IN_CYCLES*128) + 765)
#define ELEVEN_BIT_ECAP_TIMER_45K ((ELEVEN_BIT_IN_CYCLES*264) + 1583)
#define ELEVEN_BIT_ECAP_TIMER_19K ((ELEVEN_BIT_IN_CYCLES*625) + 3735)
#define ELEVEN_BIT_ECAP_TIMER_9K ((ELEVEN_BIT_IN_CYCLES*1250) + 7410)

static const u32 min_tsdr_calculation[BAUD_12M + 1] = {
    ELEVEN_BIT_ECAP_TIMER_9K,
    ELEVEN_BIT_ECAP_TIMER_19K,
	ELEVEN_BIT_ECAP_TIMER_45K,
    ELEVEN_BIT_ECAP_TIMER_93K,
    ELEVEN_BIT_ECAP_TIMER_187K,
    ELEVEN_BIT_ECAP_TIMER_500K,
    ELEVEN_BIT_ECAP_TIMER_1_5M,
    ELEVEN_BIT_ECAP_TIMER_3M,
    ELEVEN_BIT_ECAP_TIMER_6M,
    ELEVEN_BIT_ECAP_TIMER_12M
};
//#define EVAL_VERSION
extern byte timer_inc;

/****************************************************************************
*
*      Name          : BSP_Init
*      Parameters    : --
*      Return Values : --
*      Description   : This function initializes the Hardware
*
****************************************************************************/
/**
 *  @brief This function initializes the Hardware
 *                  
 */

extern void BSP_Init(void);
/**
 * @brief This function cleans up the Hardware drivers
 *                  
 */


void BSP_exit(void);


#if (SW_DMA == 0)
#else
extern void DmaCpy(void XDATA * pDest, void XDATA * pSource, u32 n);
extern void DmaSet(void XDATA * pDest, byte by, u32 n);
#endif

/**
 * @brief This function initializes the Hardware Timer
 *                  
 */
void init_hw_timer(void);

/**
 * @brief This function enables the Timer-Interrupt
 *                  
 */
void enable_timer(void);

/**
 * @brief This function diables the Timer-Interrupt
 *                  
 */
void disable_timer(void);

/**
 * @brief This function initializes the Hardware Timer
 *                  
 */
extern void init_hw_clock(void);

/**
 * @brief Timer-Interrupt-Routine
 *                  
 */
extern void do_int_timer(void);

/**
 * @brief send the command to PRU Firmware
 *                  
 */
extern void SendCommandToFDL(void);

extern THREAD_RETURN user(THREAD_ARGS);
#endif
/*! @} */
