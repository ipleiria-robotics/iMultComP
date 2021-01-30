#include <stdint.h>
#include <pru_cfg.h>

// Register R30, Responsible for writing the status of the pins
volatile register uint32_t __R30;  

/*  Configuration Pinout 
   the pin configurations are in the .gel file, the pin (pr1_pru0_pru_r30_0), 
   (pr1_pru0_pru_r30_1), (pr1_pru0_pru_r30_2), (pr1_pru0_pru_r30_3), 
   referring to the PRU, that is, the 4 least significant bits, 
*/

int main(void)
{

    volatile uint32_t gpo;  // variable auxiliar 

    /* GPI Mode 0, GPO Mode 0 */ 
    // 
    CT_CFG.GPCFG0 = 0;

    /* Clear GPO pins */
    gpo = 0x0000;   // 

    while (1) {
        // toggle pins 
        gpo = __R30;  
        gpo ^= 0xF;                // exchange of the least significant byte
        __R30 = gpo;
        __delay_cycles(100000000); // half-second delay
    }

}
