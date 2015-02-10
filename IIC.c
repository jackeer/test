#include <Def_INC.h>
#include <IIC.h>

sbit ISDA = 0x90^1; 
sbit ISCL = 0x90^2;


/*========================
I2C Delay Time
	  __________
SCL _/
	  __________
SDA	_/
==========================*/
void IiCWait(BYTE Delay_Time) large
{
	BYTE i;
	for(i=0;i<=Delay_Time;i++)
	 {
	 _nop_();
	 }

}

/*========================
I2C Initialize
	  __________
SCL _/
	  __________
SDA	_/
==========================*/
void IIC_Inital(void) large
{
	ISDA=1;
	ISCL=1;
}

/*========================
I2C Stop
           __________
SCL ______/
             ________
SDA	________/
==========================*/
void IIC_Stop(void) large
{
	ISDA=0;
	IiCWait(1);
	ISCL=1;
	IiCWait(1);
	ISDA=1;
}

/*========================
I2C start 
	__________
SCL 		  \____________
				
	_______
SDA		   \_______________
==========================*/
void IICstart(void) large
{
	ISDA=1;
	ISCL=1;
	IiCWait(1);
	ISDA=0;
	IiCWait(1);
	ISCL=0;
}

/*========================
I2C Send_BIT 
       ______
SCL __/      \_____
				
	  _________
SDA _/          \____
==========================*/
void IICSend_BIT(BOOL HI_LOW) large
{
	if(HI_LOW==1)
		{
		ISDA=1;
		ISCL=0;
		IiCWait(1);
		ISCL=1;
		IiCWait(1);
		ISCL=0;
		IiCWait(1);
		ISDA=0;
		}
	else if(HI_LOW==0)
		{
		ISDA=0;
		ISCL=0;
		IiCWait(1);
		ISCL=1;
		IiCWait(1);
		ISCL=0;
		IiCWait(1);
		ISDA=0;
		}
}

/*========================
I2C GET_BIT 
        ______
SCL ___/      \_____
				
	__ _________
SDA	__X_(_RET_)_X______
==========================*/
BOOL IICGet_BIT (void) large
{
	BOOL RET_ACK=1;

	ISDA=1;
	ISCL=0;
	IiCWait(1);
	ISCL=1;
	IiCWait(1);
	RET_ACK=ISDA;
	ISCL=0;
	IiCWait(1);
	ISDA=0;

	return RET_ACK;
}

/*================================
I2C Send_BYTE
       _   _   _   _   _   _   _   _          _
SCL __/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \________/ \___
                                             ACK				
       _ _ _ _ _ _ _ _ _ _ _ _ _ _ _          _
SDA __/_X_X_X_X_X_X_X_X_X_X_X_X_X_X_\________/ \___
=================================*/
BOOL IICSend_BYTE(BYTE BYTE_DATA) large
{
	BYTE i,j;

	IIC_Stop();
	IIC_Inital();

	for(i=1;i<=8;i++)
		{
		j = (BYTE_DATA & 0x80);
		IICSend_BIT(j);
		BYTE_DATA >>= 1;
		}
	return IICGet_BIT();
}

/*================================
I2C Get_BYTE
       _   _   _   _   _   _   _   _         _
SCL __/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_______/ \___
									        ACK				
       _ _ _ _ _ _ _ _ _ _ _ _ _ _ _         _
SDA __/_X_X_X_X_X_X_X_X_X_X_X_X_X_X_\_______/ \___
=================================*/
BOOL IICGet_BYTE(BYTE BYTE_DATA) large
{
	BYTE i,j;
	
	IIC_Stop();
	IIC_Inital();

	for(i=1;i<=8;i++)
		{
		BYTE_DATA <<= 1;
		j = IICGet_BIT();
		BYTE_DATA = (j & 0x01);

		}
	return IICGet_BIT();

}

/*================================
I2C Select_Device
      _______  _   _______   _
SCL _||||||||_||__||||||||__||__
              ACK           ACK
      ______   _   _______   _
SDA _|||||||__||__||||||||__||__
=================================*/
BOOL IICSelect_Device(BYTE DEVICE,BYTE ADDRES) large
{

	if(!IICSend_BYTE(DEVICE))
		return (!IICSend_BYTE(ADDRES));
	return 0;
}

/*================================
I2C Write_Byte_Device
      _______  _   _______   _
SCL _||||||||_||__||||||||__||__
              ACK           ACK
      ______   _   _______   _
SDA _|||||||__||__||||||||__||__
=================================*/
BOOL IICWrite_Byte(BYTE DEVICE,BYTE ADDRES,BYTE Len,BYTE *Data) large
{
	BYTE i,*tmp;
	BOOL ret = 1;
	IIC_Stop();
	IIC_Inital();

	*tmp = *Data;

	DEVICE &= 0xFE;
	for(i=0;!IICSelect_Device(DEVICE,ADDRES);i++)
		{
		IiCWait(100);

		if(i>=10)
		return 0;
		}

	IiCWait(200);

	for(i=1;i<=Len;i++)
	{
	ret = IICSend_BYTE(*tmp);
	}
	
	return !ret;
}

/*================================
I2C Read_Byte_Device
      _______  _   _______   _
SCL _||||||||_||__||||||||__||__
              ACK           ACK
      ______   _   _______   _
SDA _|||||||__||__||||||||__||__
=================================*/
BOOL IICRead_Byte(BYTE DEVICE,BYTE ADDRES,BYTE Len,BYTE *Data) large
{
	BYTE i;
	BOOL ret = 1;

	IIC_Stop();
	IIC_Inital();

	DEVICE &= 0xFE;
	for(i=0;!IICSelect_Device(DEVICE,ADDRES);i++)
		{
		IiCWait(100);

		if(i>=10)
		return 0;
		}

	IiCWait(200);

	for(i=1;i<=Len;i++)
	{
	ret = IICGet_BYTE(*(Data++));
	}

	return !ret;
}

