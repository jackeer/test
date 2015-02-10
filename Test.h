
#ifndef _TEST_H_
#define _TEST_H_

#ifndef CODE
#define CODE		code
#endif

#ifndef NULL_VIEW
#define NULL_VIEW  0
#endif

#ifndef TRUE
#define   TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

//#define Time0_support
#define key_Power P0_0
#define key_shink P0_1
#define key_shift P0_2
#define key_shift_stop P0_3
#define key5 P0_4
#define key6 P0_5
#define key7 P0_6
#define key8 P0_7

#define led1 P2_4
#define led2 P2_5
#define led3 P2_6
#define led4 P2_7

enum UI_STATE_TYPE {
	UI_WAIT_INIT,
	UI_INIT,
	UI_DETECT,
	UI_RUN,
	UI_STANDBY,
	UI_MAX
};
/*
extern void AUTO_COLOR_TEST(void);
extern void try_test(void);
extern void check_parms (char *string);
extern void Test_Char_Key(void);
*/
#endif
