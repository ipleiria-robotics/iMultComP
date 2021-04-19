
#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#define OS_OK     1
#define OS_NOT_OK 0
#define OS_NULL   0

/* Ethernet Frame Maximal Length */
#define ETHERNET_MAX_FRAME_LENGTH  1518
#define MAC_ADDRESS_LEN 6

/* Maximal Data in production and consumption frame */
#define MAX_DATA_FRAME_IO          1440

/* Minimal Data in production and consumption frame */
#define MIN_DATA_FRAME_IO_RT_CLASS_1_2_3        40
#define MIN_DATA_FRAME_IO_RT_CLASS_UDP          12

#define INVALID                   0xffff

#define COUNT_OF(array) (sizeof(array)/sizeof(array[0]))

#endif
