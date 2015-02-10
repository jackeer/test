
#ifndef __FLOAT_H__
#define __FLOAT_H__


/*========================
I2C Delay Time
	   __________
SCL _/
	   __________
SDA	_/
==========================*/
extern void IiCWait(BYTE Delay_Time) large;

/*========================
I2C Initialize
	   __________
SCL _/
	   __________
SDA	_/
==========================*/
extern void IIC_Inital(void) large;


/*========================
I2C Stop
                   __________
SCL ______/
                       ________
SDA	________/
==========================*/
extern void IIC_Stop(void) large;


/*========================
I2C start 
	__________
SCL 			    \____________
				
	_______
SDA		     \_______________
==========================*/
extern void IICstart(void) large;


/*========================
I2C Send_BIT 
            ______
SCL __/           \_____
				
	  _________
SDA_/               \____
==========================*/
extern void IICSend_BIT(bit HI_LOW) large;


/*========================
I2C GET_BIT 
              ______
SCL 	___/           \_____
				
	__    _____
SDA	    X ( RET ) X______
==========================*/
extern BOOL IICGet_BIT (void) large;


/*================================
I2C Send_BYTE
           _    _    _    _    _    _    _    _      _
SCL __/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \__/ \_____
                                                         ACK				
             _______________________       _
SDA ___XXXXXXXXXXXXXXXXXXXXX___X__
=================================*/
extern BOOL IICSend_BYTE(BYTE BYTE_DATA) large;


/*================================
I2C Get_BYTE
           _    _    _    _    _    _    _    _      _
SCL __/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \__/ \_____
                                                         ACK				
             _______________________       _
SDA ___XXXXXXXXXXXXXXXXXXXXX___X__
=================================*/
extern BOOL IICGet_BYTE(BYTE BYTE_DATA) large;


/*================================
I2C Select_Device
         _______    _     _______     _
SCL _||||||||_||__||||||||__||__
                       ACK                  ACK
          ______     _     _______     _
SDA _|||||||__||__||||||||__||__
=================================*/
extern BOOL IICSelect_Device(BYTE DEVICE,BYTE ADDRES) large;


/*================================
I2C Write_Byte_Device
         _______    _     _______     _
SCL _||||||||_||__||||||||__||__
                       ACK                  ACK
          ______     _     _______     _
SDA _|||||||__||__||||||||__||__
=================================*/
extern BOOL IICWrite_Byte(BYTE DEVICE,BYTE ADDRES,BYTE Len,BYTE *Data) large;


/*================================
I2C Read_Byte_Device
         _______    _     _______     _
SCL _||||||||_||__||||||||__||__
                       ACK                  ACK
          ______     _     _______     _
SDA _|||||||__||__||||||||__||__
=================================*/
extern BOOL IICRead_Byte(BYTE DEVICE,BYTE ADDRES,BYTE Len,BYTE *Data) large;

#endif


