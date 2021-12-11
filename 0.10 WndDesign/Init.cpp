#pragma once

#include "User.h"
#include <cstdlib>

using namespace WndDesign;

bool MainWndHandler(WID wid, Msg msg, void* para) {
	switch (msg) {
		case Msg::MOUSE_LEFTUP:{
			Color color = Color32{rand() % 0xff,rand() % 0xff,rand() % 0xff, 0x00};
			SendMsg(wid, Msg::WND_SETBG, &color);
		}break;
		default:
			break;
	}
	return true;
}

void Init() {
	InitWndMng();

	Color color = COLOR_BLUE;

	WID mainWnd = CreateWnd(MASTER_WID, WndType::FINAL, {{100,100}, {800,480}}, color, MainWndHandler);

}