/***************************************************************************
*
*            Copyright (c) 2016-2017 by HCC Embedded
*
* All InterNiche trademarks and copyrights are the intellectual property
* of HCC Embedded.  This software is copyrighted by and is, except where
* acknowledged, the sole property of HCC.  All rights, title, ownership,
* or other interests in the software remain the property of HCC Embedded.
*
* This software may only be used in accordance with the corresponding
* license agreement.  Any unauthorized use, duplication, transmission,
* distribution, or disclosure of this software is expressly forbidden.
*
* This Copyright notice may not be removed or modified without prior
* written consent of HCC.
*
* HCC reserves the right to modify this software without notice.
*
* HCC Embedded
* Budapest 1133
* Vaci ut 76
* Hungary
*
* Tel:  +36 (1) 450 1302
* Fax:  +36 (1) 450 1303
* http: www.hcc-embedded.com
* email: info@hcc-embedded.com
*
***************************************************************************/

/*
 * v4.01 h/snmpagt.h
 * June 11, 2013
 * Distributed per InterNiche ref US1613-2
 */
/*
 * FILENAME: snmpagt.h
 *
 * Composite header file for SNMP agent implementation.
 *
 * Copyright 2013 By InterNiche Technologies Inc. All rights reserved.
 *
 * MODULE: SNMP
 *
 * ROUTINES:
 *
 * PORTABLE: yes
 */
#include "../psp/include/psp_tick.h"
//#include "api/api_ip_app_snmp_comp.h"

#define ENTERPRISE     18868  /* InterNiche's enterprise identifier */
#define TPS               1000  /* ticks per second */
#define SUBID_32BIT        1  /* 32 bit Object ID components */
#define SNMP_32BIT_OID     1  /* Support parsing/building of 32-bit OIDs */
#define MIB_VALIDATION     1  /* validate value being set */
#define SNMP_COUNTER64     1  /* support 64-bit datatypes */

/* timer tick function (returns number of ticks elapsed since startup) */
#define USE_ARG(x)            { x = x; }

#define DEF_VARLEN 20 /* default length of oid buffers */
#define SNMP_MAX_OIDS      32       /* max number of OIDs in SET request */

#define  MAX_NAME_LEN         64 /* max number of subid's in a objid */
#define  MAX_OID_LEN          64 /* max length in bytes of an encoded oid */
#define  MAX_COMMUNITY_SIZE   32 /* max chars in a community string */

#define  SYS_STRING_MAX    256      /* max length of sys group strings */

#define  SNMPERROR(msg)    (void)(msg)   /* SNMP error routine map to nothing */

/* SNMP module return type */
typedef uint32_t  t_snmp_ret;

#define  ASN_BOOLEAN       (0x01)
#define  ASN_INTEGER       (0x02)
#define  ASN_BIT_STR       (0x03)
#define  ASN_OCTET_STR     (0x04)
#define  ASN_NULL          (0x05)
#define  ASN_OBJECT_ID     (0x06)
#define  ASN_SEQUENCE      (0x10)
#define  ASN_SET           (0x11)
#define  ASN_NSO           (0x80)  /* RFC1905:noSuchObject   */
#define  ASN_NSI           (0x81)  /* RFC1905:noSuchInstance */
#define  ASN_EOM           (0x82)  /* RFC1905:endOfMib       */

#define  ASN_UNIVERSAL     (0x00)
#define  ASN_APPLICATION   (0x40)
#define  ASN_CONTEXT       (0x80)
#define  ASN_PRIVATE       (0xC0)

#define  ASN_PRIMITIVE     (0x00)
#define  ASN_CONSTRUCTOR   (0x20)

#define  ASN_LONG_LEN      (0x80)
#define  ASN_EXTENSION_ID  (0x1F)
#define  ASN_BIT8          (0x80)

#define  IS_CONSTRUCTOR(byte) ((byte)  &  ASN_CONSTRUCTOR)
#define  IS_EXTENSION_ID(byte)   (((byte) &  ASN_EXTENSION_ID) == ASN_EXTENSION_ID)

#define SNMP_INT32_MIN_ASN1_ENC_LEN 1
#define SNMP_INT32_MAX_ASN1_ENC_LEN 5

#define  RONLY       0xAAAA   /* read access for everyone */
#define  RWRITE      0xAABA   /* add write access for community private */
#define  NOACCESS    0x0000   /* no access for anybody */
#define  READCREATE  0xBBBB   /* read,write,create access for everyone */
#define  WRITEONLY   0xCCCC   /* writeonly access for everyone */
#define  ACCESSIBLE_FOR_NOTIFY   0xDDDD   /* notify access for everyone */

#define  INTEGER     ASN_INTEGER
#define  INTEGER32   ASN_INTEGER
#define  BITS        ASN_OCTET_STR
#define  STRING      ASN_OCTET_STR
#define  OBJID       ASN_OBJECT_ID
#define  NULLOBJ     ASN_NULL

#define  SEQUENCE (ASN_SEQUENCE  |  ASN_CONSTRUCTOR)

/* defined types (from the SMI, RFC 1065) */
#define  IPADDRESS   (ASN_APPLICATION |  0)
#define  COUNTER     (ASN_APPLICATION |  1)
#define  COUNTER32   (ASN_APPLICATION |  1)
#define  GAUGE       (ASN_APPLICATION |  2)
#define  GAUGE32     (ASN_APPLICATION |  2)
#define  UNSIGNED32  (ASN_APPLICATION |  2)
#define  TIMETICKS   (ASN_APPLICATION |  3)
#ifdef OPAQUE
#undef OPAQUE
#endif
#define  OPAQUE      (ASN_APPLICATION |  4)

/*       NSAP        (ASN_APPLICATION |  5)            historic */
#define  COUNTER64   (ASN_APPLICATION |  6)
/*       UINTEGER    (ASN_APPLICATION |  7)            historic */
#define  FLOAT       (ASN_APPLICATION |  8)
#define  DOUBLE      (ASN_APPLICATION |  7)
#define  INTEGER64   (ASN_APPLICATION | 10)
#define  UNSIGNED64  (ASN_APPLICATION | 11)

#define VARTAB_OID_LEN 16 /* maximum length of an OID in variables[] table */

#ifdef MIB_VALIDATION
#define  MCT_OTHER   1
#define  MCT_INT     2
#define  MCT_STR     3

#define  MCT_RANGES  1
#define  MCT_VALUES  2

#define  MCT_MAX_VALUES 10
#endif   /* MIB_VALIDATION */


/* values for "oper", passed to var_ routines. */
#define SET_OP   -1
#define NEXT_OP  0
#define GET_OP   1

/* Set of flags for the SetParms's vbflags field */

#define SF_NOASCIICHK      0x0001  /* don't do is_us_ascii() check for this obj */
#define SF_UNSIGNEDVALUE   0x0002  /* verify that object is non-negative */
#define SF_STORAGETYPE     0x0004  /* verify values of StorageType object */
#define SF_SECURITYLEVEL   0x0008  /* verify values of SecurityLevel object */
#define SF_NOCREATION      0x0010  /* can't create new row, return noCreation */
#define SF_WRONGVALUE      0x0020  /* can't create new row, return wrongValue */

/* Set of flags for the SetParms's pduflags field */
#define SF_PDU_NEWROW      0x0001  /* new row created */


#define  SNMP_MAX_LEN              484

#define  GET_REQ_MSG               (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x0)
#define  GETNEXT_REQ_MSG           (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x1)
#define  GET_RSP_MSG               (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x2)
#define  SET_REQ_MSG               (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x3)
#define  TRP_REQ_MSG               (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x4)
#define  GETBULK_REQ_MSG           (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x5)
#define  V3_INFORM_PDU             (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x6)
#define  V3_TRAP_PDU               (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x7)
#define  V3_REPORT_PDU             (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x8)

#define  SNMP_ERR_NOERROR          (0x0)
#define  SNMP_ERR_TOOBIG           (0x1)
#define  SNMP_ERR_NOSUCHNAME       (0x2)
#define  SNMP_ERR_NOSUCH           (0x2) /* hate doing all that typing... */
#define  SNMP_ERR_BADVALUE         (0x3)
#define  SNMP_ERR_READONLY         (0x4)
#define  SNMP_ERR_GENERR           (0x5)

#define  SNMP_TRAP_COLDSTART       0
#define  SNMP_TRAP_WARMSTART       1
#define  SNMP_TRAP_LINKDOWN        2
#define  SNMP_TRAP_LINKUP          3
#define  SNMP_TRAP_AUTHFAIL        4
#define  SNMP_TRAP_EGPNEIGHBORLOSS 5
#define  SNMP_TRAP_ENTERPRISESPECIFIC  6

/* RowStatus values */
#define  SNMP_RS_INVALID            0
#define  SNMP_RS_ACTIVE             1
#define  SNMP_RS_NOTINSERVICE       2
#define  SNMP_RS_NOTREADY           3
#define  SNMP_RS_CREATEANDGO        4
#define  SNMP_RS_CREATEANDWAIT      5
#define  SNMP_RS_DESTROY            6

/* StorageType values */
#define  SNMP_ST_OTHER              1
#define  SNMP_ST_VOLATILE           2
#define  SNMP_ST_NONVOLATILE        3
#define  SNMP_ST_PERMANENT          4
#define  SNMP_ST_READONLY           5

#define  MAX_SUBID   0xFFFFFFFF
/************************************************************************
* TYPEDEFS
*************************************************************************/

/* types used by MIB compiler  */
typedef unsigned short unshort;
typedef unsigned long ulong;

typedef unsigned char u_char;
typedef unsigned short u_short;
//typedef unsigned long u_int;
typedef unsigned long u_long;

typedef unsigned long ip_addr;
typedef int socklen_t;

typedef volatile signed char     vint8_t;
typedef volatile signed short    vint16_t;
typedef volatile signed long     vint32_t;
typedef volatile unsigned char   vuint8_t;
typedef volatile unsigned short  vuint16_t;
typedef volatile unsigned long   vuint32_t;
typedef u_long   oid;
typedef u_long oid32;

#ifdef MIB_VALIDATION
struct size_info  {
   int      itype ;  /* informationType - MCT_INT, MCT_STR, MCT_OTHER */
   int      rtype ;  /* representationType - MCT_RANGES, MCT_VALUES */
   unsigned count ;  /* Number of values/ranges */
   long     values[MCT_MAX_VALUES];
};

typedef struct size_info * SIZE_INFO;
#endif   /* MIB_VALIDATION */

struct variable   {
     oid      name[VARTAB_OID_LEN]; /* object identifier of variable */
     u_char   namelen;       /* length of above */
     char     type;       /* type of variable, INTEGER or (octet) STRING */
     u_char   magic;      /* passed to function as a hint */
     u_short  acl;        /* access control list for variable */
     u_char  * (*findVar)(struct variable *, oid *, int *, int,int *);
   /* function that finds variable */
#ifdef MIB_VALIDATION
     struct size_info  ranges;  /* Set of ranges specified in MIB file */
#endif
};

struct SnmpObject
{
   oid      name[DEF_VARLEN*3];     /* object's OID  */
   unsigned namelen;                /* length of OID */
   u_char   *value;                 /* pointer to value in rcvd buffer */
   unsigned valuelen;               /* length of value */
   u_char   valuetype;              /* type of value */
};



struct SnmpObjects
{
   struct SnmpObject objs[SNMP_MAX_OIDS];
   unsigned num; /* number of objects */
};

/* variables set by var_xxxx() routines if specialized SET is required */

struct SetParms
{
   t_snmp_ret  (*access_method)(u_char *, u_char, int, u_char *, int);
   struct variable *vp;    /* pointer to variables table entry */
   oid   *name;            /* full Object ID of variable */
   int   name_len;         /* object count in "name". */
   int   do_range;         /* TRUE for range/length checking */
   long  hi_range;         /* high range for numeric sets */
   long  lo_range;         /* low  range for numeric sets */
   int   vbflags;          /* flags for special conditions like SF_NOASCIICHK
                            * cleared before processing every varbind */
   int   pduflags;         /* flags like SF_PDU_NEWROW, which are valid for PDU
                            * cleared before processing every pdu */
   struct SnmpObjects *vbinds; /* list of received varbinds in SET pdu */
#ifdef MIB_VALIDATION
   SIZE_INFO p_ranges;     /* Set of ranges specified in MIB */
#endif
};


struct enumList {
   char  *name;
   long  value;
};

/* define the datatype to hold a 64-bit counter.
 * It can be done in a variety of ways (one 64bit value, 2 32bit values,
 * 8 bytes, etc. Hence use a union to define it.
 */

typedef struct counter64
{
   uint32_t h;
   uint32_t l;  /* two 32bit values h is higher, l is lower */
}Counter64;

/************************************************************************
*  Defines section
*************************************************************************/
/* error status */
#define SNMP_ERR_STS_NOERR                 0U  /* no error */
#define SNMP_ERR_STS_TOOBIG                1U  /* too big packet */
#define SNMP_ERR_STS_NOSUCHNAME            2U  /* no such name */
#define SNMP_ERR_STS_BADVALUE              3U  /* bad value */
#define SNMP_ERR_STS_READONLY              4U  /* read only */
#define SNMP_ERR_STS_GENERR                5U  /* general error */
#define SNMP_ERR_STS_NOACCESS              6U  /* wrong no access */
#define SNMP_ERR_STS_WRTYPE                7U  /* wrong type */
#define SNMP_ERR_STS_WRLENGTH              8U  /* wrong length */
#define SNMP_ERR_STS_WRENCODING            9U  /* wrong coding */
#define SNMP_ERR_STS_WRVALUE               10U /* wrong value */
#define SNMP_ERR_STS_NOCREATION            11U /* no creation */
#define SNMP_ERR_STS_INCON_VALUE           12U /* inconsistent value */
#define SNMP_ERR_STS_RES_UNAVAILABLE       13U /* resource unavailable */
#define SNMP_ERR_STS_COMMIT_FAILED         14U /* commit failed */
#define SNMP_ERR_STS_UNDO_FAILED           15U /* undo failed */
#define SNMP_ERR_STS_AUTH_ERR              16U /* authorization error */
#define SNMP_ERR_STS_NOTWRITABLE           17U /* not writable */
#define SNMP_ERR_STS_INCON_NAME            18U /* inconsistent name */

/* return values */
#define SNMP_SUCCESS            0U        /* no error */
#define SNMP_ERROR              1U        /* error */
#define SNMP_ASN_SYNT_ERR       2U        /* message syntax error */
#define SNMP_AUTH_ERR           3U        /* authentication error */
#define SNMP_BUFF_ERR           4U        /* not enough space in send buffer */
#define SNMP_NO_SUCH_OBJECT     5U        /* No such object in MIB */
#define SNMP_NO_SUCH_INST       6U        /* No such instance in MIB */
#define SNMP_END_OF_MIB         7U        /* End of MIB view */
#define SNMP_PDU_SYNT_ERR       14U       /* message syntax error */
#define SNMP_PARAM_ERR          15U       /* Invalid parameter error */
#define SNMP_WRONG_ENG_ID       16U       /* Wrong engine ID */
#define SNMP_WRONG_UN           17U       /* Wrong user name */
#define SNMP_DROP_MSG           18U       /* Drop message */
#define SNMP_SECLEV_ERR         19U       /* Security level error */
#define SNMP_TIME_ERR           20U       /* Time window error */
#define SNMP_NO_PDUH_ERR        21U       /* No PDU handler error */
#define SNMP_ALREDY_REG_ERR     22U       /* Interface already registered error */

/* return values fro MIB status*/
#define SNMP_MIB_NOERROR          128U    /* MIB no error */
#define SNMP_MIB_TOOBIG           ( SNMP_MIB_NOERROR + SNMP_ERR_STS_TOOBIG )          /* too big packet */
#define SNMP_MIB_NOSUCHNAME       ( SNMP_MIB_NOERROR + SNMP_ERR_STS_NOSUCHNAME )      /* no such name */
#define SNMP_MIB_BADVALUE         ( SNMP_MIB_NOERROR + SNMP_ERR_STS_BADVALUE )        /* bad value */
#define SNMP_MIB_READONLY         ( SNMP_MIB_NOERROR + SNMP_ERR_STS_READONLY )        /* read only */
#define SNMP_MIB_GENERR           ( SNMP_MIB_NOERROR + SNMP_ERR_STS_GENERR )          /* general error */
#define SNMP_MIB_NO_ACCESS        ( SNMP_MIB_NOERROR + SNMP_ERR_STS_NOACCESS )        /* No access to the MIB */
#define SNMP_MIB_WRONG_TYPE       ( SNMP_MIB_NOERROR + SNMP_ERR_STS_WRTYPE )          /* wrong type of MIB object */
#define SNMP_MIB_WRONG_LENGTH     ( SNMP_MIB_NOERROR + SNMP_ERR_STS_WRLENGTH )        /* wrong length of MIB object */
#define SNMP_MIB_WRENCODING       ( SNMP_MIB_NOERROR + SNMP_ERR_STS_WRENCODING )      /* wrong coding */
#define SNMP_MIB_WRVALUE          ( SNMP_MIB_NOERROR + SNMP_ERR_STS_WRVALUE )         /* wrong value */
#define SNMP_MIB_NOCREATION       ( SNMP_MIB_NOERROR + SNMP_ERR_STS_NOCREATION )      /* no creation */
#define SNMP_MIB_INCON_VALUE      ( SNMP_MIB_NOERROR + SNMP_ERR_STS_INCON_VALUE )     /* inconsistent value */
#define SNMP_MIB_RES_UNAVAILABLE  ( SNMP_MIB_NOERROR + SNMP_ERR_STS_RES_UNAVAILABLE ) /* resource unavailable */
#define SNMP_MIB_COMMIT_FAILED    ( SNMP_MIB_NOERROR + SNMP_ERR_STS_COMMIT_FAILED )   /* commit failed */
#define SNMP_MIB_UNDO_FAILED      ( SNMP_MIB_NOERROR + SNMP_ERR_STS_UNDO_FAILED )     /* undo failed */
#define SNMP_MIB_AUTH_ERR         ( SNMP_MIB_NOERROR + SNMP_ERR_STS_AUTH_ERR )        /* authorization error */
#define SNMP_MIB_NOTWRITABLE      ( SNMP_MIB_NOERROR + SNMP_ERR_STS_NOTWRITABLE )     /* not writable */
#define SNMP_MIB_INCON_NAME       ( SNMP_MIB_NOERROR + SNMP_ERR_STS_INCON_NAME )      /* inconsistent name */



int32_t snmp_oid_compare( oid * name1
                         , uint32_t len1
                         , oid * name2
                         , uint32_t len2 );
t_snmp_ret snmp_comp_parse_int( uint8_t   * p_var_val
                               , uint16_t  var_len
                               , uint8_t   var_val_type
                               , int32_t *  p_out_data );
t_snmp_ret snmp_comp_parse_bits( uint8_t   * p_var_val
                                , uint16_t  var_len
                                , uint8_t   var_val_type
                                , uint8_t *  p_out_data
                                , uint16_t   out_len );
t_snmp_ret snmp_comp_parse_string( uint8_t   * p_var_val
                                  , uint16_t  var_len
                                  , uint8_t   var_val_type
                                  , uint8_t *  p_out_data
                                  , uint16_t * p_out_len );
t_snmp_ret snmp_comp_parse_objid( uint8_t   * p_var_val
                                 , uint16_t  var_len
                                 , uint8_t   var_val_type
                                 , oid *  p_out_data
                                 , uint16_t  * p_out_len );
t_snmp_ret snmp_comp_parse_counter64( uint8_t   * p_var_val
                                     , uint16_t  var_len
                                     , uint8_t   var_val_type
                                     , Counter64 *  p_out_data );
t_snmp_ret snmp_comp_set_variable ( uint8_t * p_var_val
                                   , uint16_t var_len
                                   , uint8_t var_val_type
                                   , uint8_t * p_out_data
                                   , uint16_t out_len );
t_snmp_ret snmp_comp_update_rowstatus( uint8_t * cur_status
                                      , uint8_t new_status );

/************************************************************************
* EXTERNAL VARIABLES
*************************************************************************/
extern struct SetParms   set_parms; /* set parameters structure */

/******* start of MIB variables *******/

/* tokens for 'system' group */
#define SYSDESCR    0
#define SYSOBJECTID SYSDESCR+4
#define SYSUPTIME   SYSOBJECTID+4
#define SYSCONTACT  SYSUPTIME+4
#define SYSNAME     SYSCONTACT+4
#define SYSLOCATION SYSNAME+4
#define SYSSERVICES SYSLOCATION+4

/* tokens for 'interfaces' group */
#define IFNUMBER    0

/* tokens for 'ifEntry' group */
#define IFINDEX 0
#define IFDESCR IFINDEX+4
#define IFTYPE  IFDESCR+4
#define IFMTU   IFTYPE+4
#define IFSPEED IFMTU+4
#define IFPHYSADDRESS   IFSPEED+4
#define IFADMINSTATUS   IFPHYSADDRESS+4
#define IFOPERSTATUS    IFADMINSTATUS+4
#define IFLASTCHANGE    IFOPERSTATUS+4
#define IFINOCTETS  IFLASTCHANGE+4
#define IFINUCASTPKTS   IFINOCTETS+4
#define IFINNUCASTPKTS  IFINUCASTPKTS+4
#define IFINDISCARDS    IFINNUCASTPKTS+4
#define IFINERRORS  IFINDISCARDS+4
#define IFINUNKNOWNPROTOS   IFINERRORS+4
#define IFOUTOCTETS IFINUNKNOWNPROTOS+4
#define IFOUTUCASTPKTS  IFOUTOCTETS+4
#define IFOUTNUCASTPKTS IFOUTUCASTPKTS+4
#define IFOUTDISCARDS   IFOUTNUCASTPKTS+4
#define IFOUTERRORS IFOUTDISCARDS+4
#define IFOUTQLEN   IFOUTERRORS+4
#define IFSPECIFIC  IFOUTQLEN+4

/* tokens for 'snmpEngine' group */
#define SNMPENGINEID    0
#define SNMPENGINEBOOTS SNMPENGINEID+4
#define SNMPENGINETIME  SNMPENGINEBOOTS+4
#define SNMPENGINEMAXMESSAGESIZE    SNMPENGINETIME+4
/* tokens for 'lreConfigurationGeneralGroup' group */
#define LREMANUFACTURERNAME 0
#define LREINTERFACECOUNT   LREMANUFACTURERNAME+4

/* tokens for 'lreInterfaceConfigEntry' group */
#define LREINTERFACECONFIGINDEX 0
#define LREROWSTATUS    LREINTERFACECONFIGINDEX+4
#define LRENODETYPE LREROWSTATUS+4
#define LRENODENAME LRENODETYPE+4
#define LREVERSIONNAME  LRENODENAME+4
#define LREMACADDRESS   LREVERSIONNAME+4
#define LREPORTADMINSTATEA  LREMACADDRESS+4
#define LREPORTADMINSTATEB  LREPORTADMINSTATEA+4
#define LRELINKSTATUSA  LREPORTADMINSTATEB+4
#define LRELINKSTATUSB  LRELINKSTATUSA+4
#define LREDUPLICATEDISCARD LRELINKSTATUSB+4
#define LRETRANSPARENTRECEPTION LREDUPLICATEDISCARD+4
#define LREHSRLREMODE   LRETRANSPARENTRECEPTION+4
#define LRESWITCHINGENDNODE LREHSRLREMODE+4
#define LREREDBOXIDENTITY   LRESWITCHINGENDNODE+4
#define LREEVALUATESUPERVISION  LREREDBOXIDENTITY+4
#define LRENODESTABLECLEAR  LREEVALUATESUPERVISION+4
#define LREPROXYNODETABLECLEAR  LRENODESTABLECLEAR+4

/* tokens for 'lreInterfaceStatsEntry' group */
#define LREINTERFACESTATSINDEX  0
#define LRECNTTXA   LREINTERFACESTATSINDEX+4
#define LRECNTTXB   LRECNTTXA+4
#define LRECNTTXC   LRECNTTXB+4
#define LRECNTERRWRONGLANA  LRECNTTXC+4
#define LRECNTERRWRONGLANB  LRECNTERRWRONGLANA+4
#define LRECNTERRWRONGLANC  LRECNTERRWRONGLANB+4
#define LRECNTRXA   LRECNTERRWRONGLANC+4
#define LRECNTRXB   LRECNTRXA+4
#define LRECNTRXC   LRECNTRXB+4
#define LRECNTERRORSA   LRECNTRXC+4
#define LRECNTERRORSB   LRECNTERRORSA+4
#define LRECNTERRORSC   LRECNTERRORSB+4
#define LRECNTNODES LRECNTERRORSC+4
#define LRECNTPROXYNODES    LRECNTNODES+4
#define LRECNTUNIQUEA   LRECNTPROXYNODES+4
#define LRECNTUNIQUEB   LRECNTUNIQUEA+4
#define LRECNTUNIQUEC   LRECNTUNIQUEB+4
#define LRECNTDUPLICATEA    LRECNTUNIQUEC+4
#define LRECNTDUPLICATEB    LRECNTDUPLICATEA+4
#define LRECNTDUPLICATEC    LRECNTDUPLICATEB+4
#define LRECNTMULTIA    LRECNTDUPLICATEC+4
#define LRECNTMULTIB    LRECNTMULTIA+4
#define LRECNTMULTIC    LRECNTMULTIB+4
#define LRECNTOWNRXA    LRECNTMULTIC+4
#define LRECNTOWNRXB    LRECNTOWNRXA+4

/* tokens for 'lreNodesEntry' group */
#define LRENODESINDEX   0
#define LRENODESMACADDRESS  LRENODESINDEX+4
#define LRETIMELASTSEENA    LRENODESMACADDRESS+4
#define LRETIMELASTSEENB    LRETIMELASTSEENA+4
#define LREREMNODETYPE  LRETIMELASTSEENB+4

/* tokens for 'lreProxyNodeEntry' group */
#define LREPROXYNODEINDEX   0
#define LREPROXYNODEMACADDRESS  LREPROXYNODEINDEX+4

/* tokens for 'lldpConfiguration' group */
#define LLDPMESSAGETXINTERVAL   0
#define LLDPMESSAGETXHOLDMULTIPLIER LLDPMESSAGETXINTERVAL+4
#define LLDPREINITDELAY LLDPMESSAGETXHOLDMULTIPLIER+4
#define LLDPTXDELAY LLDPREINITDELAY+4
#define LLDPNOTIFICATIONINTERVAL    LLDPTXDELAY+4

/* tokens for 'lldpPortConfigEntry' group */
#define LLDPPORTCONFIGPORTNUM   0
#define LLDPPORTCONFIGADMINSTATUS   LLDPPORTCONFIGPORTNUM+4
#define LLDPPORTCONFIGNOTIFICATIONENABLE    LLDPPORTCONFIGADMINSTATUS+4
#define LLDPPORTCONFIGTLVSTXENABLE  LLDPPORTCONFIGNOTIFICATIONENABLE+4

/* tokens for 'lldpConfigManAddrEntry' group */
#define LLDPCONFIGMANADDRPORTSTXENABLE  0

/* tokens for 'lldpStatistics' group */
#define LLDPSTATSREMTABLESLASTCHANGETIME    0
#define LLDPSTATSREMTABLESINSERTS   LLDPSTATSREMTABLESLASTCHANGETIME+4
#define LLDPSTATSREMTABLESDELETES   LLDPSTATSREMTABLESINSERTS+4
#define LLDPSTATSREMTABLESDROPS LLDPSTATSREMTABLESDELETES+4
#define LLDPSTATSREMTABLESAGEOUTS   LLDPSTATSREMTABLESDROPS+4

/* tokens for 'lldpStatsTxPortEntry' group */
#define LLDPSTATSTXPORTNUM  0
#define LLDPSTATSTXPORTFRAMESTOTAL  LLDPSTATSTXPORTNUM+4

/* tokens for 'lldpStatsRxPortEntry' group */
#define LLDPSTATSRXPORTNUM  0
#define LLDPSTATSRXPORTFRAMESDISCARDEDTOTAL LLDPSTATSRXPORTNUM+4
#define LLDPSTATSRXPORTFRAMESERRORS LLDPSTATSRXPORTFRAMESDISCARDEDTOTAL+4
#define LLDPSTATSRXPORTFRAMESTOTAL  LLDPSTATSRXPORTFRAMESERRORS+4
#define LLDPSTATSRXPORTTLVSDISCARDEDTOTAL   LLDPSTATSRXPORTFRAMESTOTAL+4
#define LLDPSTATSRXPORTTLVSUNRECOGNIZEDTOTAL    LLDPSTATSRXPORTTLVSDISCARDEDTOTAL+4
#define LLDPSTATSRXPORTAGEOUTSTOTAL LLDPSTATSRXPORTTLVSUNRECOGNIZEDTOTAL+4

/* tokens for 'lldpLocalSystemData' group */
#define LLDPLOCCHASSISIDSUBTYPE 0
#define LLDPLOCCHASSISID    LLDPLOCCHASSISIDSUBTYPE+4
#define LLDPLOCSYSNAME  LLDPLOCCHASSISID+4
#define LLDPLOCSYSDESC  LLDPLOCSYSNAME+4
#define LLDPLOCSYSCAPSUPPORTED  LLDPLOCSYSDESC+4
#define LLDPLOCSYSCAPENABLED    LLDPLOCSYSCAPSUPPORTED+4

/* tokens for 'lldpLocPortEntry' group */
#define LLDPLOCPORTNUM  0
#define LLDPLOCPORTIDSUBTYPE    LLDPLOCPORTNUM+4
#define LLDPLOCPORTID   LLDPLOCPORTIDSUBTYPE+4
#define LLDPLOCPORTDESC LLDPLOCPORTID+4

/* tokens for 'lldpLocManAddrEntry' group */
#define LLDPLOCMANADDRSUBTYPE   0
#define LLDPLOCMANADDR  LLDPLOCMANADDRSUBTYPE+4
#define LLDPLOCMANADDRLEN   LLDPLOCMANADDR+4
#define LLDPLOCMANADDRIFSUBTYPE LLDPLOCMANADDRLEN+4
#define LLDPLOCMANADDRIFID  LLDPLOCMANADDRIFSUBTYPE+4
#define LLDPLOCMANADDROID   LLDPLOCMANADDRIFID+4

/* tokens for 'lldpRemEntry' group */
#define LLDPREMTIMEMARK 0
#define LLDPREMLOCALPORTNUM LLDPREMTIMEMARK+4
#define LLDPREMINDEX    LLDPREMLOCALPORTNUM+4
#define LLDPREMCHASSISIDSUBTYPE LLDPREMINDEX+4
#define LLDPREMCHASSISID    LLDPREMCHASSISIDSUBTYPE+4
#define LLDPREMPORTIDSUBTYPE    LLDPREMCHASSISID+4
#define LLDPREMPORTID   LLDPREMPORTIDSUBTYPE+4
#define LLDPREMPORTDESC LLDPREMPORTID+4
#define LLDPREMSYSNAME  LLDPREMPORTDESC+4
#define LLDPREMSYSDESC  LLDPREMSYSNAME+4
#define LLDPREMSYSCAPSUPPORTED  LLDPREMSYSDESC+4
#define LLDPREMSYSCAPENABLED    LLDPREMSYSCAPSUPPORTED+4

/* tokens for 'lldpRemManAddrEntry' group */
#define LLDPREMMANADDRSUBTYPE   0
#define LLDPREMMANADDR  LLDPREMMANADDRSUBTYPE+4
#define LLDPREMMANADDRIFSUBTYPE LLDPREMMANADDR+4
#define LLDPREMMANADDRIFID  LLDPREMMANADDRIFSUBTYPE+4
#define LLDPREMMANADDROID   LLDPREMMANADDRIFID+4

/* tokens for 'lldpRemUnknownTLVEntry' group */
#define LLDPREMUNKNOWNTLVTYPE   0
#define LLDPREMUNKNOWNTLVINFO   LLDPREMUNKNOWNTLVTYPE+4

/* tokens for 'lldpRemOrgDefInfoEntry' group */
#define LLDPREMORGDEFINFOOUI    0
#define LLDPREMORGDEFINFOSUBTYPE    LLDPREMORGDEFINFOOUI+4
#define LLDPREMORGDEFINFOINDEX  LLDPREMORGDEFINFOSUBTYPE+4
#define LLDPREMORGDEFINFO   LLDPREMORGDEFINFOINDEX+4

/* tokens for 'lldpXPnoConfigEntry' group */
#define LLDPXPNOCONFIGSPDTXENABLE   0
#define LLDPXPNOCONFIGPORTSTATUSTXENABLE    LLDPXPNOCONFIGSPDTXENABLE+4
#define LLDPXPNOCONFIGALIASTXENABLE LLDPXPNOCONFIGPORTSTATUSTXENABLE+4
#define LLDPXPNOCONFIGMRPTXENABLE   LLDPXPNOCONFIGALIASTXENABLE+4
#define LLDPXPNOCONFIGPTCPTXENABLE  LLDPXPNOCONFIGMRPTXENABLE+4

/* tokens for 'lldpXPnoLocEntry' group */
#define LLDPXPNOLOCLPDVALUE 0
#define LLDPXPNOLOCPORTTXDVALUE LLDPXPNOLOCLPDVALUE+4
#define LLDPXPNOLOCPORTRXDVALUE LLDPXPNOLOCPORTTXDVALUE+4
#define LLDPXPNOLOCPORTSTATUSRT2    LLDPXPNOLOCPORTRXDVALUE+4
#define LLDPXPNOLOCPORTSTATUSRT3    LLDPXPNOLOCPORTSTATUSRT2+4
#define LLDPXPNOLOCPORTNOS  LLDPXPNOLOCPORTSTATUSRT3+4
#define LLDPXPNOLOCPORTMRPUUID  LLDPXPNOLOCPORTNOS+4
#define LLDPXPNOLOCPORTMRRTSTATUS   LLDPXPNOLOCPORTMRPUUID+4
#define LLDPXPNOLOCPORTPTCPMASTER   LLDPXPNOLOCPORTMRRTSTATUS+4
#define LLDPXPNOLOCPORTPTCPSUBDOMAINUUID    LLDPXPNOLOCPORTPTCPMASTER+4
#define LLDPXPNOLOCPORTPTCPIRDATAUUID   LLDPXPNOLOCPORTPTCPSUBDOMAINUUID+4
#define LLDPXPNOLOCPORTMODERT3  LLDPXPNOLOCPORTPTCPIRDATAUUID+4
#define LLDPXPNOLOCPORTPERIODLENGTH LLDPXPNOLOCPORTMODERT3+4
#define LLDPXPNOLOCPORTPERIODVALIDITY   LLDPXPNOLOCPORTPERIODLENGTH+4
#define LLDPXPNOLOCPORTREDOFFSET    LLDPXPNOLOCPORTPERIODVALIDITY+4
#define LLDPXPNOLOCPORTREDVALIDITY  LLDPXPNOLOCPORTREDOFFSET+4
#define LLDPXPNOLOCPORTORANGEOFFSET LLDPXPNOLOCPORTREDVALIDITY+4
#define LLDPXPNOLOCPORTORANGEVALIDITY   LLDPXPNOLOCPORTORANGEOFFSET+4
#define LLDPXPNOLOCPORTGREENOFFSET  LLDPXPNOLOCPORTORANGEVALIDITY+4
#define LLDPXPNOLOCPORTGREENVALIDITY    LLDPXPNOLOCPORTGREENOFFSET+4
#define LLDPXPNOLOCPORTSTATUSRT3OPTIMIZATIONSUPPORTED   LLDPXPNOLOCPORTGREENVALIDITY+4
#define LLDPXPNOLOCPORTSTATUSRT3PREAMBLESHORTENINGSUPPORTED LLDPXPNOLOCPORTSTATUSRT3OPTIMIZATIONSUPPORTED+4
#define LLDPXPNOLOCPORTSTATUSRT3PREAMBLESHORTENING  LLDPXPNOLOCPORTSTATUSRT3PREAMBLESHORTENINGSUPPORTED+4
#define LLDPXPNOLOCPORTSTATUSRT3FRAGMENTATIONSUPPORTED  LLDPXPNOLOCPORTSTATUSRT3PREAMBLESHORTENING+4
#define LLDPXPNOLOCPORTSTATUSRT3FRAGMENTATION   LLDPXPNOLOCPORTSTATUSRT3FRAGMENTATIONSUPPORTED+4

/* tokens for 'lldpXPnoRemEntry' group */
#define LLDPXPNOREMLPDVALUE 0
#define LLDPXPNOREMPORTTXDVALUE LLDPXPNOREMLPDVALUE+4
#define LLDPXPNOREMPORTRXDVALUE LLDPXPNOREMPORTTXDVALUE+4
#define LLDPXPNOREMPORTSTATUSRT2    LLDPXPNOREMPORTRXDVALUE+4
#define LLDPXPNOREMPORTSTATUSRT3    LLDPXPNOREMPORTSTATUSRT2+4
#define LLDPXPNOREMPORTNOS  LLDPXPNOREMPORTSTATUSRT3+4
#define LLDPXPNOREMPORTMRPUUID  LLDPXPNOREMPORTNOS+4
#define LLDPXPNOREMPORTMRRTSTATUS   LLDPXPNOREMPORTMRPUUID+4
#define LLDPXPNOREMPORTPTCPMASTER   LLDPXPNOREMPORTMRRTSTATUS+4
#define LLDPXPNOREMPORTPTCPSUBDOMAINUUID    LLDPXPNOREMPORTPTCPMASTER+4
#define LLDPXPNOREMPORTPTCPIRDATAUUID   LLDPXPNOREMPORTPTCPSUBDOMAINUUID+4
#define LLDPXPNOREMPORTMODERT3  LLDPXPNOREMPORTPTCPIRDATAUUID+4
#define LLDPXPNOREMPORTPERIODLENGTH LLDPXPNOREMPORTMODERT3+4
#define LLDPXPNOREMPORTPERIODVALIDITY   LLDPXPNOREMPORTPERIODLENGTH+4
#define LLDPXPNOREMPORTREDOFFSET    LLDPXPNOREMPORTPERIODVALIDITY+4
#define LLDPXPNOREMPORTREDVALIDITY  LLDPXPNOREMPORTREDOFFSET+4
#define LLDPXPNOREMPORTORANGEOFFSET LLDPXPNOREMPORTREDVALIDITY+4
#define LLDPXPNOREMPORTORANGEVALIDITY   LLDPXPNOREMPORTORANGEOFFSET+4
#define LLDPXPNOREMPORTGREENOFFSET  LLDPXPNOREMPORTORANGEVALIDITY+4
#define LLDPXPNOREMPORTGREENVALIDITY    LLDPXPNOREMPORTGREENOFFSET+4
#define LLDPXPNOREMPORTSTATUSRT3PREAMBLESHORTENING  LLDPXPNOREMPORTGREENVALIDITY+4
#define LLDPXPNOREMPORTSTATUSRT3FRAGMENTATION   LLDPXPNOREMPORTSTATUSRT3PREAMBLESHORTENING+4

/* tokens for 'lldpXdot3PortConfigEntry' group */
#define LLDPXDOT3PORTCONFIGTLVSTXENABLE 0

/* tokens for 'lldpXdot3LocPortEntry' group */
#define LLDPXDOT3LOCPORTAUTONEGSUPPORTED    0
#define LLDPXDOT3LOCPORTAUTONEGENABLED  LLDPXDOT3LOCPORTAUTONEGSUPPORTED+4
#define LLDPXDOT3LOCPORTAUTONEGADVERTISEDCAP    LLDPXDOT3LOCPORTAUTONEGENABLED+4
#define LLDPXDOT3LOCPORTOPERMAUTYPE LLDPXDOT3LOCPORTAUTONEGADVERTISEDCAP+4

/* tokens for 'lldpXdot3LocPowerEntry' group */
#define LLDPXDOT3LOCPOWERPORTCLASS  0
#define LLDPXDOT3LOCPOWERMDISUPPORTED   LLDPXDOT3LOCPOWERPORTCLASS+4
#define LLDPXDOT3LOCPOWERMDIENABLED LLDPXDOT3LOCPOWERMDISUPPORTED+4
#define LLDPXDOT3LOCPOWERPAIRCONTROLABLE    LLDPXDOT3LOCPOWERMDIENABLED+4
#define LLDPXDOT3LOCPOWERPAIRS  LLDPXDOT3LOCPOWERPAIRCONTROLABLE+4
#define LLDPXDOT3LOCPOWERCLASS  LLDPXDOT3LOCPOWERPAIRS+4

/* tokens for 'lldpXdot3LocLinkAggEntry' group */
#define LLDPXDOT3LOCLINKAGGSTATUS   0
#define LLDPXDOT3LOCLINKAGGPORTID   LLDPXDOT3LOCLINKAGGSTATUS+4

/* tokens for 'lldpXdot3LocMaxFrameSizeEntry' group */
#define LLDPXDOT3LOCMAXFRAMESIZE    0

/* tokens for 'lldpXdot3RemPortEntry' group */
#define LLDPXDOT3REMPORTAUTONEGSUPPORTED    0
#define LLDPXDOT3REMPORTAUTONEGENABLED  LLDPXDOT3REMPORTAUTONEGSUPPORTED+4
#define LLDPXDOT3REMPORTAUTONEGADVERTISEDCAP    LLDPXDOT3REMPORTAUTONEGENABLED+4
#define LLDPXDOT3REMPORTOPERMAUTYPE LLDPXDOT3REMPORTAUTONEGADVERTISEDCAP+4

/* tokens for 'lldpXdot3RemPowerEntry' group */
#define LLDPXDOT3REMPOWERPORTCLASS  0
#define LLDPXDOT3REMPOWERMDISUPPORTED   LLDPXDOT3REMPOWERPORTCLASS+4
#define LLDPXDOT3REMPOWERMDIENABLED LLDPXDOT3REMPOWERMDISUPPORTED+4
#define LLDPXDOT3REMPOWERPAIRCONTROLABLE    LLDPXDOT3REMPOWERMDIENABLED+4
#define LLDPXDOT3REMPOWERPAIRS  LLDPXDOT3REMPOWERPAIRCONTROLABLE+4
#define LLDPXDOT3REMPOWERCLASS  LLDPXDOT3REMPOWERPAIRS+4

/* tokens for 'lldpXdot3RemLinkAggEntry' group */
#define LLDPXDOT3REMLINKAGGSTATUS   0
#define LLDPXDOT3REMLINKAGGPORTID   LLDPXDOT3REMLINKAGGSTATUS+4

/* tokens for 'lldpXdot3RemMaxFrameSizeEntry' group */
#define LLDPXDOT3REMMAXFRAMESIZE    0
/* tokens for 'lldpXdot1ConfigPortVlanEntry' group */
#define LLDPXDOT1CONFIGPORTVLANTXENABLE 0

/* tokens for 'lldpXdot1ConfigVlanNameEntry' group */
#define LLDPXDOT1CONFIGVLANNAMETXENABLE 0

/* tokens for 'lldpXdot1ConfigProtoVlanEntry' group */
#define LLDPXDOT1CONFIGPROTOVLANTXENABLE    0

/* tokens for 'lldpXdot1ConfigProtocolEntry' group */
#define LLDPXDOT1CONFIGPROTOCOLTXENABLE 0

/* tokens for 'lldpXdot1LocEntry' group */
#define LLDPXDOT1LOCPORTVLANID  0

/* tokens for 'lldpXdot1LocProtoVlanEntry' group */
#define LLDPXDOT1LOCPROTOVLANID 0
#define LLDPXDOT1LOCPROTOVLANSUPPORTED  LLDPXDOT1LOCPROTOVLANID+4
#define LLDPXDOT1LOCPROTOVLANENABLED    LLDPXDOT1LOCPROTOVLANSUPPORTED+4

/* tokens for 'lldpXdot1LocVlanNameEntry' group */
#define LLDPXDOT1LOCVLANID  0
#define LLDPXDOT1LOCVLANNAME    LLDPXDOT1LOCVLANID+4

/* tokens for 'lldpXdot1LocProtocolEntry' group */
#define LLDPXDOT1LOCPROTOCOLINDEX   0
#define LLDPXDOT1LOCPROTOCOLID  LLDPXDOT1LOCPROTOCOLINDEX+4

/* tokens for 'lldpXdot1RemEntry' group */
#define LLDPXDOT1REMPORTVLANID  0

/* tokens for 'lldpXdot1RemProtoVlanEntry' group */
#define LLDPXDOT1REMPROTOVLANID 0
#define LLDPXDOT1REMPROTOVLANSUPPORTED  LLDPXDOT1REMPROTOVLANID+4
#define LLDPXDOT1REMPROTOVLANENABLED    LLDPXDOT1REMPROTOVLANSUPPORTED+4

/* tokens for 'lldpXdot1RemVlanNameEntry' group */
#define LLDPXDOT1REMVLANID  0
#define LLDPXDOT1REMVLANNAME    LLDPXDOT1REMVLANID+4

/* tokens for 'lldpXdot1RemProtocolEntry' group */
#define LLDPXDOT1REMPROTOCOLINDEX   0
#define LLDPXDOT1REMPROTOCOLID  LLDPXDOT1REMPROTOCOLINDEX+4

/* max length of sys group strings */
#define  SYS_STRING_MAX    256

/* Global SNMP variables */
extern char sys_descr[SYS_STRING_MAX]; /* define in application */
extern char sysContact[SYS_STRING_MAX];
extern char sysName[SYS_STRING_MAX];
extern char sysLocation[SYS_STRING_MAX];

/******* end of MIB variables *******/

/* prototypes for "var_" routines in variables table.
Simply delete any that aren't used */

u_char *
var_system(struct variable * vp, 
    oid* name, int * len, int exact, 
    int * var_len);


u_char *
var_interfaces(struct variable * vp, 
    oid* name, int * len, int exact, 
    int * var_len);

u_char *
var_ifEntry(struct variable * vp, 
    oid* name, int * len, int exact, 
    int * var_len);

u_char *
var_snmpEngine(struct variable * vp, 
    oid* name, int * len, int exact, 
    int * var_len);

u_char *
var_lldpConfiguration(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpPortConfigEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpConfigManAddrEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpStatistics(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpStatsTxPortEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpStatsRxPortEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpLocalSystemData(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpLocPortEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpLocManAddrEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpRemEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpRemManAddrEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpRemUnknownTLVEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpRemOrgDefInfoEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXPnoConfigEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXPnoLocEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXPnoRemEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot3PortConfigEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot3LocPortEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot3LocPowerEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot3LocLinkAggEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot3LocMaxFrameSizeEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot3RemPortEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot3RemPowerEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot3RemLinkAggEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot3RemMaxFrameSizeEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);



    u_char *
var_lldpXdot1ConfigPortVlanEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot1ConfigVlanNameEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot1ConfigProtoVlanEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot1ConfigProtocolEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot1LocEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot1LocProtoVlanEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot1LocVlanNameEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot1LocProtocolEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot1RemEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot1RemProtoVlanEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot1RemVlanNameEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lldpXdot1RemProtocolEntry(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len);

u_char *
var_lreConfigurationGeneralGroup(
    struct variable *vp,   /* IN - pointer to variables[] entry */
    oid   *name,    /* IN/OUT - input name requested; output name found */
    int   *length,    /* IN/OUT - length of input & output oids */
    int  oper,    /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int   *var_len  /* OUT - length of variable, or 0 if function */
);

u_char *
var_lreInterfaceConfigEntry
(
    struct variable *vp,      /* IN - pointer to variables[] entry */
    oid             *name,    /* IN/OUT - input name requested; output name found */
    int             *length,  /* IN/OUT - length of input & output oids */
    int              oper,    /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int             *var_len  /* OUT - length of variable, or 0 if function */
);

u_char *
var_lreInterfaceStatsEntry
(
    struct variable *vp,      /* IN - pointer to variables[] entry */
    oid             *name,    /* IN/OUT - input name requested; output name found */
    int             *length,  /* IN/OUT - length of input & output oids */
    int             oper,     /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int             *var_len  /* OUT - length of variable, or 0 if function */
);

u_char *
var_lreNodesEntry
(
    struct variable *vp,     /* IN - pointer to variables[] entry */
    oid             *name,   /* IN/OUT - input name requested; output name found */
    int             *length, /* IN/OUT - length of input & output oids */
    int              oper,   /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int             *var_len /* OUT - length of variable, or 0 if function */
);

u_char *
var_lreProxyNodeEntry(
    struct variable *vp,      /* IN - pointer to variables[] entry */
    oid             *name,    /* IN/OUT - input name requested; output name found */
    int             *length,  /* IN/OUT - length of input & output oids */
    int              oper,    /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int             *var_len  /* OUT - length of variable, or 0 if function */
);


/* Set of flags for the SetParms's vbflags field */
#define SF_NOASCIICHK      0x0001  /* don't do is_us_ascii() check for this obj */
#define SF_UNSIGNEDVALUE   0x0002  /* verify that object is non-negative */
#define SF_STORAGETYPE     0x0004  /* verify values of StorageType object */
#define SF_SECURITYLEVEL   0x0008  /* verify values of SecurityLevel object */
#define SF_NOCREATION      0x0010  /* can't create new row, return noCreation */
#define SF_WRONGVALUE      0x0020  /* can't create new row, return wrongValue */
/* Set of flags for the SetParms's pduflags field */
#define SF_PDU_NEWROW      0x0001  /* new row created */

extern struct SetParms set_parms;  /* single static copy of set parms */

/* function prototypes */
#define compare(p_name1, len1,p_name2, len2) snmp_oid_compare(p_name1, len1,p_name2, len2)

int setTestAndIncr(u_char *var_val, u_char var_val_type, int var_val_len, u_char *statP, int statLen);
void oid2bytes(char* octetbuf, oid *oidbuf, int count);
int snmp_update_rowstatus(int *cur_status, int new_status);

#ifndef oidcpy
#define oidcpy(dest,src,len) memcpy((char*)dest,(char*)src,((int)(len)*sizeof(oid)))
#endif

#define SNMP_ERR_NOERROR          (0x0)
#define SNMP_ERR_TOOBIG           (0x1)
#define SNMP_ERR_NOSUCHNAME       (0x2)
#define SNMP_ERR_BADVALUE         (0x3)
#define SNMP_ERR_READONLY         (0x4)
#define SNMP_ERR_GENERR           (0x5)

/* values of various error codes as defined in RFC 1905 */
#define V3_VB_NOERROR              0
#define V3_VB_TOOBIG               1
#define V3_VB_NOSUCHNAME           2
#define V3_VB_BADVALUE             3
#define V3_VB_READONLY             4
#define V3_VB_GENERR               5
#define V3_VB_NOACCESS             6
#define V3_VB_WRONGTYPE            7
#define V3_VB_WRONGLENGTH          8
#define V3_VB_WRONGENCODING        9
#define V3_VB_WRONGVALUE           10
#define V3_VB_NOCREATION           11
#define V3_VB_INCONSISTENTVALUE    12
#define V3_VB_RESOURCEUNAVAILABLE  13
#define V3_VB_COMMITFAILED         14
#define V3_VB_UNDOFAILED           15
#define V3_VB_AUTHORIZATIONERROR   16
#define V3_VB_NOTWRITABLE          17
#define V3_VB_INCONSISTENTNAME     18

/* values for "oper", passed to var_ routines */
#define SET_OP   -1
#define NEXT_OP  0
#define GET_OP   1

/* RowStatus values */
#define SNMP_RS_INVALID            0
#define SNMP_RS_ACTIVE             1
#define SNMP_RS_NOTINSERVICE       2
#define SNMP_RS_NOTREADY           3
#define SNMP_RS_CREATEANDGO        4
#define SNMP_RS_CREATEANDWAIT      5
#define SNMP_RS_DESTROY            6

/* StorageType values */
#define SNMP_ST_OTHER              1
#define SNMP_ST_VOLATILE           2
#define SNMP_ST_NONVOLATILE        3
#define SNMP_ST_PERMANENT          4
#define SNMP_ST_READONLY           5

#ifdef SNMP_COUNTER64
/* define the datatype to hold a 64-bit counter.
 * It can be done in a variety of ways (one 64bit value, 2 32bit values,
 * 8 bytes, etc. Hence use a union to define it.
 */

/* MIB table for 'ifEntry' group */

struct ifEntry_mib {
    long    ifIndex;
    void  * ifDescr;    /* 32 bit ptr */
    long    ifType;
    long    ifMtu;
    u_long  ifSpeed;
    void  * ifPhysAddress;  /* 32 bit ptr */
    long    ifAdminStatus;
    long    ifOperStatus;
    u_long  ifLastChange;
    u_long  ifInOctets;
    u_long  ifInUcastPkts;
    u_long  ifInNUcastPkts;
    u_long  ifInDiscards;
    u_long  ifInErrors;
    u_long  ifInUnknownProtos;
    u_long  ifOutOctets;
    u_long  ifOutUcastPkts;
    u_long  ifOutNUcastPkts;
    u_long  ifOutDiscards;
    u_long  ifOutErrors;
    u_long  ifOutQLen;
    oid  *  ifSpecific; /* 32 bit ptr */
};


struct trapVar {  /* struct for each trap variable */
     oid   varName[MAX_OID_LEN];/* ObjectId of variable */
     unsigned varNameLen;       /* oid components in varName */
     u_char   varType;          /* ASN.1 type of variable */
     unsigned varValLen;        /* octets in variable data field */
     u_char * varBuf;           /* the actual variable data */
     unsigned varBufLen;        /* used only by snmp_parse_trap() */
};

#endif /* SNMP_COUNTER64 */

