/**
 * @file  icss_timeSync_ecap_edma.h
 *
 * @brief Include file for icss_timeSync_ecap_edma.c
 *
 *\par
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 *\par
 */

#ifndef ICSS_TIMESYNC_ECAP_EDMA_H_
#define ICSS_TIMESYNC_ECAP_EDMA_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/sdo/edma3/drv/edma3_drv.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/cslr_ecap.h>
#include "icss_timeSync.h"
#include "icss_timeSyncApi.h"


/**
 * @internal
 * @def EDMA3_ECAPEVT0_CHA_NUM
 *      EDMA3_ECAPEVT0_CHA_NUM
 */
#define EDMA3_ECAPEVT0_CHA_NUM              (38U)

/**
 * @internal
 * @def EDMA3_ECAPEVT0_LINK_CHA_NUM
 *      EDMA3_ECAPEVT0_LINK_CHA_NUM
 */
#define EDMA3_ECAPEVT0_LINK_CHA_NUM         (39U)

/**
 * @internal
 * @def EDMA3_CLEAR_ECAP_CHA_NUM
 *      EDMA3_CLEAR_ECAP_CHA_NUM
 */
#define EDMA3_CLEAR_ECAP_CHA_NUM            (40U)

/**
 * @internal
 * @def EDMA3_CLEAR_ECAP_LINK_CHA_NUM
 *      EDMA3_CLEAR_ECAP_LINK_CHA_NUM
 */
#define EDMA3_CLEAR_ECAP_LINK_CHA_NUM       (41U)

/**
 * @internal
 * @def EDMA3_SYNC0_RETRIGGER_CHA_NUM
 *      EDMA3_SYNC0_RETRIGGER_CHA_NUM
 */
#define EDMA3_SYNC0_RETRIGGER_CHA_NUM       (42)

/**
 * @internal
 * @def EDMA3_SYNC0_RETRIGGER_LINK_CHA_NUM
 *      EDMA3_SYNC0_RETRIGGER_LINK_CHA_NUM
 */
#define EDMA3_SYNC0_RETRIGGER_LINK_CHA_NUM  (43)

/**
 * @internal
 * @def EVT_QUEUE_NUM
 *      EDMA3 Event queue number
 */
#define EVT_QUEUE_NUM                       (0)

/* OPT Field specific defines */
#define OPT_SYNCDIM_SHIFT                   (0x00000002u)
#define OPT_TCC_MASK                        (0x0003F000u)
#define OPT_TCC_SHIFT                       (0x0000000Cu)
#define OPT_ITCINTEN_SHIFT                  (0x00000015u)
#define OPT_TCINTEN_SHIFT                   (0x00000014u)
#define OPT_STATIC_SHIFT                    (0x00000003u)

/**
 * @internal
 * @def MULTIPLICATION_FACTOR
 *      Used to calculate ecap period. Is SoC specific
 */
#if  defined(SOC_AM437x)
#define   MULTIPLICATION_FACTOR     10
#else
#define   MULTIPLICATION_FACTOR     5
#endif

/** @addtogroup PTP_API
 @{ */
/**
 *  @brief Init function which calls other API's to initialize ECAP and EDMA modules
 *  @param timeSyncHandle Pointer to PTP Handle
 *
 *  @retval Error value
 *
 */
int32_t TimeSync_ecapEdmaConfig(TimeSync_ParamsHandle_t timeSyncHandle);

/**
 *  @internal
 *  @brief Initialize ECAP module
 *
 *  @param args timeSyncHandle Pointer to PTP Handle
 *
 *  @retval None
 *
 */
void TimeSync_initEcap(TimeSync_ParamsHandle_t timeSyncHandle);

/**
 *  @internal
 *  @brief Initialize ECAP module
 *
 *  @param args timeSyncHandle Pointer to PTP Handle
 *
 *  @retval Error value
 *
 */
int32_t TimeSync_edmaConfig(TimeSync_ParamsHandle_t timeSyncHandle);

/**
 *  @internal
 *  @brief Computes the ECAP period and adjustment value (per cycle) from the clock drift/offset
 *
 *  @param args timeSyncHandle Pointer to PTP Handle
 *  @param args adjOffset Current clock drift/offset from master
 *
 *  @retval None
 *
 */
void TimeSync_adjTimeEcapEdma(TimeSync_ParamsHandle_t timeSyncHandle,
                              int32_t adjOffset);

/**
@}
*/
#endif /* ICSS_TIMESYNC_ECAP_EDMA_H_ */
