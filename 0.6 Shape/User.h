// User.h:
//   Defines some wrapper functions of messages for comvenient user calls.

#pragma once

#include "./WndMng.h"
#include "./shape/BasicShapes.h"

inline WndID CreateWnd(WndID parent, Pos pos, Size size, Shape& shape, WndMsgHandler handler)
{
	WndInfo wnd = { parent, pos, size, shape, handler };
	if (!SendMsg(MASTER_WND, MSG_WND_CREATE, &wnd)) {
		return INVALID_WND_ID;
	}
	return wnd.parent;
}

inline bool DestroyWnd(WndID wndID) {
	return SendMsg(MASTER_WND, MSG_WND_DESTROY, &wndID);
}


// May also support read a Wnd defination file and create multiple windows on init.
// Thus subwindows can be created before system window appears.

