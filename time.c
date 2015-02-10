#include <stdio.h>
#include <reg51.h>
#include <System.h>
#include <Macro.h>


extern unsigned char SEC_timer_0;

void timer0 (void) interrupt T1INT using TIMER1_BANK
{  // Int Vector at 000BH, Reg Bank 1

	SEC_timer_0 ++ ;


  
}

void InitTimer(void)
{
  TMOD = 0x12; //timer0 = mode2, timer1 = mode1
  TH0 = (65535L - 65000L) / 256;	//timer0 = 60 mSec
  TL0 = (65535L -65000L) % 256;
  IE = 0x82; // EA = 1 ,ET0 = 1
  IP = 0x02; 
}

void timer_0(void) interrupt T0INT using TIMER0_BANK
{
	MStopTimer0Cnt();

	InitTimer();

	Time_Out++;

	MTrgTimer0Cnt();
}




