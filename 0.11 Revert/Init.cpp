#pragma once

#include "WndDesign.h"
#include <cstdlib>

using namespace WndDesign;

WID mainWnd;
WID closeBtn;

bool HHH(WID wid, Msg msg, void* para) {
	return true;
}

bool MainWndHandler(WID wid, Msg msg, void* para) {
	switch (msg) {
		case Msg::MOUSE_LEFTUP: {
			Color color = Color32{rand() % 0xff,rand() % 0xff,rand() % 0xff, 0x00};
			SendMsg(wid, Msg::WND_SETBG, &color);
		}break;
		case Msg::MOUSE_RIGHTUP: {
			Pos pos = *(Pos*)para;
			CreateWnd(wid, WndType::FINAL, {pos, {100,200}}, Color(COLOR_WHITE), HHH);
		}break;
		default:
			break;
	}
	return true;
}

bool CloseBtnHandler(WID wid, Msg msg, void* para) {
	static enum { HOVER, LEFTDOWN }mouseState = HOVER;
	switch (msg) {
		case Msg::MOUSE_MOVEON: {
			if (mouseState == LEFTDOWN) {
				break;
			}
			Color color = Color(0x00007f00);
			SendMsg(wid, Msg::WND_SETBG, &color);
		}break;
		case Msg::MOUSE_MOVEOFF: {
			Color color = Color(0x00003f00);
			SendMsg(wid, Msg::WND_SETBG, &color);
			mouseState = HOVER;
		}break;
		case Msg::MOUSE_LEFTDOWN: {
			mouseState = LEFTDOWN;
			Color color = Color(0x0000ff00);
			SendMsg(wid, Msg::WND_SETBG, &color);
		}break;
		case Msg::MOUSE_LEFTUP: {
			if (mouseState == LEFTDOWN) {
				DestroyWnd(mainWnd);
			}
			mouseState = HOVER;
		}break;
		default:return false;
	}
	return true;
}

#define Init aciaafe

void Init() {

	mainWnd = CreateWnd(MASTER, WndType::TILE, {{100,100}, {800,480}}, (Color)COLOR_RED, MainWndHandler);
	if (mainWnd == INVALID_WID) { return; }

	closeBtn = CreateWnd(mainWnd, WndType::FINAL, {{ 765, 4 }, { 30,30 }}, Clock(), CloseBtnHandler);

}