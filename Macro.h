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
 * @file macro.h
 *
 * Macro function
 *
 * @author Htk00469
 */

#ifndef _MACRO_H_
#define _MACRO_H_

#include "typedef.h"

/*=====================================================================
  MACRO : Enable and Disable Interrupt
=====================================================================*/
#define MEnableINT()            (EA = 1)
#define MDisableINT()           (EA = 0)
#define MEnableMainINT()        (EX0 = 1)
#define MDisableMainINT()       (EX0 = 0)
#define irq_save(flags)\
  	do {\
  		if (EX0 == 0) {\
  			flags = 0;\
  		} else {\
  			flags = 1;\
  		}\
  		EX0 = 0;\
  	} while (0)
#define irq_restore(flags)\
  	do {\
  		if (flags == 0) {\
  			EX0 = 0;\
  		} else {\
  			EX0 = 1;\
  		}\
  	} while (0)
#define MMainIntPTLow()         (PX0 = 0)
#define MMainIntPTHigh()        (PX0 = 1)

/*=====================================================================
  MACRO : Enable and Disable Timer0/Timer1/Timer2 Interrupt/Counter
=====================================================================*/
#define MEnableTimer0()         (ET0 = 1)
#define MDisableTimer0()        (ET0 = 0)
#define MTrgTimer0Cnt()         (TR0 = 1)
#define MStopTimer0Cnt()        (TR0 = 0)
#define MTimer0PTHigh()         (PT0 = 1)
#define MTimer0PTLow()          (PT0 = 0)
#define MSetTimer0CntLow(val)   (TL0 = val)
#define MSetTimer0CntHigh(val)  (TH0 = val)
#define MSetTimer0Mode(val)     (TMOD = ((TMOD&0xf0)|val))

#define MEnableTimer1()         (ET1 = 1)
#define MDisableTimer1()        (ET1 = 0)
#define MTrgTimer1Cnt()         (TR1 = 1)
#define MStopTimer1Cnt()        (TR1 = 0)
#define MTimer1PTHigh()         (PT1 = 1)
#define MTimer1PTLow()          (PT1 = 0)
#define MSetTimer1CntLow(val)   (TL1 = val)
#define MSetTimer1CntHigh(val)  (TH1 = val)
#define MSetTimer1Mode(val)     (TMOD = ((TMOD&0x0f)|val))

#define MEnableTimer2()         (ET2 = 1)
#define MDisableTimer2()        (ET2 = 0)
#define MTrgTimer2Cnt()         (TR2 = 1)
#define MStopTimer2Cnt()        (TR2 = 0)
#define MTimer2PTHigh()         (PT2 = 1)
#define MTimer2PTLow()          (PT2 = 0)
#define MSetTimer2CntLow(val)   (RCAP2L = TL2 = val)
#define MSetTimer2CntHigh(val)  (RCAP2H = TH2 = val)
#define MSetTimer2Mode(val)     (T2CON = val)

#define MSetTimerMode(val)      (TMOD = val)

#define M8032Halt()             PCON = 0x02

/*=====================================================================
  MACRO : byte/word access (HIGH BYTE FIRST)
=====================================================================*/
#define bHiByte(arg)      (*(BYTE*)&arg)
#define bLoByte(arg)      (*((BYTE*)&arg + 1))

#define wHiWord(arg)      (*(WORD*)&arg)
#define wLoWord(arg)      (*((WORD*)&arg + 1))

#define BYTE0(arg)        (*((BYTE *)&(arg) + 3))
#define BYTE1(arg)        (*((BYTE *)&(arg) + 2))
#define BYTE2(arg)        (*((BYTE *)&(arg) + 1))
#define BYTE3(arg)        (* (BYTE *)&(arg))

#define BYTE21(arg)      (*(WORD*)((BYTE*)&arg+1))

#define LOADL_DWRD(arg1, arg2)  BYTE3(arg2) = *((BYTE *)(arg1) + 3); \
                                  BYTE2(arg2) = *((BYTE *)(arg1) + 2); \
                                  BYTE1(arg2) = *((BYTE *)(arg1) + 1); \
                                  BYTE0(arg2) = *((BYTE *)(arg1))

/*=====================================================================
  MACRO : Get table size
=====================================================================*/
#define TBL_SIZE(tbl)   (sizeof(tbl)/sizeof(tbl[0]))

/*=====================================================================
  MACRO : For BCD number
=====================================================================*/
#define BCD_HI(bcd)  ((bcd) >> 4)
#define BCD_LO(bcd)  ((bcd) & 0x0f)
#define BCD2DEC(bcd) (BCD_HI(bcd) * 10 +BCD_LO(bcd))
#define DEC2BCD(dec) ((((dec) / 10) << 4) + ((dec) % 10))

/*=====================================================================
  MACRO : For Digial to HEX in ASCII conversion
=====================================================================*/
#define bDigToAsc(bDig) (((bDig) > 9) ? ((bDig) - 10 + 'A') : ((bDig) + '0'))

/*=====================================================================
  MACRO : Number Comparison Macro
=====================================================================*/
#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*=====================================================================
  MACRO : Expand Value to Meet Special Request
=====================================================================*/
#define MAKE_EVEN(val) ((((val) + 1) >> 1) << 1)	/* to be even */
#define MAKE_ODD(val) ((val) | 0x01)	/* to be odd */

  /* Expand given value to be multiple of 8 in upper alignment */
#define MAKE_8X(val) ((((val) + 0x07) >> 3) << 3)

  /* Expand given value to be multiple of 16 in upper alignment */
#define MAKE_16X(val) ((((val) + 0x0F) >> 4) << 4)

  /* Make given value to be multiple of 16 in center alignment */
#define MAKE_16X_EX(val) ((((val) + 0x08) >> 4) << 4)

  /* Expand given value to be multiple of 4 in center alignment */
#define  MAKE_4X(val)  ((((val) + 0x01) >> 2) << 2)

/*=====================================================================
 *  Description:
 *    Make a item position with same step
 *  Parameters:
 *    start		base start position of all items
 *    index		index of the item to be calculated
 *    step		step of all items
=====================================================================*/
#define MAKE_ITEM_POS(start, index, step) ((start) + (index) * (step))

/*=====================================================================
  MACRO : Swap Integers Macro
=====================================================================*/
#define SWAP(a, b)     {(a) ^= (b); (b) ^= (a); (a) ^= (b);}

/*=====================================================================
  MACRO : Retrieve Value from Pointer
=====================================================================*/
#define dwGetPtrVal(ptr)  (*((DWRD *) (ptr)))	/* Get DWORD value from a pointer */
#define wGetPtrVal(ptr)   (*((WORD *) (ptr)))	/* Get  WORD value from a pointer */
#define vSetPtrValDW(ptr, dwVal)  (*((DWRD *)ptr) = dwVal)
#define vSetPtrValW(ptr, wVal)  (*((WORD *)ptr) = wVal)

/*=====================================================================
  MACRO : Retrieve value of each digit for decimal integer
=====================================================================*/
/* Maximum digit number of decimal integer. Please extend it if needed */
#define MAX_DIGIT_NUM 4

#ifdef _MAIN_FILE_
CODE WORD nMagicDigit[MAX_DIGIT_NUM] = {
	1, 10, 100, 1000
};
#else
extern CODE WORD nMagicDigit[MAX_DIGIT_NUM];
#endif

/*  Description:
 *    Get value of each digit for decimal integer
 *  Parameters:
 *    nVal		Integer value to be parsed
 *    bIdx		index of digit to be retrieved, zero based.
 */
#define bGetDigit(nVal, bIdx) (((nVal) / nMagicDigit[bIdx]) % 10)

#define vResetSystem() ((void (code*) (void)) 0x0000)()

#endif

