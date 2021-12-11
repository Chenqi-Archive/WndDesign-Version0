#include "./Handlers.h"


bool Init() {
	WndMngInit();

	WndID mainWnd = CreateWnd(MASTER_WND, { 100,100 }, { 800,480 }, MainWndHandler);


	// May support creating a system window directly and pass the HWND
	//   to the 


	// Enter windows message loop, might be changed.
	WndMngInitEnd();

	return true;
}