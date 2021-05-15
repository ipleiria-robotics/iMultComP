/****************************************************************************
***
***     TMG i-tec Gesellschaft fuer industrielle Informationstechnik mbH
***
***     Telefon: +49-721-96410-0
***     Telefax: +49-721-96410-10
***
***
*****************************************************************************
***
***      Zielsystem   : portable
***      Werkzeuge    : Borland-C++ (Ansi) / IAR-C (68HC11) / Keil-C (8031)
***
*****************************************************************************
***
***     Autor        : Josef Albert / Dirk Brauner
***     Dateiname    : timer.h
***
*****************************************************************************
***
***     Beschreibung : This modul contains the interface to the TIMER functions
***
*****************************************************************************
***
***     Version        Date       Name         Comment
***
***
***
****************************************************************************/
#ifndef _TIMERH_
#define _TIMERH_

#include "dpe.h"

/****************************************************************************
**
**     Data types / Defines
*/

#define NUMBER_OF_COUNTER     (2*DPV1_CONNECTIONS+2)
#define BAUDRATE_COUNTER (NUMBER_OF_COUNTER-1)
#define WATCHDOG_COUNTER (NUMBER_OF_COUNTER-2)

extern byte timer_zero;         /* Semaphore for Timer Overflow   */
extern byte
ClockState[NUMBER_OF_COUNTER];      /* State of Timer                 */
extern long int lAZ;            /* Actual Value of Timer          */


/*     Ende Datentypen
**
****************************************************************************/

/****************************************************************************
**
**     Makros
*/

/****************************************************************************
*
*      Name          : stop_counter
*      Parameter     : nr    Number of Timer
*      Return Values : --
*      Description   : Stops the timer nr
*
****************************************************************************/
#define stop_counter(nr) ClockState[nr] = 0;


/*     Ende Makros
**
****************************************************************************/

/****************************************************************************
**
**     Function Prototypes
*/

/****************************************************************************
*
*      Name          : start_counter
*      Parameter     : nr    Number of timer
*      Parameter     : wert  start value of timer
*      Return Values : --
*      Description   : Starts or restarts the timer nr with the value wert
*
****************************************************************************/
void start_counter(byte nr, longword wert);

/****************************************************************************
*
*      Name          :  do_timer
*      Parameter     :  -
*      Return Values :  -
*      Description   :  Checks all activ timer for overflow
*
**************************************************************************/
void do_timer(void);


/****************************************************************************
*
*      Name          : ini_counter
*      Parameter     : anzahl  number of timers for the LLI
*      Return Values : -
*      Description   : Initializes the LLI-Timer
*
**************************************************************************/
void ini_counter(void);


/****************************************************************************
*
*      Name          : update_counter
*      Parameter     : -
*      Return Values : -
*      Description   : The Counters are updated.
*
****************************************************************************/
void update_counter(void);


/*     End Function Prototypes
**
****************************************************************************/

#endif
