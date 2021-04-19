// network abstration layer

#ifndef _TYPES_PN_H
#define _TYPES_PN_H

/*****************************************************************************/
/*                        DATA TYPE DEFINITION                               */
/*****************************************************************************/
#define APP_EXPORT __declspec( dllexport ) void

#define APP_NEAR
#define APP_FAR

#ifndef __export
  #define __export
#endif

#define APP_API __stdcall
#define APP_CALLBACK APP_API

#ifndef VOID
  #define VOID void
#endif

#ifndef FALSE
  #define FALSE 0
  #define TRUE  1
#endif

#ifndef APP_FALSE
  #define APP_FALSE 0
  #define APP_TRUE  1
#endif

#define APP_NULL   0

#ifndef NULL
  #define NULL APP_NULL
#endif


#define TCHAR   char
//#define HANDLE  int

#define APP_VOID void
#define APP_PVOID void *
#define APP_LPVOID void *

typedef unsigned long          APP_HANDLE,   APP_NEAR * APP_PHANDLE,   APP_FAR * APP_LPHANDLE;
/*typedef void            APP_VOID,     APP_NEAR * APP_PVOID,     APP_FAR * APP_LPVOID; */
/*shall be always a 16bits*/
typedef short           APP_BOOL,     APP_NEAR * APP_PBOOL,     APP_FAR * APP_LPBOOL;
typedef unsigned char   APP_BYTE,     APP_NEAR * APP_PBYTE,     APP_FAR * APP_LPBYTE;
typedef char            APP_CHAR,     APP_NEAR * APP_PCHAR,     APP_FAR * APP_LPCHAR;
typedef TCHAR           APP_TCHAR,    APP_NEAR * APP_PTCHAR,    APP_FAR * APP_LPTCHAR;
typedef short int       APP_SHORT,    APP_NEAR * APP_PSHORT,    APP_FAR * APP_LPSHORT;
typedef unsigned short  APP_WORD,     APP_NEAR * APP_PWORD,     APP_FAR * APP_LPWORD;
typedef unsigned int    APP_DWORD,    APP_NEAR * APP_PDWORD,    APP_FAR * APP_LPDWORD;
typedef int            APP_LONG,     APP_NEAR * APP_PLONG,     APP_FAR * APP_LPLONG;
typedef float           APP_FLOAT,    APP_NEAR * APP_PFLOAT,    APP_FAR * APP_LPFLOAT;
typedef void *                                                  APP_FAR * APP_AR_HANDLE;
typedef void *                                                  APP_FAR * APP_CR_HANDLE;



#endif
