/*
+------------------------------------------------------------------------------+
| Woodhead Software & Electronics                                              |
| -------------------------------                                              |
|                                                                              |
|  Copyright 2004 (c)   Woodhead Software & Electronics. All rights reserved.  |
+------------------------------------------------------------------------------+
  Description .......:                                                          
                       
+------------------------------------------------------------------------------+
  $Workfile: eip_err.h $
  $Revision: 16 $
  $Date: 20/11/17 11:57 $
+------------------------------------------------------------------------------+
  Version | Date     | Author | Comments
  1.0.0     15/02/08   J.Y.B    Initial stack sources
+------------------------------------------------------------------------------+

*/

/* ----------------------------------------------------------------------------*/
/* ........................................................Definition of STATUS*/
/* ----------------------------------------------------------------------------*/
#define STAT_OK                       0    /* status ok with out comments              */
#define STAT_ERR_FONC                 1    /* function error code                      */
#define STAT_LOCK_DATA                4    /* no access data                           */
#define STAT_PAR                      32   /* bad parameter                            */
#define STAT_TIMEOUT                  33   /* time out                                 */
#define STAT_UNKNOWN_DEVICE           36   /* device unknown                           */
#define STAT_BUG                      52   /* bug in applicom sofware                  */
#define STAT_SYNC                     53   /* no synchronous on bus                    */
#define STAT_NOPOLLING                54   /* address is never polled                  */
#define STAT_BADINTPOL                55   /* too much time beetwen question/answer    */
#define STAT_NET_NO_READY             58   /* no answer from netm_ute to protocol task */
#define STAT_NOT_CONNECT              65   /* invalid logical link (DP & FMS)          */
#define STAT_RESOURCE                 66   /* no resource available                    */
#define STAT_OBJECT_NON_EXISTENT      68    
#define STAT_CNXN_DISABLE             69   /* the bit of the connection is disable     */
#define STAT_ABORT_USER               70
#define STAT_MAX_PDU_SIZE             74
#define STAT_USER_INITIATE_DENIED     77

#define STAT_UNKNOWN                  -1   /* Status unknown                           */

/* moved from app_scan.h */
/*-----------------------------------------------------------------------------*/
/* DEFINE General Status for CIP - Appendix B CIP spec											   */
/*-----------------------------------------------------------------------------*/
#define STAT_CIP_SUCCESS                0x00  /* We done good                  */
#define STAT_CIP_FAILURE                0x01  /* Connection failure            */
#define STAT_CIP_NO_RESOURCE            0x02  /* Resource(s) unavailable       */
#define STAT_CIP_BAD_DATA               0x03  /* Obj specific data bad         */
#define STAT_CIP_BAD_PATH               0x04  /* Path syntax error             */
#define STAT_CIP_BAD_CLASS_INSTANCE     0x05  /* Class/instance unknown        */
#define STAT_CIP_PARTIAL_DATA           0x06  /* Not all expected data sent    */
#define STAT_CIP_CONN_LOST              0x07  /* Messaging connection lost     */
#define STAT_CIP_BAD_SERVICE            0x08  /* Unimplemented service code    */
#define STAT_CIP_BAD_ATTR_DATA          0x09  /* Bad attribute data value      */
#define STAT_CIP_ATTR_LIST_ERROR        0x0A  /* Get/set attr list failed      */
#define STAT_CIP_ALREADY_IN_MODE        0x0B  /* Obj already in requested mode */
#define STAT_CIP_BAD_OBJ_MODE           0x0C  /* Wrong object state            */
#define STAT_CIP_OBJ_ALREADY_EXISTS     0x0D  /* Object already created        */
#define STAT_CIP_ATTR_NOT_SETTABLE      0x0E  /* Set of get only attr tried    */
#define STAT_CIP_PERMISSION_DENIED      0x0F  /* Insufficient access permission*/
#define STAT_CIP_DEV_IN_WRONG_STATE     0x10  /* Device not in proper mode     */
#define STAT_CIP_REPLY_DATA_TOO_LARGE   0x11  /* Response packet too large     */
#define STAT_CIP_FRAGMENT_PRIMITIVE     0x12  /* Primitive value will fragment */
#define STAT_CIP_CONFIG_TOO_SMALL       0x13  /* Goldilocks complaint #1       */
#define STAT_CIP_UNSUPPORTED_ATTR       0x14  /* Attribute is not supported    */
#define STAT_CIP_CONFIG_TOO_BIG         0x15  /* Goldilocks complaint #2       */
#define STAT_CIP_OBJ_DOES_NOT_EXIST     0x16  /* Non-existant object specified */
#define STAT_CIP_NO_FRAGMENTATION       0x17  /* Fragmentation not active      */
#define STAT_CIP_DATA_NOT_SAVED         0x18  /* Attr data not previously saved*/
#define STAT_CIP_DATA_WRITE_FAILURE     0x19  /* Attr data not saved this time */
#define STAT_CIP_REQUEST_TOO_LARGE      0x1A  /* Routing failure on request    */
#define STAT_CIP_RESPONSE_TOO_LARGE     0x1B  /* Routing failure on response   */
#define STAT_CIP_MISSING_LIST_DATA      0x1C  /* Attr data not found in list   */
#define STAT_CIP_INVALID_LIST_STATUS    0x1D  /* Returned list of attr w/status*/
#define STAT_CIP_SERVICE_ERROR          0x1E  /* Embedded service failed       */
#define STAT_CIP_CONN_RELATED_FAILURE   0x1F  /* Error in conn processing      */
#define STAT_CIP_INVALID_PARAMETER      0x20  /* Parameter was not valid       */
#define STAT_CIP_WRITE_ONCE_FAILURE     0x21  /* Write once previously done    */
#define STAT_CIP_INVALID_REPLY          0x22  /* Invalid reply received        */
#define STAT_CIP_BAD_KEY_IN_PATH        0x25  /* Electronic key in path failed */
#define STAT_CIP_BAD_PATH_SIZE          0x26  /* Invalid path size             */
#define STAT_CIP_UNEXPECTED_ATTR        0x27  /* Cannot set attr at this time  */
#define STAT_CIP_INVALID_MEMBER         0x28  /* Member ID in list nonexistant */
#define STAT_CIP_MEMBER_NOT_SETTABLE    0x29  /* Cannot set value of member    */
#define STAT_CIP_SERVICE_NOT_SUPPORTED  0x2E  /* Service Not supported for Specific Path    */

#define STAT_CIP_ERROR_CCO              0xD0  /* Connection Configuration error*/

#define STAT_API_ERROR                  0xFA  /* Error in function call        */
#define STAT_CIP_INT_ERROR              0xFB  /* Error from lower level(CIP)   */
#define STAT_NET_ERROR                  0xFC  /* Error from lower level(NET)   */
#define STAT_NET_EPIC_ERROR             0xFD  /* Error from lower level(EPIC)  */
#define STAT_NET_TCP_ERROR              0xFE  /* Error from lower level(TCP)   */
#define STAT_CIP_STILL_PROCESSING       0xFF  /* Special marker to indicate    */
                                              /* we haven't finished processing*/
                                              /* the request yet               */

#define STAT_CIP_ERROR                                              0x8000
#define STAT_CIP_INTERNAL                                           0x8100
#define STAT_CIP_NET_ERROR                                          0x8200
#define STAT_CIP_TCP_ERROR                                          0x8300
#define STAT_CIP_EPIC_ERROR                                         0x8400
#define STAT_CIP_APPLI_ERROR                                        0x8500
#define STAT_CIP_IO_ERROR                                           0x8600

#define STAT_CIP_NONE                                               0xFFFF

/*---------------------------------------------*/
/* STATUS EXTENDED*/
/*---------------------------------------------*/
/* 0x01 : STAT_CIP_FAILURE*/
#define STAT_EXT_CONNECTION_IN_USE                                  0x0100
#define STAT_EXT_TRANSCLASS_NOT_SUPPORTED                           0x0103
#define STAT_EXT_OWNERSHIP_CONFLICT                                 0x0106
#define STAT_EXT_CONNECTION_NOT_FOUND                               0x0107
#define STAT_EXT_INVALID_CONNECTION_TYPE                            0x0108  /* Obsolete */
#define STAT_EXT_INVALID_CONNECTION_SIZE                            0x0109  /* Obsolete */
#define STAT_EXT_DEVICE_NOT_CONFIGURED                              0x0110
#define STAT_EXT_RPI_NOT_SUPPORTED                                  0x0111
#define STAT_EXT_RPI_VALUE_NOT_ACCEPTABLE                           0x0112
#define STAT_EXT_CM_ANY_MORE_CONNECTION                             0x0113
#define STAT_EXT_VENDOR_PRODUCT_DONT_MATCH                          0x0114
#define STAT_EXT_PRODUCT_TYPE_DONT_MATCH                            0x0115
#define STAT_EXT_REVISION_DONT_MATCH                                0x0116
#define STAT_EXT_INVALID_CONNECTION_POINT                           0x0117  /* Obsolete */
#define STAT_EXT_INVALID_CONF_FORMAT                                0x0118  /* Obsolete */
#define STAT_EXT_CONTROL_CNXN_CLOSE                                 0x0119
#define STAT_EXT_TA_ANY_MORE_CONNECTION                             0x011A
#define STAT_EXT_RPI_SMALLER_PROD_INHIBIT                           0x011B

#define STAT_EXT_TRANSPORT_CLASS_NOT_SUPPORTED                      0x011C
#define STAT_EXT_PRODUCTION_TRIGGER_NOT_SUPPORTED                   0x011D
#define STAT_EXT_DIRECTION_NOT_SUPPORTED                            0x011E
#define STAT_EXT_INVALID_O_TO_T_NTK_CNXN_FIXVAR                     0x011F
#define STAT_EXT_INVALID_T_TO_O_NTK_CNXN_FIXVAR                     0x0120
#define STAT_EXT_INVALID_O_TO_T_NTK_CNXN_PRIORITY                   0x0121
#define STAT_EXT_INVALID_T_TO_O_NTK_CNXN_PRIORITY                   0x0122
#define STAT_EXT_INVALID_O_TO_T_NTK_CNXN_TYPE                       0x0123
#define STAT_EXT_INVALID_T_TO_O_NTK_CNXN_TYPE                       0x0124
#define STAT_EXT_INVALID_O_TO_T_NTK_CNXN_REDUNDANT_OWNER            0x0125
#define STAT_EXT_INVALID_CONFIGURATION_SIZE                         0x0126
#define STAT_EXT_INVALID_O_TO_T_SIZE                                0x0127
#define STAT_EXT_INVALID_T_TO_O_SIZE                                0x0128

#define STAT_EXT_INVALID_CONFIGURATION_APP_PATH                     0x0129
#define STAT_EXT_INVALID_CONSUMING_APP_PATH                         0x012A
#define STAT_EXT_INVALID_PRODUCING_APP_PATH                         0x012B
#define STAT_EXT_CNF_SYMBOL_DOES_NOT_EXIST                          0x012C
#define STAT_EXT_CONSUMING_SYMBOL_DOES_NOT_EXIST                    0x012D
#define STAT_EXT_PRODUCING_SYMBOL_DOES_NOT_EXIST                    0x012E
#define STAT_EXT_INCONSISTENT_APPLICATION_PATH_COMBINATION          0x012F

#define STAT_EXT_INCONSISTENT_CONSUME_DATA_FORMAT                   0x0130
#define STAT_EXT_INCONSISTENT_PRODUCE_DATA_FORMAT                   0x0131
#define STAT_EXT_NULL_FW_FUNCTION_NOT_SUPPORTED                     0x0132
#define STAT_EXT_CNXN_TIMEOUT_MULTIPLIER_NOT_ACCEPTABLE             0x0133

#define STAT_EXT_MISMATCHED_T_TO_O_NETWORK_CNXN_SIZE                0x0134
#define STAT_EXT_MISMATCHED_T_TO_O_NETWORK_CNXN_FIXVAR              0x0135
#define STAT_EXT_MISMATCHED_T_TO_O_NETWORK_CNXN_PRIORITY            0x0136
#define STAT_EXT_MISMATCHED_T_TO_O_TRANSPORT_CLASS                  0x0137
#define STAT_EXT_MISMATCHED_T_TO_O_PRODUCTION_TRIGGER               0x0138
#define STAT_EXT_MISMATCHED_T_TO_O_PRODUCTION_INHIBIT_TIME_SEGMENT  0x0139

#define STAT_EXT_CNXN_CANT_CLOSE_SINCE_TO                           0x0203
#define STAT_EXT_UNCONNECTED_SEND_TO                                0x0204
#define STAT_EXT_UNCONNECTED_SEND_PARAM                             0x0205
#define STAT_EXT_UNCONNECTED_SEND_TOO_LARGE                         0x0206
#define STAT_EXT_UNCONNECTED_ACK_WITHOUT_REPLY                      0x0207
#define STAT_EXT_NO_BUFFER_MEMORY_AVAILABLE                         0x0301
#define STAT_EXT_BANDWIDTH_NOT_AVAILABLE                            0x0302
#define STAT_EXT_NO_SCREENERS_AVAILABLE                             0x0303
#define STAT_EXT_NOT_CONF_FOR_REAL_TIME_DATA                        0x0304
#define STAT_EXT_PORT_NOT_AVAILABLE                                 0x0311
#define STAT_EXT_LINK_ADDR_NOT_AVAILABLE                            0x0312
#define STAT_EXT_INVALID_SEGMENT_TYPE_IN_PATH                       0x0315
#define STAT_EXT_ERROR_IN_CLOSE_PATH                                0x0316
#define STAT_EXT_SCHEDULING_NOT_SPECIFIED                           0x0317
#define STAT_EXT_LINK_ADDR_TO_SELF_INVALID                          0x0318
#define STAT_EXT_SECONDARY_RESOURCES_UNAVAILABLE                    0x0319
#define STAT_EXT_CONNECTION_ALREADY_ESTABLISHED                     0x031A
#define STAT_EXT_DIRECT_CONNECTION_ALREADY_EST                      0x031B
#define STAT_EXT_MISCELLANEOUS                                      0x031C
#define STAT_EXT_REDUNDANT_CONNECTION_MISMATCH                      0x031D
#define STAT_EXT_NO_MORE_CONSUMER_RESOURCES                         0x031E
#define STAT_EXT_NO_CONNECTION_RESOURCES_EXIST                      0x031F
#define STAT_EXT_CONNECTION_DISABLE                                 0x0320

#define STAT_EXT_PRODUCED_CONSUMED_DATA_FORMAT                      0x0814  /* Obsolete */

#define STAT_EXT_INTERNAL_FW_TO										                  0xFB01
#define STAT_EXT_INTERNAL_BAD_SOADDR							                  0xFB02
#define STAT_EXT_INTERNAL_BAD_OT_NET_PARAM				                  0xFB03
#define STAT_EXT_INTERNAL_BAD_TO_NET_PARAM				                  0xFB04
#define STAT_EXT_INTERNAL_BAD_UDP_PORT						                  0xFB05
#define STAT_EXT_INTERNAL_JOIN_MULTICAST					                  0xFB06
#define STAT_EXT_INTERNAL_PREPARE_IO_PACKET				                  0xFB07
#define STAT_EXT_INTERNAL_IO_PRODUCTION   				                  0xFB08
#define STAT_EXT_INTERNAL_IO_CONSUMPTION					                  0xFB09
#define STAT_EXT_INTERNAL_RESOURCES     					                  0xFB0A
#define STAT_EXT_INTERNAL_CONSUMPTION   					                  0xFB0B
#define STAT_EXT_INTERNAL_FW_CLOSE								                  0xFB0C
#define	STAT_EXT_INTERNAL_STOP_ERROR							                  0xFB0D
#define	STAT_EXT_INTERNAL_ADPT_STOPPED						                  0xFB0E

#define NO_EXTENDED_STATUS                                          0x0


/* 0xD0 : STAT_CIP_ERROR_CCO*/
#define STAT_EXT_CONNECTION_STOPPED                                 0x0001
#define STAT_EXT_CONNECTION_OPEN_PENDING                            0x0002
#define STAT_EXT_CONNECTION_CLOSE_PENDING                           0x0003
