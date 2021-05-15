/****************************************************************************
***
***     TMG Gesellschaft fuer industrielle Informationstechnik mbH
***
*****************************************************************************
***
***     Filename     : IAndM.h
***     Author       : ESc
***
*****************************************************************************
***
***     Description : This file contains the data structures of the I&M
***                   interface
***
****************************************************************************/
#ifndef _IANDMH_
#define _IANDMH_


typedef struct {
    /* mandatory parameters */
    byte Header[10];
    unsigned short ManufacturerID;
    unsigned char OrderID[20];
    unsigned char SerialNumber[16];
    unsigned short HardwareRevision;
    byte SoftwareRevision[4];
    unsigned short RevisionCounter;
    unsigned short ProfileID;
    byte ProfileSpecificType[2];
    byte IMVersion[2];
    byte IMSupported[2];
} TIAndM0;

typedef struct {
    /* mandatory parameters */
    byte Header[10];
    unsigned char TagFunction[32];
    unsigned char TagLocation[22];
} TIAndM1;

typedef struct {
    /* mandatory parameters */
    byte Header[10];
    unsigned char InstallDate[16];
    byte Reserved[38];
} TIAndM2;

typedef struct {
    /* mandatory parameters */
    byte Header[10];
    byte PA_IM_Version[2];
    byte Reserved[18];
    byte PA_IMSupported[2];
} TPA_IAndM0;

/* functions which are called from the user */
extern byte IAndMdpe_write_ind(byte C2_Sap, TBufPtr p);
extern byte IAndMdpe_read_ind(byte C2_Sap, TBufPtr p);
extern void InitiateAREP(void);
extern void InitiateAREPSingle(byte sap);

/* result codes of IAndMdpe_write_ind and IAndMdpe_read_ind */
#define DM_SUCCESS 0
#define DM_ACCESS_ERROR 0xA9
#define DM_INVALID_SLOT 0xB0
#define DM_INVALID_INDEX 0xB0
#define DM_INVALID_WRITE_LENGTH 4
#define DM_NO_RESOURCES 5
#define DM_INVALID_RANGE 6
#define DM_INVALID_DATA 7
#define DM_NOT_SUPPORTED 0xA9
#define DM_VERSION_CONFLICT 9
#define DM_FIRSTOTHER 10
#define DM_STATE_CONFLICT 0xB5
#define DM_MODULE_FAILURE 12


/* these structures must be filled by the user application */
extern const TIAndM2 IAndM2;
extern const TIAndM1 IAndM1;
extern const TIAndM0 IAndM0;

#endif
