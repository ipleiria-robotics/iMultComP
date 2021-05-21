/**
 * @file debugtrace.h
 *
 * @brief Debug Line Trace Interface
* \par
* Copyright (C) 2017 Texas Instruments Incorporated
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
*
* 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from
* this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* \par
 *
*/
#ifndef DEBUGTRACE_H_
#define DEBUGTRACE_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stdlib.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/System.h>

/**
 * @def DBG_INFO
 *  Use to report informative debug info
 */
#ifndef DBG_INFO
#define DBG_INFO        1
#endif

/**
 * @def DBG_WARN
 *  Use to report warning debug info
 */
#ifndef DBG_WARN
#define DBG_WARN        2
#endif

/**
 * @def DBG_ERROR
 *  Use to report errors
 */
#ifndef DBG_ERROR
#define DBG_ERROR       3
#endif

#ifndef DBG_NONE
#define DBG_NONE        4
#endif

/* Configuration Structure */
typedef struct _dbgtrc_cfg
{
    uint32_t     PrintLevel;     /* Debug print threshold */
    uint32_t     AbortLevel;     /* Debug system abort threshold */
} dbgtrc_cfg_t;
extern dbgtrc_cfg_t gDbgTraceCfg;

#define DEBUG_PRINT_LEVEL gDbgTraceCfg.PrintLevel
#define DEBUG_ABORT_LEVEL gDbgTraceCfg.AbortLevel

/* Default values */
#define DEF_DEBUG_PRINT_LEVEL     DBG_ERROR
#define DEF_DEBUG_ABORT_LEVEL     DBG_ERROR
/**
 * @brief  Logging function for debug
 *
 *     var SysMin = xdc.useModule('xdc.runtime.SysMin');
 *     System.SupportProxy = SysMin;
 *     SysMin.outputFxn = "&myOutputFunc";
 * User can specify myOutputFunc as per requirement in .cfg file and define it in application
 *      #include <stdio.h>
 *      Void myOutputFunc(Char *buf, UInt size)
 *      {
 *          fwrite(buf, sizeof(Char), size, stdout);
 *      }
 *   System_flush() shall be called after System_printf() or call it in a background task to trigger
 *   myOutputFunc, default will print to stdout
 *
 * @param[in] Level      Debug Level
 * @param[in] fmt        Debug Info String
 *
 */
extern void Debug_Printf(uint32_t Level, char *fmt, ...);

#endif /*DEBUGTRACE_H_*/
