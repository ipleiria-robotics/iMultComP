/****************************************************************************
*
*     TECHNOLOGIE MANAGEMENT GRUPPE Technologie und Engineering GmbH
*
****************************************************************************/
/**
*       \author    Dirk Brauner
*       \date      08.10.2009
*       \file      TARGETDEFS.H
*       \brief     Hardware Interface macros
*
* This file contains the hardware interface for the slave stack
*
* \weakgroup BSP Board Support Package
* Hardware abstraction layer
* @{
****************************************************************************/
#ifndef _TARGETDEFSH_
#define _TARGETDEFSH_


#include "basictyp.h"


#define msleep bsp_msleep

#define MULTI_THREAD 0          //No need to simulate PRU here


#define SW_DMA              1

#ifdef EXEC_FROM_IRAM
	#define SW_HEXDUMP 0
#else
	#define SW_HEXDUMP          2
#endif

#define DEFAULT_SLAVE_ADDR  5

#define ASSERT(b)

//#define enable()
//#define disable()

#define TRIGGERWATCHDOG

extern void UARTFDL_main(void);
#define START_LAYER2 UARTFDL_main

#ifdef __linux__

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
// System adaption: (test suite runs under cygwin on windows)
#define MUTEX_DEFINE(m)    pthread_mutex_t m
#define MUTEX_INIT(m)      m = PTHREAD_MUTEX_INITIALIZER
#define MUTEX_CREATE(m)    pthread_mutex_init( &m, NULL);
#define MUTEX_LOCK(m)      pthread_mutex_lock (&m)      // pthread_mutex_trylock
#define MUTEX_FREE(m)      pthread_mutex_unlock (&m)
#define MUTEX_DESTROY(m)   pthread_mutex_destroy(&m)

#define THREAD_DEFINE(h)         pthread_t h
#define THREAD_RETURN            void*
#define THREAD_ARGS              void* pArgs
#define THREAD_INIT              void* pRet=NULL
#define THREAD_EXIT              return pRet
#define THREAD_START(h,c,t,ta)   pthread_create( &(h), c, t, ta)
#define THREAD_STOP(h)           pthread_cancel(h)

#else

#define THREAD_RETURN            void*
#define THREAD_ARGS              void* pArgs
#define THREAD_INIT              void* pRet=NULL
#define THREAD_EXIT              return pRet


#endif 

//TBool g_bDoAbort = 0;
#define THREAD_LOOP              !g_bDoAbort
extern TBool g_bDoAbort;

#if (SW_HEXDUMP > 0)
extern char HexdumpBuffer[2048];
extern void Hexdump(char *sOut, const char *sIntro, int nDisplay,
                    const unsigned char *p, int n);
#endif

#define NUMBER_OF_CONNECTIONS 2


#endif
/*! @} */
