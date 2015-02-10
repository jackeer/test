
/********************************************************
  IR Communication Function v0.1(Copyright(c) VXIS Inc. 2002)
  v0.0
  Data:2002.08.02
  by P.J. Yang
  v0.1
  Data:2003.01.15
  by P.J. Yang
  Modify:1.儲存變數減少為Custom Code和Data Code
  	 2.將最末端 last_code_com = 0; 程式移至 IF 判斷式外，避免
  	   無法連續按同一按鈕的問題。
  Data:2003.01.16
  by P.J. Yang
  Modify:1.修正持續壓住按鈕，但無法偵測的問題。
********************************************************/

#include <reg51.h>
#include <stdio.h>
#include <intrins.h>
#include <System.h>
#include <Macro.h>
#include "Func_ir.h"

extern void IR_Received (void);

BYTE idata IR_Received_Data[0x02];
BOOL last_code_com;	// A flag to show whether the last code receive is complete
				// 0 => Not complete
				// 1 => Completed

sbit IRPORT = P3^2;


/************************************************************************************
Function: IR_Received
Active Interrupt: INT0
Input:
      1.IRPORT
      2.last_code_com => A flag to show whether the received IR code is processed complete
				0 => Not complete
				1 => Completed
Output:
      1.IR_Received_Data[0] = Data_Code
        IR_Received_Data[1] = Custom_Code

How to use:
      1.Set INT0 enable
      2.Add below program code to your Code:
           extern char idata IR_Received_Data[0x04];
	   extern bit last_code_com;
      3.Then,your program can detect whether new IR_code have received by detecting
        the "last_code_com" flag.
        last_code_com = 0 => New IR_code have received
                        1 => No new IR_code
      4.After processing New IR_code,you should reset the "last_code_com" flag to '1'.

************************************************************************************/

void IR_Received() interrupt EX0INT using EX0INT_BANK
{
  unsigned char count1=0;
  unsigned char count2=0;
  char idata IR_Received_Data_temp[4];

  if (!last_code_com)		// Check whether the last code receive is complete
  {
      goto End_Int0;
  }

  EA = 0;			// Disable all interrupt

  // Check leader code. Signal will keep low until 9.2ms,then go high.
  // If signal go high before 9.2ms,it must be a noise and keep it out
  // Delay xx ms
  for (count1=0;count1<0x03;count1++)
  {
    for (count2=0;count2<0x96;count2++)
    {
    }
  }
  // Check whether the signal go high and delay xx ms
  for (count1=0;count1<0xff;count1++)
  {
   if (IRPORT)
   {
    goto End_Int0;			// The signal is noise
   }
  }

  // Start Receiving 32 bits data (customer code + data code)
  while (!IRPORT)			// wait until IRPORT = 1
  {					// waiting for the end of the carrier waveform of leader code
  }
  while (!IRPORT)			// wait until IRPORT = 1
  {					// Check the carrier waveform
  }
  while (IRPORT)			// wait until IRPORT = 0
  {					// waiting for the carrier waveform of custom code
  }

  for (count1=0;count1<0x20;count1++)
  {
    while (!IRPORT)			// wait until IRPORT = 1
    {					// waiting for the end of the carrier waveform of custom code
    }
    // To know the content of the code by checking how long it is
    for (count2=0;count2<0x7f;count2++)
    {
     _nop_();
     _nop_();
     if (!IRPORT)
     {
      break;
     }
    }

    // Save the received bit to carry flag
    CY = IRPORT;

    // Check whether the code is the same code.
    for (count2=0;count2<=0xff;count2++)
    {
     _nop_();
     _nop_();
     if (!IRPORT)
     {
      break;
     }
     else if (count2 == 0xff)
     {
	  last_code_com = 0;	// Set the flag to show New IR_Code have received
	  goto End_Int0;
     }
    }

    // Save the received bit to IR_Received_Data_temp by rotating ACC and Carrier
    ACC = IR_Received_Data_temp[0];
    #pragma asm
     rrc A
    #pragma endasm
    IR_Received_Data_temp[0] = ACC;

    ACC = IR_Received_Data_temp[1];
    #pragma asm
     rrc A
    #pragma endasm
    IR_Received_Data_temp[1] = ACC;

    ACC = IR_Received_Data_temp[2];
    #pragma asm
     rrc A
    #pragma endasm
    IR_Received_Data_temp[2] = ACC;

    ACC = IR_Received_Data_temp[3];
    #pragma asm
     rrc A
    #pragma endasm
    IR_Received_Data_temp[3] = ACC;
  }

  // Check whether IR_Received_Data_temp and IR_Received_Data is the same
  if (IR_Received_Data[0] != IR_Received_Data_temp[1] || IR_Received_Data[1] != IR_Received_Data_temp[3])
  {
   IR_Received_Data[0] = IR_Received_Data_temp[1];
   IR_Received_Data[1] = IR_Received_Data_temp[3];
  }
  last_code_com = 0;			// Set the flag to show New IR_Code have received

IR_Received_Data[1] = _pbIRKeyTbl[IR_Received_Data[1]];
	
End_Int0:
  EA = 1;	// Enable all interrupts
}

