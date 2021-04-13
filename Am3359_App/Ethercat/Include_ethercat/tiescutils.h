/**
 * tiescutils.h
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

#include <Ethercat/Include_ethercat/tiescbsp.h>
#include <Ethercat/Include_ethercat/tieschw.h>


#include <osal/MiscP.h>
#include <osal/ClockP.h>
#include <osal/TaskP.h>
#include <osal/SwiP.h>
#include <ti/osal/SemaphoreP.h>
#include <ti/osal/TimerP.h>


#define APPL_BUILD_VER  "3.3.0"
#include <ti/osal/CacheP.h>

void task1(uint32_t arg0, uint32_t arg1);


#ifdef ENABLE_PDI_TASK
void PDItask(uint32_t arg1, uint32_t arg2);
#endif
#if AL_EVENT_ENABLED
void HW_EcatIsr(void);
#endif
void LEDtask(uint32_t arg0, uint32_t arg1);

#ifdef ENABLE_SPIA_TASK
void SPIAMaster_statusTask(uint32_t arg0, uint32_t arg1);
#endif

#ifdef ENABLE_GPMC_TASK
void GPMC_statusTask(uint32_t arg0, uint32_t arg1);
#endif

#ifdef ENABLE_SYNC_TASK
void Sync0task(uint32_t arg1, uint32_t arg2);
#endif

void common_main();



#ifdef ENABLE_ONLINE_FIRMWARE_UPGRADE
void relocate_reload_code();
#endif

