#ifndef _TUNER_H_
#define  _TUNER_H_
//--------------------------------------------------------------------------------------------
#define LG_TunerAddr 0xc6
#define PH_TunerAddr 0xc0
#define LISTEN_TunerAddr 0xC0
#define TunerTimer PoSvTimer
///#define VHF 0x1075
///#define UHF 0x264b
///#define VHF 3008
///#define UHF 7808
//--------------------------------------------------------------------------------------------
enum TV_AREA
{
	AREA_USA=0,
	AREA_JAPAN,
	AREA_GERMANY,
	AREA_AUSTRALIA,
	AREA_UK,
	AREA_CHINA
};
//--------------------------------------------------------------------------------------------
void ChannelSel(unsigned char ch);
void WriteTuner(unsigned char DivH,unsigned char DivL,unsigned char StepCode,unsigned char Band);
///unsigned char ReadTuner(void);
void SetTuner(void);
void ChannelUpDown(bit Direct);
void ChNumber(void);
void CheckTuner(void);
unsigned char CheckChMask(unsigned char Mask);
void ScanChannel(void);
void LoadChLimit(void);
//--------------------------------------------------------------------------------------------
///extern UNI_DATA_DEF idata UNI_Sel;
//--------------------------------------------------------------------------------------------
#endif
