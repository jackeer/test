/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of HappyTek Inc. (C) 2005
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   extern.h 
 *
 * Project:
 * --------
 *   MT8205
 *
 * Description:
 * ------------
 *   
 *   
 * Author:
 * -------
 *
 * Last changed:
 * -------------
 * $Author: $ 
 *
 * $Modtime: 05/06/02 10:53p $  
 *
 * $Revision: 1.1 $
****************************************************************************/

#ifndef _EXTERN_H_
#define _EXTERN_H_

#include "typedef.h"
#include "macro.h"

/****************************************************************
 * Clock Configuration
 *****************************************************************/
#define SYS_CLK                 100000000	// 100M

/* Timer */
#define MAX_T0_COUNTER_NS       2
#define T0_OSD_IDX              1

/* T0 Interval 250 ms */
#define T0_INTERVAL             (SYS_CLK >> 2)
#define T0_1SEC                 (SYS_CLK / T0_INTERVAL)
#define T0_1_2SEC               (T0_1SEC >> 1)
#define T0_1_4SEC               (T0_1SEC >> 2)

/* global for timer */
extern DWRD xdata _pdwT0[MAX_T0_COUNTER_NS];

#define vSetTimer0(dwIdx, dwValue)         _pdwT0[dwIdx] = dwValue
#define dwTimer0(dwIdx)                    (_pdwT0[dwIdx])
#define fgUtSetT0(dwIdx, dwValue)           _pdwT0[dwIdx] = dwValue
#define fgUtFreeT0(dwIdx)                   _pdwT0[dwIdx] = 0
#define dUtT0(dwIdx)                        (_pdwT0[dwIdx])

extern BOOL fgSetMacro;

#endif				/* _EXTERN_H_ */

