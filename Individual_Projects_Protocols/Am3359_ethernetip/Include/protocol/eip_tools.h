/**
 * @file eip_tools.h
 * @brief Header file for eip_tools.c
 *
* \par
* Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
* \par
*/

#ifndef EIP_TOOLS_H_
#define EIP_TOOLS_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#include <ti/drv/icss_emac/icss_emac_ver.h>

/*TODO: USE NIMU DRV version instead of EMAC LLD*/
//#if ICSS_EMAC_DRV_VERSION_ID ==(0x01000005)
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**Starting offset for an IP header in TCP/IP packet*/
#define START_OF_IP_HEADER (14)
/**Starting offset for checksum in IP header in TCP/IP packet*/
#define START_OF_IP_CHECKSUM (START_OF_IP_HEADER + 10)
/**Starting offset for length field in IP header in TCP/IP packet*/
#define START_OF_IP_LENGTH (START_OF_IP_HEADER + 2)
/**Starting offset for UDP header in TCP/IP packet*/
#define START_OF_UDP_HEADER (34)
/**Starting offset for checksum field in UDP header in TCP/IP packet*/
#define START_OF_UDP_CHECKSUM (START_OF_UDP_HEADER + 6)
/**Starting offset for length field in UDP header in TCP/IP packet*/
#define START_OF_UDP_LENGTH  (START_OF_UDP_HEADER + 4)
/**Starting offset for protocol field in IP header in TCP/IP packet*/
#define START_OF_IP_PROTOCOL (START_OF_IP_HEADER + 9)
/**Starting offset for IP address field in IP header in TCP/IP packet*/
#define START_OF_IP_ADDRESS (START_OF_IP_HEADER + 12)
/**Starting offset for payload data in UDP header in TCP/IP packet*/
#define START_OF_PAYLOAD (START_OF_UDP_HEADER + DEFAULT_UDP_HEADER_SIZE)
/**standard size for Ethernet Header (src MAC + dst MAC + EthType) in 802.3 packet*/
#define DEFAULT_ETH_HEADER_SIZE 14
/**standard size for IP header in TCP/IP packet*/
#define DEFAULT_IP_HEADER_SIZE 20
/**standard size for UDP header in TCP/IP packet*/
#define DEFAULT_UDP_HEADER_SIZE 8
/**standard size for UDP + IP header in TCP/IP packet*/
#define DEFAULT_HEADER_SIZE  42

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
* @brief Add a word to the packet stream
* @param src pointer to the packet stream
* @param word the word which is to be added
* @return none
*/
void addWord(uint8_t *src, uint32_t word);

/**
* @brief Add a half word to the packet stream
* @param src pointer to the packet stream
* @param halfWord the 16 bit word which is to be added
* @return none
*/
void addHalfWord(volatile uint8_t *src, uint16_t halfWord);

/**
* @brief Convert specified number of bytes in source from big endian bytes to little
* endian and vice versa.
* Assumption :
* 1. Source and Destination are different
* 2. Memory is properly allocated (!Function does not check for memory overrun)
* 3. Number of bytes is even
* @param src pointer to source byte stream
* @param dst pointer to destination byte stream
* @param numBytes number of bytes to convert and copy
* @return none
*/
void convEndianess(volatile void *src, volatile void *dst, uint8_t numBytes);


/**
* @brief Compute checksum used in IP/UDP packets for a given stream
* @param packet pointer to the packet stream
* @param len length of stream
* @return Checksum computed
*/
uint32_t calcChecksum(uint8_t *packet, uint16_t len);

/**
* @brief Compute checksum for IP Header and modify in place
* @param packet pointer to the packet stream
* @return none
*/
void calcIPChecksum(uint8_t *packet);

/**
* @brief Compute checksum for UDP Header and modify in place
* @param packet pointer to the packet stream
* @return none
*/
void calcUDPChecksum(uint8_t *packet);

/**
* @brief Get the MAC Id from the network stream, convert it to little endian format and return it
* assumption : MAC is present in big endian byte format and starts from zero offset
* @param packet - byte stream
* @param macId pointer to array of six bytes which contain the MAC Id in little endian format
*/
void getMACId(uint8_t *packet, uint8_t *macId);

/**
* @brief Copy MAC ID to a stream
* @param src - pointer to stream
* @param macID pointer to MAC ID
*/
void addMACID(uint8_t *src, uint8_t *macID);

/**
* @brief Get a word from the network stream, convert it to little endian format and return it
* assumption word is present in big endian byte format and starts from zero offset
* @param byte pointer to byte stream
* @return word in little endian format
*/
uint32_t convBigEndianToLittleEndianWord(uint8_t *byte);

/**
* @brief get a halfword from the network stream, convert it to little endian format and return it
* assumption halfword is present in big endian byte format and starts from zero offset
* @param byte pointer to byte stream
* @return halfword halfword in little endian format
*/
uint16_t convBigEndianToLittleEndianHalfWord(uint8_t *byte);

/**
* @brief Takes in a 6 byte reverse byte endian source and puts it in an 64 bit double word with
* correct endianness. This function is specific to PTP
* Assumption :
* 1. Source and Destination are different
* 2. Memory is properly allocated (!Function does not check for memory overrun)
* @param src pointer to source byte stream
* @param dst pointer to destination byte stream
* @return none
*/
void convEnd6to8(volatile void *src, void *dst);
//#endif


#endif /* EIP_TOOLS_H_ */
