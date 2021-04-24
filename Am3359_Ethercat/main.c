

#include "app.h"


/*                      BOARD
 *       -> board_misc.c -> Board_getDigInput
 *       ->
 * */

/*                       ETHERCAT
 *
 *  tiesc_bscp.c :
 *      -> Include "BIN  eeprom"
 *          -> //#include <demo_tiesc_eeprom.h> // header equivalent of ESI bin file
 *           #include <Ethercat/Include_ethercat/tiesc_spi_eeprom.h>
 *      -> Notas no cabe�alho do arquivos tiescutils.c
 *      -> Notas no cabe�alho do arquivos tiescbsp.c e .h *** NOTAS IMPORTANTES *****
 *
 *
 */


int main(){
    init_peripheral();

    OSAL_OS_start();
    return(0);
}


