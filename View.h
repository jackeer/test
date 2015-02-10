
#ifndef _VIEW_H_
#define _VIEW_H_

#include <Test.h>
#include <ASSERT.H>
#include <Typedef.h>

#define SUPPORT_VIEW_OTHER_FUNC 1
#define ACTIVEVIEW_MAX 5

enum ViewType {
	View_Base_View,
	View_ACT1,
	View_ACT2,
	View_ACT3,
	View_ACT4,
	VIEW_MAX
};

typedef struct structView {
	void (*vInit) (void) ;
	BOOL (*fgDo) (void) ;
	void (*vExit) (void) ;
} strucView;

enum ViewState {
	VIEW_STATE_EXIT,
	VIEW_STATE_INIT,
	VIEW_STATE_RUNNING
};

extern BYTE bCheckView(BYTE bView) large;
#if SUPPORT_VIEW_OTHER_FUNC
extern BYTE bGetFocusView(void) large;
#endif
extern void vViewInit(void) large;
#if SUPPORT_VIEW_OTHER_FUNC
extern void vViewUninit(void) large;
#endif
extern void vViewProcess(void) large;
extern void vEnterView(BYTE bView) large;
extern void vExitFocusView(void) large;

#endif

