

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
 *      -> Notas no cabeçalho do arquivos tiescutils.c
 *      -> Notas no cabeçalho do arquivos tiescbsp.c e .h *** NOTAS IMPORTANTES *****
 *
 *
 */


/*                       Profinet
 *
 *  tiesc_bscp.c :
 *      -> Include "BIN  eeprom"
 *          -> //#include <demo_tiesc_eeprom.h> // header equivalent of ESI bin file
 *           #include <Ethercat/Include_ethercat/tiesc_spi_eeprom.h>
 *      -> \mainpage PROFINET IRT API -> IRtcDrv2.h
 *      -> iRtcDrv2.c -> Core profinet driver. Contains APIs that interface directly with the Profinet stack to configure lists
 *      -> Profinet / Include / RT_MRB / platform.h ->  contem as configuraçoes GSD/TIA PORTAL
 *
 *
 *
 *
 */

int main(){
    init_peripheral();
    return(0);
}


