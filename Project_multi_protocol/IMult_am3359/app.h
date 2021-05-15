


#ifndef _APP_H_
#define _APP_H_

/* STDIO */
#include <stdbool.h>
#include <stdint.h>

/*  INCLUDE BIOS/osl */
#include <osal/OSP.h>


void init_ethercat();
uint8_t _read_word;
uint32_t _write_word;
uint32_t addr_len_eth;


void _main_profinet_();


#endif



