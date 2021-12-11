#include "./Handlers.h"


int Init() {
	WndMngInit();

	Color color = COLOR_RED;

	Circle circle;

	RoundedRectangle rect(70);

	WndID mainWnd = CreateWnd(MASTER_WND, { 100,100 }, { 800,480 }, rect, MainWndHandler);

	color = COLOR_GREEN;
	WndID closeBtn = CreateWnd(mainWnd, { 765, 4 }, { 30,30 }, color, CloseBtnHandler);

	// May support creating a system window directly and pass the HWND to WndMng
	// Or support defining and created derived Wnd classes.


	// Enter windows message loop, might be changed.
	return WndMngInitEnd();
}