#include <stdint.h>
#include <pru_cfg.h>

#include <pru_intc.h>


volatile register uint32_t __R30;
volatile register uint32_t __R31;

/* Defines */
#define PRU1
#define HOST1_MASK      (0x80000000)  // offset 80h -> GPIO -> global prioritized index Register 
                                      // 6.4.11- manual am335xPruReferenceGuide
#define PRU0_PRU1_EVT   (16)         // configure Event Interrupt 

// define Led, pri1_pru1_pru_r30_3 -> GPIO P8_44
#define TOGGLE_LED     (__R30 ^= (1 << 3))

void main(void)
{

    /* Configure GPI and GPO as Mode 0 (Direct Connect) */
    CT_CFG.GPCFG0 = 0x0000;

    /* Clear GPO pins */
    __R30 &= 0xFFFF0000;

    
    while (1) {
        // read register __R31, -> responsible for reading interruptions
        if (__R31 & HOST1_MASK) {
            TOGGLE_LED;
            /* Clear interrupt event */
            CT_INTC.SICR = 16; 
            /* Delay to ensure the event is cleared in INTC */
            __delay_cycles(5);
        }
    }
}

