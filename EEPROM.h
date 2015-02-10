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
 * @file eeprom.h
 *
 * This file defines EEPROM layout
 *
 * @author Htk00469
 */


#ifndef _EEPDEF_H
#define _EEPDEF_H




/*===================================================================*/
/* EEPROM position layout */
/*===================================================================*/

/**
 *  Description :
 *  Allocation LayOut of 8205 Eeprom, Each Item's Length is counted by Byte
*/
enum EEP_POSTION_LIST
{
	/** to check if the serial EEPROM is used */
	EEP_MAGIC0 = 0,
	EEP_MAGIC1,

	/** to check the serial EEPROM memory map version */
	EEP_VERSION0,
	EEP_VERSION1,

	/** to check the AC On Power Status */
#if SUPPORT_POWER_STATUS
	EEP_POWER_STATUS,
#endif


	EEP_GEN_SET_END
};

#define  EEPROM_GENERAL_SIZE   EEP_GEN_SET_END
#define  EEPROM_END_POS        EEP_GEN_SET_END

#endif				/* _EEPDEF_H */
