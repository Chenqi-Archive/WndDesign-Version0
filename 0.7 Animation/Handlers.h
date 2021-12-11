// This file defines message handlers for all windows created,
//  may finally be packed into a dll to load dynamically.

#pragma once

#include "./User.h"

#include <cstdlib>


// You should only send WND messages here, or you will be in dead loop.
bool MainWndHandler(WndID wndID, Msg msg, void* para) {
	static enum { HOVER, LEFTDOWN }mouseState = HOVER;	// Current mouse state.
	static Pos mouseDownPos = {};  // Mouse pos when down
	static bool moveFlag = false;
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
			moveFlag = true;
		}
	}break;
	case MSG_MOUSE_LEFTDOWN: {
		moveFlag = false;
		mouseState = LEFTDOWN;
		mouseDownPos = mouseCurPos;
	}break;
	case MSG_MOUSE_LEFTUP: {
		if (mouseState == LEFTDOWN && !moveFlag) {
			// Just a demo: change color only when both clicks down and up at the same position.
			Color color = Color32{ rand() % 0xff,rand() % 0xff,rand() % 0xff, 0x00 };
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
	case MSG_MOUSE_MOVEON: {
		// Change color when first move on.
		if (mouseState == LEFTDOWN) {
			return true;
		}
		Color color = Color(0x007f0000);
		SendMsg(wndID, MSG_WND_CHANGEBG, &color);
	}break;
	case MSG_MOUSE_MOVEOFF: {
		// Change color back when move off.
		Color color = Color(0x003f0000);
		SendMsg(wndID, MSG_WND_CHANGEBG, &color);
		mouseState = HOVER;
	}break;
	case MSG_MOUSE_LEFTDOWN: {
		mouseState = LEFTDOWN;
		// Change color when click down.
		Color color = Color(0x00ff0000);
		SendMsg(wndID, MSG_WND_CHANGEBG, &color);
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
