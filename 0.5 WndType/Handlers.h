// This file defines message handlers for all windows created,
//  may finally be packed into a dll to load dynamically.

#pragma once

#include "./User.h"

#include <cstdlib>


// You should only send WND messages here, or you will be in dead loop.
bool MainWndHandler(WndID wndID, Msg msg, void* para) {
	static enum { HOVER, LEFTDOWN }mouseState = HOVER;	// Current mouse state.
	static Pos mouseDownPos = {};  // Mouse pos when down
	Pos mouseCurPos = *(Pos*)para;
	switch (msg)
	{
	case MSG_MOUSE_MOVEON: {
		if (mouseState == LEFTDOWN) {
			// Move the window. Send move message to parent window.

			// Wrap the message to InformParent(WndID self, Msg msg, void* para)
			WndID parent = GetParent(wndID);
			MoveWndInfo moveWnd = { wndID, mouseCurPos - mouseDownPos };
			SendMsg(parent, MSG_WND_MOVESUBWND, &moveWnd);

			// Too complicated! May reorder the message handlers.
		}
	}break;
	case MSG_MOUSE_LEFTDOWN: {
		mouseState = LEFTDOWN;
		mouseDownPos = mouseCurPos;
	}break;
	case MSG_MOUSE_LEFTUP: {
		if (mouseState == LEFTDOWN) {
			// Just a demo: change color only when both clicks down and up.
			Color color = Color32{ uchar(rand() % 0xff), uchar(rand() % 0xff),uchar(rand() % 0xff), 0x00 };
			SendMsg(wndID, MSG_WND_CHANGEBG, &color);
		}
		mouseState = HOVER;
	}break;
	default:
		break;
	}

	return false;
}

bool CloseBtnHandler(WndID wndID, Msg msg, void* para) {
	static enum { HOVER, LEFTDOWN }mouseState = HOVER;	// Current mouse state.
	switch (msg)
	{
	case MSG_MOUSE_LEFTDOWN: {
		mouseState = LEFTDOWN;
	}break;
	case MSG_MOUSE_LEFTUP: {
		if (mouseState == LEFTDOWN) {
			return DestroyWnd(GetParent(wndID));
		}
		mouseState = HOVER;
	}break;
	default:
		break;
	}
	return true;
}
