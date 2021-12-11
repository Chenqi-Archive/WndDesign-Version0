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

			// Get parent window
			WndID parent;
			SendMsg(wndID, MSG_WND_PARENT, &parent);

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
		// Just a demo: change color when clicks.
		Color32 color = { rand() % 0xff, rand() % 0xff, rand() % 0xff, 0x00 };
		SendMsg(wndID, MSG_WND_CHANGECOLOR, &color);

		mouseState = HOVER;
	}break;
	default:
		break;
	}

	return false;
}


