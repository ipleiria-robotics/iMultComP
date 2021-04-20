/***************************************************************************
 *
 *            Copyright (c) 2015-2017 by HCC Embedded
 *
 * This software is copyrighted by and is the sole property of
 * HCC.  All rights, title, ownership, or other interests
 * in the software remain the property of HCC.  This
 * software may only be used in accordance with the corresponding
 * license agreement.  Any unauthorized use, duplication, transmission,
 * distribution, or disclosure of this software is expressly forbidden.
 *
 * This Copyright notice may not be removed or modified without prior
 * written consent of HCC.
 *
 * HCC reserves the right to modify this software without notice.
 *
 * HCC Embedded
 * Budapest 1133
 * Vaci ut 76
 * Hungary
 *
 * Tel:  +36 (1) 450 1302
 * Fax:  +36 (1) 450 1303
 * http: www.hcc-embedded.com
 * email: info@hcc-embedded.com
 *
 ***************************************************************************/
#ifndef _PSP_STDIO_H_
#define _PSP_STDIO_H_


/* UART Driver Header Files */
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#include "../../version/ver_psp_stdio.h"
#if VER_PSP_STDIO_MAJOR != 1 || VER_PSP_STDIO_MINOR != 2
 #error Incompatible PSP_STDIO version number!
#endif

#define psp_sprintf
#define psp_snprintf
#define psp_printf UART_printf

#endif /* ifndef _PSP_STDIO_H_ */

