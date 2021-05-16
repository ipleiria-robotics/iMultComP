/*---------------------------------------------------------------------------*/
/*  Copyright (C) 2006 Woodhead Software & Electonics. All rights reserved.  */
/*---------------------------------------------------------------------------*/
/*  This program is protected by international copyright.                    */
/*                                                                           */
/*  The use of this software including but not limited to its Source Code    */
/*  is subject to restrictions as agreed in the license agreement between    */
/*  you and Woodhead.                                                        */
/*  Copying or distribution is not allowed unless expressly permitted        */
/*  according to your license agreement with Woodhead.                       */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  Project             : STACK PROFINET Controller                          */
/*  Component           : RPC Interface                                      */
/* $Workfile:: uuid_pn.h                                                    $*/
/* $Revision:: 7                                                            $*/
/* $Date:: 17/10/16 9:42                                                    $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*     UUID and UUID Utility functions                                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/* From OSF DCE 1.1

Universal Unique Identifier
---------------------------
This appendix specifies the syntax and semantics of the DCE variant of
Universal Unique Identifiers (UUIDs). A UUID is an identifier that is unique
across both space and time1, with respect to the space of all UUIDs.
A UUID can be used for multiple purposes, from tagging objects with an
extremely short lifetime, to reliably identifying very persistent objects
across a network.

The generation of UUIDs does not require a registration authority for each
single identifier. Instead, it requires a unique value over space for each
UUID generator. This spatially unique value is specified as an IEEE 802
address, which is usually already applied to network-connected systems.
This 48-bit address can be assigned based on an address block obtained
through the IEEE registration authority. This UUID specification assumes
the availability of an IEEE 802 address.

Format
UUID Format shows the format of a UUID.
Field                      NDR Data Type   Octet #  Note
time_low                   unsigned long   0-3      The low field of the timestamp.
time_mid                   unsigned short  4-5      The middle field of the timestamp.
time_hi_and_version        unsigned short  6-7      The high field of the timestamp multiplexed with the version number.
clock_seq_hi_and_reserved  unsigned small  8        The high field of the clock sequence multiplexed with the variant.
clock_seq_low              unsigned small  9        The low field of the clock sequence.
node                       character       10-15    The spatially unique node identifier.


The UUID consists of a record of 16 octets and must not contain
padding between fields. The total size is 128 bits.

To minimise confusion about bit assignments within octets,
the UUID record definition is defined only in terms of fields that are integral
numbers of octets. The version number is multiplexed with the time stamp (time_high),
and the variant field is multiplexed with the clock sequence (clock_seq_high).

The timestamp is a 60 bit value. For UUID version 1,
this is represented by Coordinated Universal Time (UTC) as a count of 100-nanosecond
intervals since 00:00:00.00, 15 October 1582 (the date of Gregorian reform to the Christian calendar).

The version number is multiplexed in the 4 most significant bits of the time_hi_and_version field.
UUID version Field lists currently defined versions of the UUID.

Table: UUID version Field
msb1  msb2  msb3  msb4  Version  Description
0     0     0     1        1       DCE version, as specified herein.
0     0     1     0        2       DCE Security version, with embedded POSIX UIDs.

The variant field determines the layout of the UUID.
The structure of DCE UUIDs is fixed across different versions.
Other UUID variants may not interoperate with DCE UUIDs. Interoperability of UUIDs
is defined as the applicability of operations such as string conversion, comparison,
and lexical ordering across different systems. The variant field consists of a variable
number of the msbs of the clock_seq_hi_and_reserved field. UUID variant Field lists the
contents of the DCE variant field.

Table: UUID variant Field
msb1  msb2  msb3  Description
0     -     -      Reserved, NCS backward compatibility.
1     0     -      DCE variant.
1     1     0      Reserved, Microsoft Corporation GUID.
1     1     1      Reserved for future definition.


The clock sequence is required to detect potential losses of monotonicity of the clock.
Thus, this value marks discontinuities and prevents duplicates.
An algorithm for generating this value is outlined in Clock Sequence .
The clock sequence is encoded in the 6 least significant bits of the clock_seq_hi_and_reserved
field and in the clock_seq_low field.

The node field consists of the IEEE address, usually the host address.
For systems with multiple IEEE 802 nodes, any available node address can be used.
The lowest addressed octet (octet number 10) contains the global/local bit and the unicast/multicast bit,
and is the first octet of the address transmitted on an 802.3 LAN.

Depending on the network data representation, the multi-octet unsigned integer fields are subject to byte
swapping when communicated between different endian machines.

The nil UUID is special form of UUID that is specified to have all 128 bits set to 0 (zero).

UUID Generation
---------------------------

UUIDs are generated according to the following algorithm:

Determine the values for the UTC-based timestamp and clock sequence to be used in the UUID.
Format and Clock Sequence define how to determine these values.
For the purposes of this algorithm, consider the timestamp to be a 60-bit unsigned integer and
the clock sequence to be a 14-bit unsigned integer. Sequentially number the bits in a field,
starting from 0 (zero) for the least significant bit.

Set the time_low field equal to the least significant
32-bits (bits numbered 0 to 31 inclusive) of the time stamp in the same order of significance.
If a DCE Security version UUID is being created, then replace the time_low field with the local
user security attribute as defined by the DCE: Security Services specification.

Set the time_mid field equal to the bits numbered 32 to 47 inclusive of the time stamp
in the same order of significance.

Set the 12 least significant bits (bits numbered 0 to 11 inclusive) of the time_hi_and_version
field equal to the bits numbered 48 to 59 inclusive of the time stamp in the same order of significance.

Set the 4 most significant bits (bits numbered 12 to 15 inclusive) of the time_hi_and_version field
to the 4-bit version number corresponding to the UUID version being created, as shown in UUID version Field .

Set the clock_seq_low field to the 8 least significant bits (bits numbered 0 to 7 inclusive)
of the clock sequence in the same order of significance.

Set the 6 least significant bits (bits numbered 0 to 5 inclusive) of the clock_seq_hi_and_reserved field
to the 6 most significant bits (bits numbered 8 to 13 inclusive) of the clock sequence in the same order of significance.

Set the 2 most significant bits (bits numbered 6 and 7) of the clock_seq_hi_and_reserved
field as shown in The 2 msb of clock_seq_hi_and_reserved .

Table: The 2 msb of clock_seq_hi_and_reserved
Bit 7  Bit 6
1       0

Set the node field to the 48-bit IEEE address in the same order of significance as the address.

String Representation
---------------------------
The following is an example of the string representation of a UUID:
2fac1234-31f8-11b4-a222-08002b34c003

Comparing UUIDs
---------------------------
Field Order and Type lists the UUID fields in order of significance,
from most significant to least significant, for purposes of UUID comparison.
The table also shows the data types applicable to the fields.

Table: Field Order and Type

Field                      Type
time_low                   Unsigned 32-bit integer
time_mid                   Unsigned 16-bit integer
time_hi_and_version        Unsigned 16-bit integer
clock_seq_hi_and_reserved  Unsigned 8-bit integer
clock_seq_low              Unsigned 8-bit integer
node                       Unsigned 48-bit integer

Consider each field to be an unsigned integer as shown in Field Order and Type .
To compare a pair of UUIDs, arithmetically compare the corresponding fields from each UUID in order
of significance and according to their data type. Two UUIDs are equal if and
only if all the corresponding fields are equal. The first of two UUIDs follows the second
if the most significant field in which the UUIDs differ is greater for the first UUID.
The first of a pair of UUIDs precedes the second if the most significant field in which the UUIDs differ
is greater for the second UUID.

*/
#ifndef __UUID_PN_H__
#define __UUID_PN_H__

#define UUID4LEN  8

/*********************************************** OBJECT UUID Definition*/
/* Data1 : DWORD in little endian*/
#define UUIDDATA1NULL   0xDEA00000
#define UUIDDATA1DEVICE 0xDEA00001
#define UUIDDATA1CONTRO 0xDEA00002
#define UUIDDATA1SUPERV 0xDEA00003
#define UUIDDATA1PARSER 0xDEA00004
/* Data 2*/
#define UUIDDATA2     0x6C97

/* Data 3*/
#define UUIDDATA3     0x11D1

/* Data 4*/
#define UUIDDATA4_0   0x82
#define UUIDDATA4_1   0x71

/*---------------------------------------------------------------------------*/
/*                       STRUCTURE DEFINITION                                */
/*---------------------------------------------------------------------------*/
#include "os_pck.h"


/*
  Here the two representation of UUID DCE, PNIO
  Depending of part of code we can used both
*/

struct _ST_CLR_RPC_UUID
{
  ALIGNED_DWORD_DECL(Data1);             /*4 - 0*/
  APP_WORD   Data2;             /*2 - 4*/
  APP_WORD   Data3;             /*2 - 6*/
  APP_BYTE   Data4[UUID4LEN];   /*8 - 8*/
}STRUCT_ALIGNEMENT;             /*   16*/
typedef struct _ST_CLR_RPC_UUID PACK_ALIGNEMENT(ST_CLR_RPC_UUID), APP_FAR* LPST_CLR_RPC_UUID;
#define CLR_RPC_UUID_SIZEOF sizeof(ST_CLR_RPC_UUID)



#include "os_unpck.h"


/*---------------------------------------------------------------------------*/
/*                       FUNCTION DEFINITION                                 */
/*---------------------------------------------------------------------------*/
APP_WORD pn_uuid_from_string        (APP_PCHAR szUUID,LPST_CLR_RPC_UUID  pUUID);
APP_WORD pn_uuid_is_nil             (LPST_CLR_RPC_UUID pUUID);
APP_VOID pn_uuid_build_ar           (LPST_CLR_RPC_UUID pArUUID,APP_LPBYTE pbyMacAddress);
APP_VOID pn_uuid_build_initiator    (LPST_CLR_RPC_UUID lpuuidPROFINETIOConstantValue , LPST_CLR_RPC_UUID pInitiatorUUID, APP_WORD wCtrlDeviceId, APP_WORD wCtrlVendorId);
APP_VOID pn_uuid_build_object       (LPST_CLR_RPC_UUID lpuuidPROFINETIOConstantValue , LPST_CLR_RPC_UUID pRPCObjectUUID, APP_WORD wInstance, APP_WORD wDeviceId, APP_WORD wVendorId);
APP_VOID pn_uuid_create_activity_dce(LPST_CLR_RPC_UUID pUUID,APP_PBYTE pbyMacAddress);
APP_VOID pn_uuid_create             (LPST_CLR_RPC_UUID pUUID,APP_LPBYTE lpbyMacAddress);
APP_VOID pn_uuid_to_string          (LPST_CLR_RPC_UUID pUUID,APP_PCHAR szUUID);
APP_BOOL pn_uuid_cmp                (LPST_CLR_RPC_UUID pUUID1, LPST_CLR_RPC_UUID UUID2);
APP_VOID pn_uuid_cpy                (LPST_CLR_RPC_UUID pUUID1, LPST_CLR_RPC_UUID UUID2);
APP_VOID pn_uuid_fmx                (LPST_CLR_RPC_UUID pUUID1, LPST_CLR_RPC_UUID UUID2);
APP_WORD pn_uuid_equal              (LPST_CLR_RPC_UUID pUUID1, LPST_CLR_RPC_UUID  pUUID2);


#endif
