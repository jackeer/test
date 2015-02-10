
#include<View.h>
#include <Def_INC.h>
#include <IIC.h>

BYTE xdata _bViewState[ACTIVEVIEW_MAX];
BYTE xdata _bActiveView[ACTIVEVIEW_MAX];
BYTE xdata _bTempForView;
BOOL Led_Act;

extern BYTE xdata _bFocusViewIndex;
extern BYTE xdata _bKeyProcessViewIndex;

extern BYTE xdata UI_kwy_buffer;
extern BYTE xdata  STATUS_TSK;
extern WORD xdata Time_Count;
extern BOOL keypress;

BOOL fgBaseViewState(void) large
{
	if(P0 != 0xFF)
		{
		if(!keypress)
			{
			if(!key_Power)
				vEnterView(View_ACT2);
			else if(!key_shink)
				{
				vEnterView(View_ACT1);
				Led_Act = 1;
				}
			/*			else if(!key_shift_stop)
				{
				//			vExitFocusView();
				Led_Act = 0;
				}
			*/
			else if(!key5 || !key6 || !key7 || !key8)
				UI_kwy_buffer = P0;

			if(!key_shift)
				{
				UI_kwy_buffer = P2;
				UI_kwy_buffer = ~UI_kwy_buffer;
				UI_kwy_buffer <<= 1;
				UI_kwy_buffer = ~UI_kwy_buffer;
				keypress = 1;
				}
			}
		}
			else
				keypress = 0;

	return TRUE;

}
void vExitBaseView(void) large
{

}
void fgInitACT1ViewState(void) large
{

}
BOOL fgACT1ViewState(void) large
{
	if(!key_shift_stop || !key_Power)
		{
		vExitFocusView();
		Led_Act = 0;
		}
	if(Led_Act)
		{
		if((++Time_Count) > 1300L)
			{
			Time_Count = 0;
			if(UI_kwy_buffer == 0x0F)
				UI_kwy_buffer = 0xF6;
			else if(UI_kwy_buffer == 0xF6)
				UI_kwy_buffer = 0xF7;
			else if(!led4)
				UI_kwy_buffer = 0x0F;
			else
				{
				P2 = 0xFF;
				UI_kwy_buffer = ~UI_kwy_buffer;
				UI_kwy_buffer <<= 1;
				UI_kwy_buffer = ~UI_kwy_buffer;
				}
			}
		}
	return TRUE;
}
void fgExitACT1ViewState(void) large
{

}
void fgInitACT2ViewState(void) large
{

}
BOOL fgACT2ViewState(void) large
{
if(bGetFocusView() != View_Base_View)
	{
	vViewUninit();
	}

	if(key_Power == 0)
		{
		keypress = 1;
		STATUS_TSK = UI_STANDBY;
		}

	return TRUE;
}
void fgExitACT2ViewState(void) large
{

}
strucView CODE rView[VIEW_MAX] =
{
	{NULL_VIEW,fgBaseViewState,vExitBaseView}
	,{fgInitACT1ViewState,fgACT1ViewState,fgExitACT1ViewState}
	,{fgInitACT2ViewState,fgACT2ViewState,fgExitACT2ViewState}
	,{NULL_VIEW,NULL_VIEW,NULL_VIEW}
	,{NULL_VIEW,NULL_VIEW,NULL_VIEW}
};

/*****************************************************
Internal functions
*****************************************************/
BYTE bCheckView(BYTE bView) large
{
	BYTE bTempViewIdx;
	for (bTempViewIdx = _bFocusViewIndex + 1; bTempViewIdx > 0; bTempViewIdx--)
	{
		assert(bTempViewIdx > 0);
		if (bView == _bActiveView[bTempViewIdx - 1])
		{
			return _bViewState[bTempViewIdx - 1] ;
		}
	}
	return VIEW_STATE_EXIT;
}
#if SUPPORT_VIEW_OTHER_FUNC
BYTE bGetFocusView(void) large
{
	if(_bFocusViewIndex != 0xFF) //if view stack  empty
	{
		return _bActiveView[_bFocusViewIndex];
	}
	return(VIEW_MAX);
}
#endif
void vViewInit(void) large
{
	_bFocusViewIndex = 0xFF;	//view stack empty
	_bKeyProcessViewIndex = 0xFF;
	vEnterView(View_Base_View);
}
#if SUPPORT_VIEW_OTHER_FUNC
void vViewUninit(void) large
{
	for (_bTempForView = _bFocusViewIndex + 1; _bTempForView > 0; _bTempForView--)
	{
		assert(_bTempForView > 0);
		if (rView[_bActiveView[_bTempForView - 1]].vExit)
		{
			(*rView[_bActiveView[_bTempForView - 1]].vExit)();
		}
	}
	_bFocusViewIndex = 0xFF;
}
#endif
void vViewProcess(void) large
{
	do{
	assert(_bFocusViewIndex != 0xFF);
	_bKeyProcessViewIndex = _bFocusViewIndex;
	switch (_bViewState[_bKeyProcessViewIndex])
		{
		case VIEW_STATE_RUNNING:
		if (rView[_bActiveView[_bKeyProcessViewIndex]].fgDo)
			{
			if ((*rView[_bActiveView[_bKeyProcessViewIndex]].fgDo)())
				{
				return;
				}
			}
		_bKeyProcessViewIndex--;
		break;
		default:
		break;
		}
	}while((_bKeyProcessViewIndex >= _bFocusViewIndex) && (_bKeyProcessViewIndex != 0xFF));


while(_bKeyProcessViewIndex != 0xFF)
{
	if((rView[_bActiveView[_bKeyProcessViewIndex]].fgDo) && (_bViewState[_bKeyProcessViewIndex] == VIEW_STATE_RUNNING))
	{
		if ((*rView[_bActiveView[_bKeyProcessViewIndex]].fgDo)()) //Exit view
		{
			return;
		}
	}
	assert(_bKeyProcessViewIndex > 0);
	_bKeyProcessViewIndex--;
};


}

void vEnterView(BYTE bView) large
{
	if(bView >= VIEW_MAX)
	{
		return;
	}
	if (VIEW_STATE_EXIT == bCheckView(bView))
	{
		assert((_bFocusViewIndex == 0xFF) || (_bFocusViewIndex < (ACTIVEVIEW_MAX - 1)));
		_bFocusViewIndex++;
		_bActiveView[_bFocusViewIndex] = bView;
		_bViewState[_bFocusViewIndex] = VIEW_STATE_RUNNING;
		if (rView[_bActiveView[_bFocusViewIndex]].vInit)
		{
			(*rView[_bActiveView[_bFocusViewIndex]].vInit)();
		}
	}
}

void vExitFocusView(void) large
{
	BYTE bTempViewIdx;

	assert(_bFocusViewIndex  != 0xFF);
	if (rView[_bActiveView[_bFocusViewIndex]].vExit)
	{
		(*rView[_bActiveView[_bFocusViewIndex]].vExit)();
	}

	bTempViewIdx = _bKeyProcessViewIndex;
	while(bTempViewIdx != _bFocusViewIndex)
	{
		_bActiveView[bTempViewIdx] = _bActiveView[bTempViewIdx + 1];
		_bViewState[bTempViewIdx] = _bViewState[bTempViewIdx + 1];
		bTempViewIdx++;
	}
	
	_bFocusViewIndex--;
}











