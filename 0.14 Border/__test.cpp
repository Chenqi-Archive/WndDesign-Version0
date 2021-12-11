#include "../WndDesign/WndDesign.h"

#pragma comment(lib, "../bin/Win32/Debug/WndDesign.lib")

using namespace WndDesign;

void Init() {
	Wnd* mainwnd = new FinalWnd(new Color(COLOR_RED));

	master->AddSubWnd(mainwnd, { {100,100}, {800,480} });

	mainwnd->SetBorder(Pen(COLOR_BLACK, 3));

	mainwnd->Paint();
}