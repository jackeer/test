
#include <Def_INC.h>
#include <Test.h>
#include <View.h>
#include <System.h>
#include <Macro.h>


BYTE xdata _bFocusViewIndex;
BYTE xdata _bKeyProcessViewIndex;

BYTE xdata UI_kwy_buffer,Time_Out;
WORD xdata Time_Count;
BYTE xdata STATUS_TSK;
BOOL keypress;

/*
#ifdef Time0_support
#define PERIOD     -250
#endif
*/
/*
enum ALERT_BUTTON_TYPE
{
  ALERT_BTN_OK,
  ALERT_BTN_CANCEL,
  MAX_ALERT_BTN_NUM
};
*/
/*
struct Clock_Show
{
  BYTE Hour;
  BYTE Min;
  BYTE Sec;
  
}Clock_Show;
BYTE Clock_Show;
*/

//BYTE SEC_timer_0;

BYTE Init_All_Func(BYTE TSK)
{
	BYTE init_Tsk = TSK;
	if(init_Tsk >=UI_MAX || Time_Out >= 100)
		init_Tsk = UI_INIT;
	else if (!init_Tsk)
		init_Tsk = UI_INIT;
	else
		init_Tsk = UI_DETECT;
	return init_Tsk;
}

void InitIO(void)
{
  P2 = 0xFF;
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
void main(void)
{
BYTE xdata Task_loop;
/*
#ifdef Time0_support
TH0 = (unsigned char) PERIOD;          // set timer period
TL0 = (unsigned char) PERIOD;
TMOD = TMOD | 0x02;                    // select mode 2
TR0 = 1;                               // start timer 0
ET0 = 1;                               // enable timer 0 interrupt
EA  = 1;                               // global interrupt enable
#endif
SCON  = 0x50;               // SCON: mode 1, 8-bit UART, enable rcvr
TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit reload
TH1   = 221;                // TH1:  reload value for 1200 baud @ 16MHz
TR1   = 1;                  // TR1:  timer 1 run
TI    = 1;                  // TI:   set TI to send first char of UART
*/
	InitTimer();
	TR0 = 1;

	Task_loop = Init_All_Func(STATUS_TSK);

while(1)
{

	switch(Task_loop)
	{
		case UI_WAIT_INIT:
			if (Time_Out >= 100)
				STATUS_TSK = UI_DETECT;
			break;
			
		case UI_INIT:
			STATUS_TSK = UI_WAIT_INIT;
			InitIO();
			InitTimer();
			Time_Count = 0;
			Time_Out = 0;
			UI_kwy_buffer = 0xFF;
			_bFocusViewIndex = 0;
			_bKeyProcessViewIndex = 0;

			TR0 = 1;
			break;
			
		case UI_DETECT:
			if(P0 != 0xFF)
				{
				if(!keypress)
					{
					if(key_Power)
						break;
					else
						{
						keypress = 1;
						vViewInit();
						STATUS_TSK = UI_RUN;
						Time_Out = 0;
						break;
						}
					}
				}
			else
				{
				keypress = 0;
				break;
				}
		
		case UI_RUN:
			vViewProcess();
			P2 = UI_kwy_buffer;
/*
			if(Time_Out > 1000L)
				{
//				STATUS_TSK = UI_DETECT;
				Time_Out = 0;
				}
*/
			break;
			
		case UI_STANDBY:
			STATUS_TSK = UI_INIT;
			break;
			
		default:
			break;
	}

	Task_loop = STATUS_TSK;

}


//	AUTO_COLOR_TEST();
//	getchar();

//	try_test();
//	Test_Char_Key();
/*
#pragma asm

#pragma endasm
*/
}

