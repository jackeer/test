/*************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of HappyTek Inc. (C) 2003
*
*************************************************************************/

/**
 * IR key mapping table.
 *
 * @author yj_chiang@Htk.com.tw
 */

/* map IR key code to internal key code */
#ifndef _IRKEY_H_
#define _IRKEY_H_
#include "Key_Def.h"

/* map IR key code to internal key code */
#define IR_CUSTOM_CODE      (_pbIRSysCode[0])
#define IR_CUSTOM_SUB_CODE  (_pbIRSysCode[1])

const BYTE code _pbIRSysCode[2] = { 0x04, 0xB9 };	/* DigiMedia */

/* for uPD6122 only */
#define IR_KEY_CODE_MAX   0x30
/* key Data Code Start value */
#define IR_KEY_GAP_START  0x40
/* Key size */
#define IR_KEY_GAP_SIZE   0x00
#define IR_KEY_TBL_SIZE   (IR_KEY_CODE_MAX - IR_KEY_GAP_SIZE)
/* DGM RC-23 */
const BYTE code _pbIRKeyTbl[IR_KEY_TBL_SIZE] = {
 IR_POWER,			//0x00	Power-TV
 IR_NUM0,			//0x01	0
 IR_NUM1,			//0x02	1
 IR_NUM2,			//0x03	2
 IR_NUM3,			//0x04	3
 IR_NUM4,			//0x05	4
 IR_NUM5,			//0x06	5
 IR_NUM6,			//0x07	6
 IR_NUM7,			//0x08	7
 IR_NUM8,			//0x09	8
 IR_NUM9,			//0x0A	9
 IR_NONE,			//0x0B	10
 IR_NONE,			//0x0C	11
 IR_NONE,			//0x0D	12
 IR_NONE,			//0x0E	100
 IR_NONE,			//0x0F	--
 IR_UP,				//0x10	ADJ Up
 IR_DOWN,			//0x11	ADJ Down
 IR_LEFT,				//0x12	ADJ Left
 IR_RIGHT,			//0x13	ADJ Right
 IR_CHANNEL_UP,		//0x14	CH+
 IR_CHANNEL_DOWN,	//0x15	CH-
 IR_VOLUME_UP,		//0x16	VOL+
 IR_VOLUME_DOWN,	//0x17	VOL-
 IR_ALTERNATE_CH,	//0x18	CH RTN
 IR_MTS,				//0x19	AUDIO
 IR_MUTE,			//0x1A	MUTE
 IR_SETUP_MENU,		//0x1B	MENU
 IR_ENTER,			//0x1C	ENTER
 IR_INPUT_SOURCE,	//0x1D	SOURCE
 IR_NONE,			//0x1E	FUNCTION
 IR_FAVORITE,		//0x1F	FAVORITE
 IR_OSD_DISPLAY,		//0x20	DISPLAY
 IR_NONE,			//0x21	INFO
 IR_NONE,			//0x22	GUIDE
 IR_BACKLIGHT,		//0x23	LIGHTING
 IR_AUTO,			//0x24	AUTO
 IR_PICTURE_SIZE,	//0x25	ASPECT
 IR_NONE,			//0x26	SWAP
 IR_NONE,			//0x27	VIEW
 IR_SLEEP,			//0x28	SLEEP
 IR_FREEZE,			//0x29	FREEZE
 IR_CC,				//0x2A	CC
 IR_TIMER,			//0x2B	Time
};

#endif				/* _IRKEY_H_ */

