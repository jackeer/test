/*****************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of HappyTek Inc. (C) 2003
 *
 *****************************************************************************/
/**
 * @file typedef.h
 *
 * type define header
 *
 * @author Htk00469
 */

#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#ifdef PC_8202_SIMULATOR
#include "PC_General.h"
#endif

/*=====================================================================
  basic type definition
=====================================================================*/

typedef void VOID;

typedef bit BIT;
typedef bit BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWRD;
typedef char CHAR;
typedef int INT;
typedef short SHORT;
typedef long LONG;
typedef float FLOAT;
typedef double DOUBLE;
typedef long double LDOUBLE;
#define SBIT              sbit

typedef char INT8;
typedef short INT16;
typedef long INT32;

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned long UINT32;

typedef char *LPSTR;		/* Null-terminated string */
typedef const char *LPCSTR;

typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned int UINT;
typedef unsigned long ULONG;

typedef void *HANDLE;

/*=====================================================================
  Function pointer type definition
=====================================================================*/
/*
 * A generic function pointer is always stored using one more byte than a
 * CODE memory-specific function pointer. If you're sure the function body
 * share the smae segment space with the function pointer, please use CODE
 * memory-specific function pointer to achive high execution efficiency.
 * Othwerwise, please use generic function pointer to achive high safety.
 */

/* Generic Pointer */
typedef void (*vFuncVoid) (void) large;
typedef void (*vFuncByte) (BYTE) large;
typedef BYTE(*bFuncVoid) (void) large;
typedef BYTE(*bFuncByte) (BYTE) large;
typedef BOOL(*fgFuncVoid) (void) large;

/* CODE Memory-specific Pointers */
typedef void (code * vCoFuncVoid) (void) large;
typedef void (code * vCoFuncByte) (BYTE) large;
typedef BYTE(code * bCoFuncVoid) (void) large;
typedef BYTE(code * bCoFuncByte) (BYTE) large;
typedef BOOL(code * fgCoFuncVoid) (void) large;

/*=====================================================================
  Common macro definition
=====================================================================*/
#define CONST   const
#ifndef CODE
#define CODE    code
#endif
#define PUBLIC  extern
#define PRIVATE static

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL  0
#endif

#define ON   1
#define OFF  0

#ifdef PC_8202_SIMULATOR
/* Special for OSD Service/Driver */
#include "defs.h"
#include "pc_api_osd.h"
#include "pc_drv_osd.h"
#include "pc_osd_prv.h"
#endif

#endif

