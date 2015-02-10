#include <reg51.h>
#include <intrins.h>
#include "TUNER.H"
#include "Define.H"
//-----------------------------------------------------------------------------------------
typedef union {
	struct CHLIMIT{
		unsigned char TV_Min;
		unsigned char TV_Max;
		unsigned char TV_Total;
		unsigned char CATV_Min;
		unsigned char CATV_Max;
		unsigned char CATV_Total;
	} CH_LIMIT;
	unsigned char bytes[1];
} UNI_Tuner_Limit;
UNI_Tuner_Limit idata UNI_Tuner;
//-----------------------------------------------------------------------------------------
xdata unsigned char TunerAddr = 0xC0;
///xdata unsigned char TunerBrand;

idata unsigned char VP_Max;
idata unsigned char HP_Min;

idata unsigned short DevNum;
xdata unsigned char FineTune;

idata unsigned char ChBuffer;
static unsigned char i2c_error = 1;                 /* Last error */
idata unsigned char ChStatus;

idata unsigned char bIrChNum=0;
unsigned char ChannelBuf[3]={0, 0, 0};
idata unsigned char TV_COUNTRY_AREA;

sbit sda = P0^0;
sbit scl = P0^1;
sbit W_en = P3^1;
//-----------------------------------------------------------------------------------
extern bit ack;
extern bit FLG_Power;
extern unsigned int idata osd_timer;
//==================================================================
void OSD_Show_Channel(char,char);
void OSD_Show_Num(char stpos2, char Num);
void Delay_nop_();
unsigned char ReadTunerOneByte(unsigned char device);
void Modify_TV_Area(void);
///unsigned short FineTuneChannel(unsigned short freq,unsigned char pb);
//-----------------------------------------------------------------------------------
extern void shi_sub_read (char,char,char,char*);
extern void shi_sub_write (char,char,char,char*);
extern void delay_nop();
extern void stop_con();
extern void start_con();
extern void nack_con();
extern void ack_con();
extern bit send_con(char);
extern char receive_con();
extern void osd_show(char,char,char*);
extern void DelayXms(unsigned char count);
extern void Write24C16(unsigned short EepAddr,unsigned char dat);
///extern unsigned char Read24C16(unsigned short addr16);
extern void osd_clear(char);
extern void osd_clearall();
extern void SetupChNumOSD();
extern void osd_size(char block, char sizex, char sizey);
extern void osd_pos(char block, char posx, char posy);
extern void aisel_func(char aisel);
extern void OSD_Flag_Reset(void);
extern void osd_switch(char osd_en);
extern void ShowCap(char signal);
extern void Osd_Show_Char(char stpos2, char numfont, char * fontcode);
extern void VideoBlanking(bit Video_Blanking);
extern void osd_hbsection(char block, bit hb, char xstart, char xend, char ystart, char yend);
extern void osd_color(char block, bit fb, bit h, char color);
//=================================================================
///extern char code AIR_TV[6];
///extern char code CATV[4];
//=================================================================
//=================================================================
// For FORWARD's Tuner
// Note:1.為了節省 ROM 的空間，tuner_freq 表中只記錄周波數表中頻率的整數部份，如 CH2 的頻率
// 	為55.25，表中只記錄55，省略了後面的小數0.25的部份。這樣作的好處是整個 tuner_freq
// 	表只需要宣告為 16 bits 的資料長度，而不必宣告為32 bits 的浮點數格式。省略的小數部
// 	份將在channel_sel()函式運算中作補償，補償的方法是將省略的 0.25 部份加至 45.75 而
// 	成為整數 46，因此在channel_sel()函式運算中，對應於週波數表中的頻率值所加入的基頻值
// 	應為 46，而不是45.75。
//=================================================================
// (83.25+45.75)*20=2580=0x0A14
// (83,250,000+45,750,000)/50000=2580=0x0A14		(STEP FREQUENCY = 50KHz)
//
// (83.25+45.75)*16=2064=0x0810
// (83,250,000+45,750,000)/62500=2064=0x0810		(STEP FREQUENCY = 62.5KHz)
//=================================================================
//=================================================================
unsigned short code CATV1_freq[125] = {				// US181 CABLE CHANNEL
	73,	55,	61,	67,	77,	83,	175,	181,	187,	193,		// 10
	199,	205,	211,	121,	127,	133,	139,	145,	151,	157,		// 20
	163,	169,	217,	223,	229,	235,	241,	247,	253,	259,		// 30
	265,	271,	277,	283,	289,	295,	301,	307,	313,	319,		// 40
	325,	331,	337,	343,	349,	355,	361,	367,	373,	379,		// 50
	385,	391,	397,	403,	409,	415,	421,	427,	433,	439,		// 60
	445,	451,	457,	463,	469,	475,	481,	487,	493,	499,		// 70
	505,	511,	517,	523,	529,	535,	541,	547,	553,	559,		// 80
	565,	571,	577,	583,	589,	595,	601,	607,	613,	619,		// 90
	625,	631,	637,	643,	91,	97,	103,	109,	115,	649,		// 100
	655,	661,	667,	673,	679,	685,	691,	697,	703,	709,		// 110
	715,	721,	727,	733,	739,	745,	751,	757,	763,	769,		// 120
	775,	781,	787,	793,	799};
unsigned short code AIRTV1_freq[69] = {				// US181 AIR CHANNEL
	73,	55,	61,	67,	77,	83,	175,	181,	187,	193,		// 10
	199,	205,	211,	471,	477,	483,	489,	495,	501,	507,		// 20
	513,	519,	525,	531,	537,	543,	549,	555,	561,	567,		// 30
	573,	579,	585,	591,	597,	603,	609,	615,	621,	627,		// 40
	633,	639,	645,	651,	657,	663,	669,	675,	681,	687,		// 50
	693,	699,	705,	711,	717,	723,	729,	735,	741,	747,		// 60
	753,	759,	765,	771,	777,	783,	789,	795,	801};
//------------------------------------------------------------------------------------
unsigned short code CATV2_freq[63] = {				// JAPAN CABLE CHANNEL
	91,	97,	103,	171,	177,	183,	189,	193,	199,	205,		// 10
	211,	217,	109,	115,	121,	127,	133,	139,	145,	151,		// 20
	157,	165,	223,	231,	237,	243,	249,	253,	259,	265,		// 30
	271,	277,	283,	289,	295,	301,	307,	313,	319,	325,		// 40
	331,	337,	343,	349,	355,	361,	367,	373,	379,	385,		// 50
	391,	397,	403,	409,	415,	421,	427,	433,	439,	445,		// 60
	451,	457,	463};
unsigned short code AIRTV2_freq[62] = {				// JAPAN AIR CHANNEL
	91,	97,	103,	171,	177,	183,	189,	193,	199,	205,		// 10
	211,	217,	471,	477,	483,	489,	495,	501,	507,	513,		// 20
	519,	525,	531,	537,	543,	549,	555,	561,	567,	573,		// 30
	579,	585,	591,	597,	603,	609,	615,	621,	627,	633,		// 40
	639,	645,	651,	657,	663,	669,	675,	681,	687,	693,		// 50
	699,	705,	711,	717,	723,	729,	735,	741,	747,	753,		// 60
	759,	765};
//------------------------------------------------------------------------------------
// CCIR CABLE CHANNEL (Germany), Sound IF 33.4MHz, Picture IF 38.9MHz
unsigned short code CATV3_freq[56] = {				// channel 90.25MHz & 97.25MHz not included in the table
	44,	48,	55,	62,	69,	76,	83,	105,	112,	119,		// 10	
	126,	133,	140,	147,	154,	161,	168,	175,	182,	189,		// 20
	196,	203,	210,	217,	224,	231,	238,	245,	252,	259,		// 30
	266,	273,	280,	287,	294,	303,	311,	319,	327,	335,		// 40
	343,	351,	359,	367,	375,	383,	391,	399,	407,	415,		// 50
	423,	431,	439,	447,	455,	463};
unsigned short code AIRTV3_freq[69] = {				// CCIR AIR CHANNEL
	44,	48,	55,	62,	175,	182,	189,	196,	203,	210,		// 10
	217,	224,	44,	44,	44,	44,	44,	44,	44,	44,		// 20
	471,	479,	487,	495,	503,	511,	519,	527,	535,	543,		// 30
	551,	559,	567,	575,	583,	591,599,	607,	615,	623,		// 40
	631,	639,	647,	655,	663,	671,	679,	687,	695,	703,		// 50
	711,	719,	727,	735,	743,	751,	759,	767,	775,	783,		// 60
	791,	799,	807,	815,	823,	831,	839,	847,	855};
//------------------------------------------------------------------------------------
// Australia PAL B/H channel, 50.0KHz step freq.
unsigned short code CATV4_freq[69] = {				// Australia PAL CABLE CHANNEL
//	46,
	57,	64,	86,	95,	102,	175,	182,	189,	196,	209,		// 10
	216,	223,	245,	252,	259,	266,	273,	280,	287,	294,		// 20
	303,	310,	317,	324,	331,	338,	345,	527,	534,	541,		// 30
	548,	555,	562,	569,	576,	583,	590,	597,	604,	611,		// 40
	618,	625,	632,	639,	646,	653,	660,	667,	674,	681,		// 50
	688,	695,	702,	709,	716,	723,	730,	737,	744,	751,		// 60
	758,	765,	772,	779,	786,	793,	800,	807,	814};
unsigned short code AIRTV4_freq[69] = {				// Australia PAL AIR CHANNEL
//	46,
	57,	64,	86,	95,	102,	175,	182,	189,	196,	209,		// 10
	216,	223,	245,	252,	259,	266,	273,	280,	287,	294,		// 20
	303,	310,	317,	324,	331,	338,	345,	527,	534,	541,		// 30
	548,	555,	562,	569,	576,	583,	590,	597,	604,	611,		// 40
	618,	625,	632,	639,	646,	653,	660,	667,	674,	681,		// 50
	688,	695,	702,	709,	716,	723,	730,	737,	744,	751,		// 60
	758,	765,	772,	779,	786,	793,	800,	807,	814};
//------------------------------------------------------------------------------------
// UK channel for B/W Broadcast
unsigned short code CATV5_freq[14] = {				// UK CABLE CHANNEL
	45,	51,	56,	61,	66,	179,	184,	189,	194,	199,
	204,	209,	214,	219};
unsigned short code AIRTV5_freq[14] = {				// UK AIR CHANNEL
	45,	51,	56,	61,	66,	179,	184,	189,	194,	199,
	204,	209,	214,	219};
//------------------------------------------------------------------------------------
unsigned short code CATV6_freq[57] = {				// CHINA CABLE CHANNEL
	49,	57,	65,	77,	85,	168,	176,	184,	192,	200,		// 10
	208,	216,	471,	479,	487,	493,	503,	511,	519,	527,		// 20
	535,	543,	551,	559,	607,	615,	623,	631,	639,	647,		// 30
	655,	663,	671,	679,	687,	695,	703,	711,	719,	727,		// 40
	735,	743,	751,	759,	767,	775,	783,	791,	799,	807,		// 50
	815,	823,	831,	839,	847,	855,	863};
unsigned short code AIRTV6_freq[57] = {				// CHINA AIR CHANNEL
	49,	57,	65,	77,	85,	168,	176,	184,	192,	200,		// 10
	208,	216,	471,	479,	487,	493,	503,	511,	519,	527,		// 20
	535,	543,	551,	559,	607,	615,	623,	631,	639,	647,		// 30
	655,	663,	671,	679,	687,	695,	703,	711,	719,	727,		// 40
	735,	743,	751,	759,	767,	775,	783,	791,	799,	807,		// 50
	815,	823,	831,	839,	847,	855,	863};
//------------------------------------------------------------------------------------
// US181, Japan, CCIR, AUSTRALIA, UK, CHINA
code unsigned short *CATV_FREQ_TAB[]={
	CATV1_freq, CATV2_freq, CATV3_freq, CATV4_freq, CATV5_freq, CATV6_freq
};
code unsigned short *AIR_FREQ_TAB[]={
	AIRTV1_freq, AIRTV2_freq, AIRTV3_freq, AIRTV4_freq, AIRTV5_freq, AIRTV6_freq
};
//==================================================================
code unsigned char INTERMEDIATE_FREQ[]={46,	46,	39,	39,	46,	46};
//==================================================================
code unsigned short STEP_FREQ[]={62500,	62500,	50000,	50000,	62500,	62500};
//==================================================================
code unsigned short VHF_FREQ[]={0x0BC0,	0x0BC0,	0x0E45,	0x0E45,	0x0BC0,	0x0BC0};
//-------------------------------------------------------------------------------------
code unsigned short UHF_FREQ[]={0x1E80,	0x1E80,	0x2461,	0x2461,	0x1E80,	0x1E80};
//==================================================================
code unsigned char DIFF_FREQ[]={0,	0,	3,	3,	0,	0};
//==================================================================
code unsigned char ChannelLimitTab[]={
//	TV_Min,	TV_Max,	TV_Total,	CATV_Min,	CATV_Max,	CATV_Total
	1,		69,		69,		1,			125,			125,		// USA
	1,		62,		62,		1,			63,			63,		// Japan
	1,		69,		69,		1,			56,			56,		// Germary
	1,		69,		69,		1,			69,			69,		// Australia
	1,		14,		14,		1,			14,			14,		// UK
	1,		57,		57,		1,			57,			57		// China
};
//==================================================================
code unsigned char MaskTab1[8]={
	0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80
};
//-------------------------------------------------------------------------------------
code unsigned char MaskTab2[8]={
	0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f
};
//==================================================================
char code USA[3] = {'U', 'S', 'A'};
char code JAPAN[5] = {'J', 'A', 'P', 'A', 'N'};
char code GERMARY[7] = {'G', 'E', 'R', 'M', 'A', 'R', 'Y'};
char code AUSTRALIA[9] = {'A', 'U', 'S', 'T', 'R', 'A', 'L', 'I', 'A'};
char code UK[2] = {'U', 'K'};
char code CHINA[5] = {'C', 'H', 'I', 'N', 'A'};
///char code SCANNING[8] = {'S', 'C', 'A', 'N', 'N', 'I', 'N', 'G'};
char code SCANNING[11] = {0x1C, 0x26, 0x24, 0x31, 0x31, 0x2C, 0x31, 0x2A, 0x44, 0x44, 0x44};
//==================================================================
//==================================================================
void LoadChLimit(void)
{
	unsigned char i, j;
	unsigned char EEPBUF;

	DelayXms(0x10);
	shi_sub_read(EEPROM,0xF0,0x00,&TV_COUNTRY_AREA);

	if(TV_COUNTRY_AREA > 0x06)
	{
		TV_COUNTRY_AREA = 0;
		DelayXms(0x10);
		shi_sub_write(EEPROM,0xF0,0x00,&TV_COUNTRY_AREA);
	}

	i = TV_COUNTRY_AREA * 6;

	for(j=0; j<6; j++)
		UNI_Tuner.bytes[j] = ChannelLimitTab[i+j];

	DelayXms(0x10);

	shi_sub_read(EEPROM, 0xF1, 0x00, &EEPBUF);	// Read AirChannel or CATVChannel

	if(EEPBUF==0)
	{
		shi_sub_read(EEPROM,0xF2,0x00,&ChBuffer);
		
		if(ChBuffer < UNI_Tuner.CH_LIMIT.TV_Min)
		{
			ChBuffer = UNI_Tuner.CH_LIMIT.TV_Min;
			DelayXms(0x10);
			shi_sub_write(EEPROM,0xF2,0x00,&ChBuffer);
		}

		if(ChBuffer > UNI_Tuner.CH_LIMIT.TV_Max)
		{
			ChBuffer = UNI_Tuner.CH_LIMIT.TV_Max;
			DelayXms(0x10);
			shi_sub_write(EEPROM,0xF2,0x00,&ChBuffer);
		}
	}
	else
	{
		shi_sub_read(EEPROM,0xF3,0x00,&ChBuffer);

		if(ChBuffer < UNI_Tuner.CH_LIMIT.CATV_Min)
		{
			ChBuffer = UNI_Tuner.CH_LIMIT.CATV_Min;
			DelayXms(0x10);
			shi_sub_write(EEPROM,0xF3,0x00,&ChBuffer);
		}

		if(ChBuffer > UNI_Tuner.CH_LIMIT.CATV_Max)
		{
			ChBuffer = UNI_Tuner.CH_LIMIT.CATV_Max;
			DelayXms(0x10);
			shi_sub_write(EEPROM,0xF3,0x00,&ChBuffer);
		}
	}
	bIrChNum = 0;
///	osd_timer = 0;
}
//==================================================================
//==================================================================
void ChannelUpDown(bit Direct)
{
	unsigned char CH,j,k,l,m;
	unsigned char EEPBUF;

	shi_sub_read(EEPROM, 0xF1, 0x00, &EEPBUF);			// Read AirChannel or CATVChannel

	if(EEPBUF==0)	// Air Channel
	{
		shi_sub_read(EEPROM, 0xF2, 0x00, &CH);			// Read Air Channel
		for(j=0; j<UNI_Tuner.CH_LIMIT.TV_Total; j++)
		{
			if(Direct)	CH++;
			else			CH--;

			if(CH > UNI_Tuner.CH_LIMIT.TV_Max)
				CH = UNI_Tuner.CH_LIMIT.TV_Min;

			if(CH < UNI_Tuner.CH_LIMIT.TV_Min)
				CH = UNI_Tuner.CH_LIMIT.TV_Max;

			k = (CH - UNI_Tuner.CH_LIMIT.TV_Min) / 8;
			l = (CH - UNI_Tuner.CH_LIMIT.TV_Min) % 8;
			shi_sub_read(EEPROM, 0x80+k, 0x00, &m);		// AirChannel Channel Mask
			if((m & MaskTab1[l]) != 0)
			{
				break;
			}
		}
		DelayXms(0x80);
		Write24C16(0xF2,CH);
	}
	else			// Cable Channel
	{
		shi_sub_read(EEPROM, 0xF3, 0x00, &CH);			// Read CATV Channel
		for(j=0; j<UNI_Tuner.CH_LIMIT.CATV_Total; j++)
		{
			if(Direct)	CH++;
			else			CH--;

			if(CH > UNI_Tuner.CH_LIMIT.CATV_Max)
				CH = UNI_Tuner.CH_LIMIT.CATV_Min;

			if(CH < UNI_Tuner.CH_LIMIT.CATV_Min)
				CH = UNI_Tuner.CH_LIMIT.CATV_Max;

			k = (CH - UNI_Tuner.CH_LIMIT.CATV_Min) / 8;
			l = (CH - UNI_Tuner.CH_LIMIT.CATV_Min) % 8;
			shi_sub_read(EEPROM, 0x90+k, 0x00, &m);
			if((m & MaskTab1[l]) != 0)
			{
				break;
			}
		}
		DelayXms(0x80);
		shi_sub_write(EEPROM,0xF3,0x00,&CH);
	}
	ChBuffer = CH;
	ChannelSel(ChBuffer);
	DelayXms(0x05);
	SetupChNumOSD();
	osd_clearall();
	ShowCap(0xFF);
	osd_size(title,0x2D,0x00);					// Set Title block size
///	osd_show(0x00, 0x05, AIR_TV);
///	ShowCap(UNI_Sel.v.Sel_Signal);
	OSD_Show_Channel(0x29,ChBuffer);
}
//==================================================================
//==================================================================
unsigned char CheckChMask(unsigned char Mask)
{
	unsigned char k,l,m;
	unsigned char EEPBUF;

	shi_sub_read(EEPROM, 0xF1, 0x00, &EEPBUF);	// Read AirChannel or CATVChannel

	if(EEPBUF==0)
	{
		k = (ChBuffer - UNI_Tuner.CH_LIMIT.TV_Min) / 8;
		l = (ChBuffer - UNI_Tuner.CH_LIMIT.TV_Min) % 8;

		shi_sub_read(EEPROM, 0x80+k, 0x00, &m);

		if(Mask > 1)
		{
			if((m & MaskTab1[l]) != 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else if(Mask == 0)
		{
			m &= MaskTab2[l];
			Write24C16(0x80+k,m);
			return 0;
		}
		else
		{
			m |= MaskTab1[l];
			Write24C16(0x80+k,m);
			return 1;
		}
	}
	else
	{
		k = (ChBuffer - UNI_Tuner.CH_LIMIT.CATV_Min) / 8;
		l = (ChBuffer - UNI_Tuner.CH_LIMIT.CATV_Min) % 8;

		shi_sub_read(EEPROM, 0x90+k, 0x00, &m);

		if(Mask > 1)
		{
			if((m & MaskTab1[l]) != 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else if(Mask == 0)
		{
			m &= MaskTab2[l];
			Write24C16(0x90+k,m);
			return 0;
		}
		else
		{
			m |= MaskTab1[l];
			Write24C16(0x90+k,m);
			return 1;
		}
	}
}
//==================================================================
//==================================================================
//	Change channel of Tuner Function (For VX1120A)
//
// Input Factor:
//	ch_num       => 8 bits , Channel Num
//	ptr_chtable  => Addr, A point of channel table address
// Output Factor:
//	None
//==================================================================
void ChannelSel(unsigned char ch)
{
	unsigned short *p;
	unsigned char EEPBUF;

	DelayXms(0x10);

	shi_sub_read(EEPROM, 0xF1, 0x00, &EEPBUF);	// Read AirChannel or CATVChannel

	if(EEPBUF == 0)
	{
		if(ch < UNI_Tuner.CH_LIMIT.TV_Min)
		{
			ch = UNI_Tuner.CH_LIMIT.TV_Min;
			ChBuffer = ch;
		}
		else if(ch > UNI_Tuner.CH_LIMIT.TV_Max)
		{
			ch = UNI_Tuner.CH_LIMIT.TV_Max;
			ChBuffer = ch;
		}

		ch = ch - UNI_Tuner.CH_LIMIT.TV_Min;
		p = AIR_FREQ_TAB[TV_COUNTRY_AREA] + ch;
		DevNum = *p;
		DevNum = DevNum + INTERMEDIATE_FREQ[TV_COUNTRY_AREA];
		DevNum = DevNum * (1000000/STEP_FREQ[TV_COUNTRY_AREA]);
		DevNum += DIFF_FREQ[TV_COUNTRY_AREA];

		DelayXms(0x10);
	}
	else
	{
		if(ch < UNI_Tuner.CH_LIMIT.CATV_Min)
		{
			ch = UNI_Tuner.CH_LIMIT.CATV_Min;
			ChBuffer = ch;
		}
		else if(ch > UNI_Tuner.CH_LIMIT.CATV_Max)
		{
			ch = UNI_Tuner.CH_LIMIT.CATV_Max;
			ChBuffer = ch;
		}
		ch = ch - UNI_Tuner.CH_LIMIT.CATV_Min;
		p = CATV_FREQ_TAB[TV_COUNTRY_AREA] + ch;
		DevNum = *p;
		DevNum = DevNum + INTERMEDIATE_FREQ[TV_COUNTRY_AREA];
		DevNum = DevNum * (1000000/STEP_FREQ[TV_COUNTRY_AREA]);
		DevNum += DIFF_FREQ[TV_COUNTRY_AREA];
		DelayXms(0x10);
	}
	SetTuner();
	delay_nop();

///	shi_sub_read(EEPROM, 0xF1, 0x00, &EEPBUF);	// Read AirChannel or CATVChannel

///	if(EEPBUF)	osd_show(0x00, 0x05, AIR_TV);
///	else			osd_show(0x00, 0x03, CATV);
}
//==================================================================
//==================================================================
void SetTuner(void)
{
	unsigned char DivH,DivL,StepCode,Band;

	DivH = (unsigned char)(DevNum >> 8);
	DivL = (unsigned char)DevNum;

	#if(TUNER == PHILIPS_Tuner)	// Philips tuner
		if(DevNum < VHF_FREQ[TV_COUNTRY_AREA])
		{
			Band=0xA0;
		}
		else
		{
			if(DevNum < UHF_FREQ[TV_COUNTRY_AREA])
				Band=0x90;
			else
				Band=0x30;
		}
	#else						// Listen tuner
		if(DevNum < VHF_FREQ[TV_COUNTRY_AREA])
		{
			Band=0x01;
		}
		else
		{
			if(DevNum < UHF_FREQ[TV_COUNTRY_AREA])
				Band=0x02;
			else
				Band=0x08;
		}
	#endif

	DelayXms(0x10);

	if (TUNER == PHILIPS_Tuner)	// Philips tuner
		StepCode = 0x8E;			// set non-fast tuning, test mode = normal mode, step freq. = 62.5KHz, and PLL disabling = normal operation
	else if(TV_COUNTRY_AREA == AREA_GERMANY)
		StepCode = 0xC0;
	else							// Listen tuner
		StepCode = 0x86;			// set step freq. = 62.5KHz

///	DivH = 0x08;
///	DivL = 0x10;
///	StepCode = 0x86;
///	Band = 0x01;

	WriteTuner(DivH,DivL,StepCode,Band);
}
//==================================================================
void ScanChannel(void)
{
	unsigned char ChannelBuf, max_ch;
	unsigned char EEPBUF;
	unsigned char DataBuf;

	shi_sub_read(EEPROM, 0xF1, 0x00, &EEPBUF);	// Read AirChannel or CATVChannel

	if(EEPBUF==0)
		max_ch = UNI_Tuner.CH_LIMIT.TV_Max;
	else
		max_ch = UNI_Tuner.CH_LIMIT.CATV_Max;

	ChannelBuf = ChBuffer;

	osd_show(0x00, 10, SCANNING);

	for ( ChBuffer = 1; ChBuffer <= max_ch; ChBuffer++ )
	{
		VideoBlanking(1);
		ChannelSel(ChBuffer);
		osd_clear(12);
///		osd_show(0x00, 10, SCANNING);
		OSD_Show_Channel(0x29, ChBuffer);

		aisel_func(0x01);		// Select TV
		DelayXms(100);
		aisel_func(0x02);		// Select TV
		DelayXms(100);
		DataBuf = 0x60;
		shi_sub_write(VX1818, 0x10, 0, &DataBuf);
		DelayXms(100);
		DataBuf = 0x20;
		shi_sub_write(VX1818, 0x10, 0, &DataBuf);
		DelayXms(100);
		shi_sub_read(VX1818, 0xD3, 0x00, &ChStatus);		// NOISE_MORE, PHALT, IS443, VPRES, MODE, FINEAGC, MVVALID

		if( ChStatus & 0x10 )
		{
			VideoBlanking(0);
			CheckChMask(1);
		}
		else
		{
			CheckChMask(0);		// test
		}
	}
	DelayXms(10);
	ChBuffer = ChannelBuf;
	VideoBlanking(1);
	ChannelSel(ChBuffer);
	DelayXms(100);
	osd_clear(0);
	OSD_Show_Channel(0x29, ChannelBuf);
	VideoBlanking(0);
}
//==================================================================
// Adjust Channel Freq. and Check if locked.
/*
unsigned short FineTuneChannel(unsigned short freq,unsigned char pb)
{
	unsigned char i,j;

	#if (TUNER == PHILIPS_Tuner)
		freq -= 40;				// 40*50K=2M
	#else
		freq -= 32;				// 32*62.5K=2M
	#endif

	for ( i = 0; i < 9; i++ )			//8*0.5M=4M, 0.5M/step
	{
		#if (TUNER == PHILIPS_Tuner)
			WriteTuner(((freq >> 8) & 0xff),(freq & 0xff),0x88,pb);
		#else
			WriteTuner(((freq >> 8) & 0xff),(freq & 0xff),0x8e,pb);
		#endif

		Delay_nop_();
		
		if ( (ReadTunerOneByte(TunerAddr) & 0x40) == 0x40 )	// Locked
		{
			return freq;
		}
		else
		{
			freq -= 4;
			for ( j = 0; j < 9; j++ )
			{
				WriteTuner(((freq >> 8) & 0xff),(freq & 0xff),0x8e,pb);
				Delay_nop_();

				if ( (ReadTunerOneByte(TunerAddr) & 0x40) )	// Locked
				{
					return freq;
				}
				freq += 1;	//fine tune 62.5K
			}
		}
		freq += 8;	//coarse fine 0.5M
	}

	return 0;
}
*/
//==================================================================
void ChannelNumInput(unsigned char IrNum)
{
	unsigned int ChNumTemp;
	unsigned char EEPBUF;
	bit OSD_TimerUp = 0;

	switch(IrNum)
	{
		case 0x05:			// +10
			osd_timer=0;

			OSD_Flag_Reset();
			osd_switch(0x00);
			ShowCap(0xFF);

			osd_clear(0);
///			ShowCap(UNI_Sel.v.Sel_Signal);
			IrNum = 1;
			bIrChNum = 2;
			ChannelBuf[bIrChNum] = IrNum;
			OSD_Show_Num(0x28, IrNum);
			OSD_Show_Num(0x29, 0x3F);
			OSD_Show_Num(0x2A, 0x3F);
			return;
		case 0x1B:			// 1
			IrNum = 1;
			break;
		case 0x5A:			// 2
			IrNum = 2;
			break;
		case 0x1A:			// 3
			IrNum = 3;
			break;
		case 0x0E:			// 4
			IrNum = 4;
			break;
		case 0x0D:			// 5
			IrNum = 5;
			break;
		case 0x12:			// 6
			IrNum = 6;
			break;
		case 0x1C:			// 7
			IrNum = 7;
			break;
		case 0x1D:			// 8
			IrNum = 8;
			break;
		case 0x1F:			// 9
			IrNum = 9;
			break;
		case 0x1E:			// 0
			IrNum = 0;
			break;

		case 0xFF:
			OSD_TimerUp = 1;
			break;

		default:
			break;
	}
	if (bIrChNum==0 && !OSD_TimerUp)
	{
		osd_timer=0;

		OSD_Flag_Reset();
		osd_switch(0x00);
		ShowCap(0xFF);

		osd_clear(0);
///		ShowCap(UNI_Sel.v.Sel_Signal);
		bIrChNum = 1;
		ChannelBuf[bIrChNum] = IrNum;
		OSD_Show_Num(0x29, IrNum);
		OSD_Show_Num(0x2A, 0x3F);
	}
	else if(bIrChNum==2 && !OSD_TimerUp)
	{
		bIrChNum = 1;
		ChannelBuf[bIrChNum] = IrNum;
		OSD_Show_Num(0x29, IrNum);
	}
	else if(bIrChNum==1 || OSD_TimerUp)
	{
		if(!OSD_TimerUp)
		{
			bIrChNum = 0;
			ChannelBuf[bIrChNum] = IrNum;
			OSD_Show_Num(0x2A, IrNum);
		}

		if(bIrChNum==0)
		{
			if(!OSD_TimerUp)
				ChNumTemp = ChannelBuf[2]*100+ChannelBuf[1]*10+ChannelBuf[0];
			else
				ChNumTemp = ChannelBuf[1];
				
			shi_sub_read(EEPROM, 0xF1, 0x00, &EEPBUF);	// Read AirChannel or CATVChannel

			if(EEPBUF==0)	// Air Channel
			{
				if((ChNumTemp>=UNI_Tuner.CH_LIMIT.TV_Min)&&(ChNumTemp<=UNI_Tuner.CH_LIMIT.TV_Max))
				{
					ChBuffer = ChNumTemp;
					Write24C16(0xF2,ChBuffer);
					VideoBlanking(1);
					ChannelSel(ChBuffer);
					aisel_func(0x01);		// Select TV
					aisel_func(0x02);		// Select TV
					VideoBlanking(0);
				}
				else
				{
					osd_clear(0);
///					ShowCap(UNI_Sel.v.Sel_Signal);
					OSD_Show_Channel(0x29, ChBuffer);
				}
			}
			else
			{
				if((ChNumTemp>=UNI_Tuner.CH_LIMIT.CATV_Min)&&(ChNumTemp<=UNI_Tuner.CH_LIMIT.CATV_Max))
				{
					ChBuffer = ChNumTemp;
					Write24C16(0xF3,ChBuffer);
					VideoBlanking(1);
					ChannelSel(ChBuffer);
					aisel_func(0x01);		// Select TV
					aisel_func(0x02);		// Select TV
					VideoBlanking(0);
				}
				else
				{
					osd_clear(0);
///					ShowCap(UNI_Sel.v.Sel_Signal);
					OSD_Show_Channel(0x29, ChBuffer);
				}
			}

			ChannelBuf[0] = 0;
			ChannelBuf[1] = 0;
			ChannelBuf[2] = 0;
		}
	}
}
//==================================================================
//==================================================================
//==================================================================
void HEX2DEC(unsigned int VALUE,unsigned char *DigitBuf)
{
	*DigitBuf=VALUE%10;

	while(VALUE>0)
	{
		DigitBuf++;
		VALUE=VALUE/10;
		*DigitBuf=VALUE%10;
	}

}
//===================================================================
void osd_show_register(char stpos2, char reg)
{
	char idata temp[0x02] = {0x10,0x00};
	char idata numtemp[0x02];

	temp[1] = stpos2;
	shi_sub_write(VX1818,cw1,0x01,&temp[0]);

	numtemp[0] = (reg >> 4) & 0x0F;
	numtemp[1] = reg & 0x0F;
	shi_sub_write(VX1818,cw3,0x01,numtemp);
}
//===================================================================
void OSD_Show_Channel(char stpos2, char reg)
{
	char idata temp[0x02] = {0x10,0x00};
	char idata DigitBuf[3] = {0,0,0};
	char idata TempBuf[3] = {0,0,0};
	unsigned char title_color = 0x84;

	shi_sub_write(VX1818, 0x5B, 0, &title_color);

	if(reg < 100)
		temp[1] = stpos2;
	else
		temp[1] = stpos2 - 1;

	shi_sub_write(VX1818,cw1,0x01,&temp[0]);

	HEX2DEC(reg, TempBuf);

	DigitBuf[0] = TempBuf[2];
	DigitBuf[1] = TempBuf[1];
	DigitBuf[2] = TempBuf[0];

	if(reg < 100)
	{
		shi_sub_write(VX1818,cw3,1,DigitBuf+1);
	}
	else
	{
		shi_sub_write(VX1818,cw3,2,DigitBuf);
	}
}
//===================================================================
void OSD_Show_Num(char stpos2, char Num)
{
	char idata temp[0x02] = {0x10,0x00};
	unsigned char title_color;

	title_color = 0x84;
	shi_sub_write(VX1818, 0x5B, 0, &title_color);

	temp[1] = stpos2;
	shi_sub_write(VX1818,cw1,0x01,&temp[0]);

	shi_sub_write(VX1818, cw3, 0x00, &Num);
}
//===================================================================
void SetupChNumOSD()
{
	// Set size of block
	osd_size(content, 0x03, 0x01);						// Set Content block size
	// Set position of block
	osd_pos(content, 0x01, 0x01);						// Set Content block position
}
//===================================================================

//===================================================================
//===================================================================
void Delay_nop_()
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

void SclH(void)
{
	scl = 1;
	Delay_nop_();
	Delay_nop_();
	Delay_nop_();
}

void SclL(void)
{
	scl = 0;
}

void SdaH(void)
{
	sda = 1;
}

void SdaL(void)
{
	sda = 0;
}

static bit SdaOut(const bit b)
{
	return(sda = (b));
}

static bit SdaIn(void)
{
	return(sda);
}

bit IIC_Start(void)
{
	SdaH();		//Before stop, Add SdaH, SdaL :Making a Wave, 
	SclH();		//Let SCL go up  

	Delay_nop_();
	
	SdaL();		//Pull SDA down... 
	
	Delay_nop_();
	Delay_nop_();
	Delay_nop_();
	
	SclL();		//...and then SCL -> start condition. 
	
	return 0;
}

bit IIC_Stop(void)
{
	SdaL();		//Before stop, Add SdaH, SdaL :Making a Wave, 

	Delay_nop_();
	SclH();		//Let SCL go up  
	
	Delay_nop_();
	Delay_nop_();
	Delay_nop_();
	Delay_nop_();
	
	SdaH() ;		//..and then SDA up -> stop condition. 
	
	Delay_nop_();
	Delay_nop_();
	return 1;
}

static bit I2cBitIn( )
{
	bit bin;

	SclH();//Pull SCL back up 
	Delay_nop_();
	Delay_nop_();
	
	bin = SdaIn();//Read in data 
	Delay_nop_();
	Delay_nop_();
	SclL();//Pull SCL back up 
	Delay_nop_();
	Delay_nop_();
	return bin;// Return the sampled bit 
}

static bit I2cBitOut(const bit bout)
{
	SdaOut(bout);// Put data out on SDA 
	Delay_nop_();

	#if 1
		SclH();//Let SCL go up 
		Delay_nop_();
		Delay_nop_();
		Delay_nop_();
		Delay_nop_();

		if (SdaIn() != bout)
		{
			//Arbitration lost, release bus and return 
			Delay_nop_();
			SdaH();//Should be up anyway, but make sure 
			Delay_nop_();
			SclH();//
			Delay_nop_();

			i2c_error = I2CERR_LOST;
			return 1;
		}
		SclL();//Pull SCL back down 
		Delay_nop_();//1029	
		Delay_nop_();
		Delay_nop_();
		Delay_nop_();
	#endif

	return 0;// OK 
}
//========================================================================
bit IIC_WriteByte(unsigned char dat)
{
	unsigned char bit_count;

	bit_count=8;
	i2c_error=0;

	SdaH();
	Delay_nop_();
	Delay_nop_();

	while(bit_count)
	{
		if (dat & 0x80)
		{
			if (I2cBitOut(1)) return 1;
			Delay_nop_();
		}
		else	
		{
			if (I2cBitOut(0)) return 1;
			Delay_nop_();
		}
		dat <<= 1;
		bit_count--;
	}
	
	Delay_nop_();
	Delay_nop_();
	
	// Set SDA High to release bus wait for ack
	Delay_nop_();
	Delay_nop_();
	SdaH();

	if (I2cBitIn())
	{
		i2c_error = I2CERR_NAK;
		Delay_nop_();
		Delay_nop_();
		return i2c_error;
	}
	Delay_nop_();
	Delay_nop_();//1029
	return 0;
}

//========================================================================
static void DelayShort(const unsigned char time)
{
	unsigned char i=0;

	for(i=0;i<(time*1);i++);
}
//========================================================================
static I2C_STATUS DevWriteTuner(const unsigned char DEVICE,unsigned char COUNT,const  unsigned int address, const unsigned char* cData, ADDR_INDEX addr_index)
{
	unsigned char byteptr, byte_out;

	if (IIC_Start()) return 1;
	i2c_error = 1;

	//Send 1st Slave address
	byte_out = DEVICE & 0xfe;     /* Ensure that it's a write address */
	if (IIC_WriteByte(byte_out))
	{
		IIC_Stop();
		return i2c_error;
	}

	if(addr_index==ADDR_2BYTE)
	{
		//Send 2nd Reg HiByte address (NVRam)
		byte_out = HiByte(address);     /* Ensure that it's a write address */
		if (IIC_WriteByte(byte_out))
		{
			IIC_Stop();
			return i2c_error;
		}
	}

	//Send 2nd Reg address LoByte address (NVRam)
	byte_out = LoByte(address);     /* Ensure that it's a write address */
	if (IIC_WriteByte(byte_out))
	{
		IIC_Stop();
		return i2c_error;
	}

	//Send the other datas
	for(byteptr=0; byteptr<COUNT; byteptr++)
	{
		byte_out = *(cData+byteptr);
		if (IIC_WriteByte(byte_out))
		{
			IIC_Stop();
			return i2c_error;
		}
		
	}
	IIC_Stop();
	DelayShort(200);//ADD::1211
	DelayShort(200);//ADD::1211
	DelayShort(200);//ADD::1211
	
	return I2C_OK;
}
//=======================================================================
//=======================================================================
void WriteTuner(unsigned char DivH,unsigned char DivL,unsigned char StepCode,unsigned char Band)
{
	idata unsigned char bytI2cSendTemp[2];

	bytI2cSendTemp[0]=StepCode;
	bytI2cSendTemp[1]=Band;

	DevWriteTuner(DEV_FI1236_IIC_ADDR,_02BYTE,(unsigned int)MAKEWORD(DivH,DivL), bytI2cSendTemp, ADDR_2BYTE);
}
//=======================================================================
//=======================================================================
unsigned char ReadTunerOneByte(unsigned char device)
{
	unsigned char i;
	unsigned char value = 0x00;

	// Send Start
	start_con();											// Master send Start command

	// Send TunerDeviceAddr Read Mode and Read ACK
	if ( send_con(device+1) )
	{
	    scl = 1;
	    sda = 1;
	    return 0xff;
	}
	delay_nop();
	
	// Get Data
	for ( i = 0; i < 8; i++ )
	{
		scl = 0;
		_nop_(); _nop_(); _nop_(); _nop_();
		scl = 1;
		 _nop_();
		if ( sda )
			value |= 0x80 >> i;
	}
	
	// Send ACK High
	scl = 0;
	sda = 1;
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	ack_con();
	
	// Send Stop
	delay_nop();
	stop_con();
	delay_nop();

	return value;
}

//=======================================================================
//=======================================================================
void Modify_TV_Area(void)
{
	unsigned char EEP_TV_Area_BUF;

	osd_switch(0x00);// Turn the OSD T/C/B block off
	// Blinking
	osd_hbsection(title,bl,0x00,0x00,0x00,0x00);
	osd_hbsection(title,hl,0x00,0x00,0x00,0x00);
	osd_clear(0x00);
	// Select Block Color
	osd_color(title,foreground,normal,Red);				// Title
	osd_color(title,background,normal,Transparent);		// Title
	// Set position of block
	osd_pos(title,0x35,0x09);							// Set Title block position
	osd_clear(0x10);
	osd_clear(0x23);
	osd_clear(0x20);
	osd_size(title,0x2D,0x00);					// Set Title block size

	if (FLG_Power)
	{
		shi_sub_read(EEPROM, 0xF0, 0x00, &EEP_TV_Area_BUF);	// Read AirChannel or CATVChannel

		switch(EEP_TV_Area_BUF)
		{
			case AREA_USA:
				EEP_TV_Area_BUF = AREA_JAPAN;
				Osd_Show_Char(0x00,0x04,JAPAN);
				break;

			case AREA_JAPAN:
				EEP_TV_Area_BUF = AREA_GERMANY;
				Osd_Show_Char(0x00,0x06,GERMARY);
				break;

			case AREA_GERMANY:
				EEP_TV_Area_BUF = AREA_AUSTRALIA;
				Osd_Show_Char(0x00,0x08,AUSTRALIA);
				break;

			case AREA_AUSTRALIA:
				EEP_TV_Area_BUF = AREA_UK;
				Osd_Show_Char(0x00,0x01,UK);
				break;

			case AREA_UK:
				EEP_TV_Area_BUF = AREA_CHINA;
				Osd_Show_Char(0x00,0x04,CHINA);
				break;

			case AREA_CHINA:
				EEP_TV_Area_BUF = AREA_USA;
				Osd_Show_Char(0x00,0x02,USA);
				break;

			default:
				break;
		}
		shi_sub_write(EEPROM,0xF0,0x00,&EEP_TV_Area_BUF);
	}
}
