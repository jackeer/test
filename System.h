/*************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the
 *  information contained herein may not be used or disclosed except with
 *  the written permission of HappyTek Inc. (C) 2003
 *
 *************************************************************************/
/**
 * @file system.h
 *
 * This file define system configuration
 *
 * @author Htk00469
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

/****************************************************************************
  Define system clock rate, in MHz
 ****************************************************************************/

#define SYS_CLOCK_RATE 27

/****************************************************************************
  Define ST(Sampling Time) for 8032 timer, in ms
 ****************************************************************************/
#define TIMER_BASE 20

#define TIMER0_ST TIMER_BASE
#define TIMER1_ST TIMER_BASE

/*=====================================================================
  REGISTER BANK SETTING
=====================================================================*/
#define MAIN_BANK   0		// General routines use bank 0
#define EX0INT_BANK 1		// EX0 interrupt use bank 1
#define TIMER0_BANK 2		// Timer 0 interrupt use bank 2
#define TIMER1_BANK 2		// Timer 1 interrupt use bank 2
#define TIMER2_BANK 2		// Timer 2 interrupt use bank 2

/*=====================================================================
  INTERRUPT VECTOR SETTING
=====================================================================*/
#define EX0INT      0		// iterrupt, EX0
#define T0INT       1		// Timer 0 interrupt
#define EX1INT      2		// iterrupt, EX1
#define T1INT       3		// Timer 1 interrupt
#define T2INT       5		// Timer 2 interrupt

/****************************************************************************
  Define macro to calculate timer configuration
 ****************************************************************************/
#define MAKE_TIMER_CFG(bVal) (65535 - ((bVal * 1000L * SYS_CLOCK_RATE)/12) + 1)

/****************************************************************************
  Define Timer 0 Configuration
 ****************************************************************************/
#define T0_CFG         MAKE_TIMER_CFG(TIMER0_ST)
#define TH0_CFG        (T0_CFG  & 0xFF00) >> 8
#define TL0_CFG        (T0_CFG  & 0x00FF)
#define T0_MODE        0x01	/* 16-bit timer */

/****************************************************************************
  Define Timer 1 Configuration
 ****************************************************************************/
#define T1_CFG         MAKE_TIMER_CFG(TIMER1_ST)
#define TH1_CFG        (T1_CFG  & 0xFF00) >> 8
#define TL1_CFG        (T1_CFG  & 0x00FF)
#define T1_MODE        0x10	/* 16-bit timer */

/****************************************************************************
  Define timer mode
 ****************************************************************************/
#define TIMER_MODE     (T0_MODE + T1_MODE)

/****************************************************************************
  Define ST(Sampling Time) for soft timer, in ms
 ****************************************************************************/
#define UPDT_TM_T0_ST    500	/* updata current time's ST -- timer 0 */
#define SYS_CLK_T1_ST    1000	/* system clock's ST -- timer 1 */


#endif				/* _8202_SYSTEM_H_ */

