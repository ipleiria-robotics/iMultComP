
#include <stdint.h>
#include <pru_cfg.h>
#include <pru_intc.h>

//#include "resource_table_0.h"

// Especial register, Read and Write pin
// __R31 é o registro que trata das interrupções 
volatile register uint32_t __R30;
volatile register uint32_t __R31;


/* Defines */
#define PRU0
#define PRU1

/* PRU0-to-PRU1 interrupt */
#define PRU0_PRU1_EVT       (16)          // Interruput 16
#define PRU0_PRU1_TRIGGER   (__R31 = (PRU0_PRU1_EVT - 16) | (1 << 5)) // write Register R31 the interrupt -> 5 because the CMR4 channel was used
#define TOGGLE_BLUE     (__R30 ^= (1 << 3))   // define pin as configured in the .gel file

// define button no pino P9_27  -> corresponde ao pino pr1_pru0_pru_r30_5 
#define SW1     (1 << 5)        // 

/* INTC configuration
 *event 16 para o  Host 1
 * PRU1 irá então esperar que r31 bit 31 (designo por Host 1) vá a nivel logico 1 
 * */
void configIntc(void)
{
    /* limpar os eventos pendentes */
    __R31 = 0x00000000;

    /* Map event 16 to channel 1 CMR4 */
    /* Pag.182 do manual Pru_am335x */
    CT_INTC.CMR4_bit.CH_MAP_16 = 1; 

    /* Map channel 1 to host 1 -> HOST INTERRUPT MAP FOR CHANNEL 1 */
    CT_INTC.HMR0_bit.HINT_MAP_1 = 1; 

    /* Ensure event 16 is cleared */
    CT_INTC.SICR = 16; // 6.2.2.5 do manual 

    /* Enable event 16 */
    CT_INTC.EISR = 16; 

    /* Enable Host interrupt 1 */
    CT_INTC.HIEISR |= (1 << 0); // This enables the host interrupt output

    // Globally enable host interrupts
    CT_INTC.GER = 1; 
}

void main(void)
{
    /* Configure GPI and GPO as Mode 0 (Direct Connect)  */
    CT_CFG.GPCFG0 = 0x0000;  // 10.1.3 do manual 

    /* Clear GPO pins */
    __R30 &= 0xFFFF0000;

    /* Configure INTC */
    configIntc();

    while (1) {
        /* Wait for SW1 to be pressed */
        if ((__R31 & SW1) != SW1) {
            //TOGGLE_BLUE; 
            /* Interrupt PRU1, wait 500ms for cheap "debounce" */
            __delay_cycles(100000000);
            
            PRU0_PRU1_TRIGGER; // generate interrupt to PRU1
        }
    }
}
