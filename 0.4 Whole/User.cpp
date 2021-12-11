#include "User.h"

WndID CreateWnd(WndID parent, Pos pos, Size size, WndMsgHandler handler)
{
	WndInfo wnd = {parent, pos, size, handler};
	if (!SendMsg(MASTER_WND, MSG_WND_CREATE, &wnd)) {
		return INVALID_WND_ID;
	}
	return wnd.parent;
}
