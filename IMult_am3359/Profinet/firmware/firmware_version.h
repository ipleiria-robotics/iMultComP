//|                            ****                                          |**
//|                            ******o***                                    |**
//|                      ********_///_****                                   |**
//|                      ***** /_//_/ ****                                   |**
//|                       ** ** (__/ ****                                    |**
//|                           *********                                      |**
//|                            ****                                          |**
//|                            ***                                           |**
//|                                                                          |**
//|         Copyright (c) 1998-2013 Texas Instruments Incorporated           |**
//|                        ALL RIGHTS RESERVED                               |**
//|                                                                          |**
//| Permission is hereby granted to licensees of Texas Instruments           |**
//| Incorporated (TI) products to use this computer program for the sole     |**
//| purpose of implementing a licensee product based on TI products.         |**
//| No other rights to reproduce, use, or disseminate this computer          |**
//| program, whether in part or in whole, are granted.                       |**
//|                                                                          |**
//| TI makes no representation or warranties with respect to the             |**
//| performance of this computer program, and specifically disclaims         |**
//| any responsibility for any damages, special or consequential,            |**
//| connected with the use of this program.                                  |**
//|                                                                          |**
//+--------------------------------------------------------------------------+**
//*****************************************************************************/
// file:   firmware_version.hp
//
// brief:  
//			
//
//  (C) Copyright 2013, Texas Instruments, Inc
//
//	Version		Description								Author
//
// 	0.1     	Created								Thomas Mauer

#ifndef __firmware_version_h
#define __firmware_version_h 1

// ICSS_FIRMWARE_RELEASE_1: 
// bit 31..16 reserved
// bit15..8	device number
#define		FIRMWARE_DEVICE_AM335x		0	// AM335x
#define		FIRMWARE_DEVICE_AM437x		1	// AM437x
#define		FIRMWARE_DEVICE_AM571x		2	// AM571x
#define		FIRMWARE_DEVICE_AM572x		3	// AM571x
// bit7..0	protocol type
#define		FIRMWARE_PROTOCOL_TYPE_PROFIBUS_SLAVE		0x00
#define		FIRMWARE_PROTOCOL_TYPE_ETHERCAT_SLAVE		0x01
#define		FIRMWARE_PROTOCOL_TYPE_PROFINET_DEVICE		0x02
#define		FIRMWARE_PROTOCOL_TYPE_SERCOS_SLAVE			0x03
#define		FIRMWARE_PROTOCOL_TYPE_OPENMAC_SLAVE		0x04
#define		FIRMWARE_PROTOCOL_TYPE_ETHERNET 			0x05
#define		FIRMWARE_PROTOCOL_TYPE_ENETIP_SLAVE			0x06
#define		FIRMWARE_PROTOCOL_TYPE_PROFINET_IRT_DEVICE	0x07

// ICSS_FIRMWARE_RELEASE_2:
// bit31		release or internal version
#define		FIRMWARE_VERSION_RELEASE	0
#define		FIRMWARE_VERSION_INTERNAL	1
// change next for official SDK release
#define 	FIRMWARE_RELEASE_TYPE   FIRMWARE_VERSION_INTERNAL
// bit30..24	major version number
#define		FIRMWARE_VERSION_MAJOR		0x00
// bit23..16		minor version number
#define		FIRMWARE_VERSION_MINOR			0x0B
// bit15..0		build number
#define		FIRMWARE_VERSION_BUILD			0x00

#define FEATURE_MODE    0x01<<1   //MODE SWITCH
#define FEATURE_STORM_PREVENTION    0x01<<3 //ENABLED
#define FEATURE_REDUNDANCY    0x01<<4  //MRP
#define FEATURE_SYNCHRONIZATION    0x02<<8 //PTCP
#define FEATURE_NUMBER_OF_QUEUES    0x04<<12 //4 Queues
#define FEATURE_PROFINET    0x01<<16   //PROFINET SUPPORTED

#define		ICSS_FIRMWARE_RELEASE_1	((FIRMWARE_DEVICE_AM335x << 8) | (FIRMWARE_DEVICE_AM437x << 8)| (FIRMWARE_PROTOCOL_TYPE_PROFINET_IRT_DEVICE << 0))
#define		ICSS_FIRMWARE_RELEASE_2 ((FIRMWARE_RELEASE_TYPE << 31) | (FIRMWARE_VERSION_MAJOR << 24) | (FIRMWARE_VERSION_MINOR << 16) | (FIRMWARE_VERSION_BUILD << 0))
#define     ICSS_FIRMWARE_FEATURE_MASK (FEATURE_MODE| FEATURE_STORM_PREVENTION| FEATURE_REDUNDANCY|FEATURE_SYNCHRONIZATION|FEATURE_NUMBER_OF_QUEUES|FEATURE_PROFINET)
#define     ICSS_FIRMWARE_RESERVED      0x00000000
#endif
