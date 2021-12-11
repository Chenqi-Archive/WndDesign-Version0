#include "./wnd/GridWnd.h"
#include "./Handlers.h"


int Init() {
	WndMngInit();

	Color color = COLOR_RED;

	SpinningBall ball;

	Fade fade;

	WndID mainWnd = CreateWnd(MASTER_WND, { 100,100 }, { 800,480 }, ball, MainWndHandler);

	color = Color(0x003f0000);
	WndID closeBtn = CreateWnd(mainWnd, { 765, 4 }, { 30,30 }, ball, MainWndHandler);

	// May support creating a system window directly and pass the HWND to WndMng
	// Or support defining and created derived Wnd classes.


	// Enter windows message loop, might be changed.
	return WndMngInitEnd();
}