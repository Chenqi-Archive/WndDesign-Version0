#include "./Handlers.h"


int Init() {
	WndMngInit();

	Color color = COLOR_GREEN;
	WndID mainWnd = CreateWnd(MASTER_WND, { 100,100 }, { 800,480 }, color, MainWndHandler);

	color = COLOR_RED;
	WndID closeBtn = CreateWnd(mainWnd, { 765, 5 }, { 30,30 }, color, CloseBtnHandler);

	// May support creating a system window directly and pass the HWND
	//   to the 


	// Enter windows message loop, might be changed.
	return WndMngInitEnd();
}