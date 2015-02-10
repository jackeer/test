/*****************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of HappyTek Inc. (C) 2004
 *
 *****************************************************************************/
/**
 * @file keydef.h
 *
 * This file defines keys(IR & keyscan)
 *
 * @author Htk00469
 */

#ifndef _KEYDEF_H_
#define _KEYDEF_H_

enum {
	/* number key */
	IR_NUM0 = 0x00,
	IR_NUM1 = 0x01,
	IR_NUM2 = 0x02,
	IR_NUM3 = 0x03,
	IR_NUM4 = 0x04,
	IR_NUM5 = 0x05,
	IR_NUM6 = 0x06,
	IR_NUM7 = 0x07,
	IR_NUM8 = 0x08,
	IR_NUM9 = 0x09,

	/* CHANNEL */
	IR_CHANNEL_UP = 0x0A,
	IR_CHANNEL_DOWN = 0x0B,
	IR_ALTERNATE_CH = 0x0C,

	/* Favorite */
	/* show favorite channels list */
	IR_FAVORITE = 0x0D,
	/* favorite channel */
	IR_FAVORITE_CH = 0x0E,
	/* add or erase from favorite channel list */
	IR_FAVORITE_ADD_ERASE = 0x0F,

	/* Volume setting */
	IR_MUTE = 0x10,
	IR_VOLUME_UP = 0x11,
	IR_VOLUME_DOWN = 0x12,

	/* System Control */
	IR_POWER = 0x13,
	IR_ZOOM = 0x14,
	IR_CAPTURE = 0x15,
	IR_FREEZE = 0x16,
	IR_SLEEP = 0x17,
	IR_TIMER = 0x18,

	/* System Setting */
	IR_SOUND_EFFECT = 0x19,
	IR_MTS = 0x1A,
	IR_COLOR_SYSTEM = 0x1B,
	IR_SMART_PIC = 0x1C,
	IR_PICTURE_SIZE = 0x1D,
	IR_ACTIVE_CTRL = 0x1E,
	IR_INPUT_SOURCE = 0x1F,

	/* Arrow and Confirmation Key */
	IR_UP = 0x20,
	IR_LEFT = 0x21,
	IR_RIGHT = 0x22,
	IR_DOWN = 0x23,
	IR_ENTER = 0x24,

	/* Setup Menu */
	IR_SETUP_MENU = 0x25,
	IR_MENU_FACTORY = 0x26,

	/* OSD Message Display */
	IR_OSD_DISPLAY = 0x27,

	/* Close Caption and V-Chip */
	IR_CC = 0x28,
	IR_V_CHIP = 0x29,

	/* POP/PIP */
	IR_POP = 0x2A,
	IR_POP_POSITION = 0x2B,

	/* keys for Teletext */
	IR_TEXT = 0x2C,
	IR_RED = 0x2D,
	IR_GREEN = 0x2E,
	IR_YELLOW = 0x2F,
	IR_BLUE = 0x30,
	IR_INDEX = 0x31,
	IR_CLOCK = 0x32,
	IR_UPDATE = 0x33,
	IR_CE = 0x34,
	IR_NS = 0x35,

	/* for Frong panel keys */
	IR_RIGHT_VOLUME_UP = 0x36,
	IR_LEFT_VOLUME_DOWN = 0x37,
	IR_UP_CHANNEL_UP = 0x38,
	IR_DOWN_CHANNEL_DOWN = 0x39,
	IR_ENTER_INPUT_SRC = 0x3a,

	/* special combined keys */
	IR_SPECIAL_OPTIONS = 0x3b,
	IR_ENTER_LONG = 0x3c,
	IR_SHOW_HIDE_KEY = 0x3d,

	/* New Key */
	IR_OSD_DISPLAY_LONG,
	IR_TVCATV,
	IR_EXIT,

	IR_ROTATE,
	IR_GAME,
	IR_SURROUND,
	IR_REVERB,
	IR_AUDIO_MODE,
	IR_CALENDAR,
	IR_SND_MODE,
	IR_DSP_MODE,
	IR_DSP_MENU,
	IR_CHEDIT,

	IR_AUTO,		//DGM
	IR_BACKLIGHT,	//DGM
	/* max mapping keys define  */
	IR_GENERAL_KEY_MAP_MAX
};


#if 1 //SUPPORT_NEW_KEYDEF
#define IR_TTX_NUM0   (0x00)
#define IR_TTX_NUM1   (0x01)
#define IR_TTX_NUM2   (0x02)
#define IR_TTX_NUM3   (0x03)
#define IR_TTX_NUM4   (0x04)
#define IR_TTX_NUM5   (0x05)
#define IR_TTX_NUM6   (0x06)
#define IR_TTX_NUM7   (0x07)
#define IR_TTX_NUM8   (0x08)
#define IR_TTX_NUM9   (0x09) 


#define IR_TTX_RED (0x11) //(S)
#define IR_TTX_GREEN (0x12) //(S)
#define IR_TTX_YELLOW (0x13) //(S)
#define IR_TTX_CYAN (0x14) //(S)

#define IR_TTX_INDEX (0x15) //(S)
#define IR_TTX_CLOCK (0x16)
#define IR_TTX_UPDATE (0x17)
#define IR_TTX_TT_TV (0x18) //(S)

#define IR_TTX_UP (0x19) //(S)
#define IR_TTX_LEFT (0x20)
#define IR_TTX_RIGHT (0x21)
#define IR_TTX_DOWN (0x22) //(S)
#define IR_TTX_OK (0x23)

#define IR_TTX_NS (0x24)
#define IR_TTX_CE (0x25) //(S)

#define IR_TTX_EXIT (0x26) //(S)
#define IR_TTX_SUB_PAGE (0x27) //(S)
#define IR_TTX_HOLD (0x28) //(S)
#define IR_TTX_STORE (0x29) //(S)
#define IR_TTX_SIZE (0x30) //(S)
#define IR_TTX_REVEAL (0x31) //(S)
#define IR_TTX_LIST (0x32) //(S)
#define IR_TTX_NONE (0xFF)
#endif

#define  IR_NONE                                         0xff

#endif				/* _KEYDEF_H_ */

