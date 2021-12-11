#include "./SysWndMng.h"
#include "./WndMng.h"
#include "./WinAPI.h"

typedef struct {
	WndID wnd;
	HWND hWnd;
}WndPair;

list<WndPair> sysWndList;

static WndID GetWndID(HWND hWnd) {
	// Linear search.
	for (auto it = sysWndList.begin(); it; ++it) {
		const WndPair& pair = it.Item();
		if (pair.hWnd == hWnd) {
			return pair.wnd;
		}
	}
	// Not found!
	return INVALID_WND_ID;
}

static HWND GetHWND(WndID wnd) {
	for (auto it = sysWndList.begin(); it; ++it) {
		const WndPair& pair = it.Item();
		if (pair.wnd == wnd) {
			return pair.hWnd;
		}
	}
	// Not found!
	return NULL;
}

static bool CreateSysWnd(WndID wnd, Pos pos, Size size) {
	HWND hWnd = SysCreateWnd(pos, size);
	if (hWnd == NULL) {
		return false;
	}
	sysWndList.push_back({ wnd, hWnd });
	// Must show window after mapping created, or the hWnd will not be found.
	SysShowWnd(hWnd);
	return true;
}

static bool DestroySysWnd(WndID wnd) {
	HWND hWnd = GetHWND(wnd);
	if (hWnd == NULL) {
		// Wnd not found!
		return false;
	}
	
	// Post destroy message

	// Delete wnd pair from list

	return true;
}

// Actually, WndID should not be used!
bool SendSysMsg(WndID wndID, Msg msg, void* para)
{
	HWND hWnd = GetHWND(wndID);

	switch (msg) {
	case MSG_SYS_CREATE: {
		SysWndInfo* sysWnd = (SysWndInfo*)para;
		if (!CreateSysWnd(wndID, sysWnd->pos, sysWnd->size)) {
			return false;
		}
	}break;
	case MSG_SYS_MSGLOOP:
		return SysMsgLoop();
		break;
	case MSG_SYS_PAINT: {  // para : NULL
		//HWND hWnd = GetHWND(wndID);
		//if (hWnd == NULL) {
		//	return false;
		//}
		//// Get the display bitmap of the window
		//void* bits;
		//SendMsg(wndID, MSG_WND_DISPLAY, &bits);
		//// if (bits == NULL)
		//SysPaintWnd(hWnd, bits);


		// It seem that we can't directly paint the window, 
		//   must post the system paint message before.

		InvalidateRect(hWnd, NULL, true);
		UpdateWindow(hWnd);
	}break;
	case MSG_SYS_MOVE:
		SysMoveWnd(hWnd, *(Pos*)para);
		break;
	default:
		return false;
	}
	return true;
}

bool ReceiveSysMsg(HWND hWnd, Msg msg, void* para)
{
	WndID wndID = GetWndID(hWnd);
	if (wndID == INVALID_WND_ID) {
		// Wnd not found! There must be something wrong!
		return false;
	}

	if (MSG_TYPE(msg, MOUSE)) {
		return SendMsg(wndID, msg, para);
	}

	switch (msg) {
	case MSG_SYS_PAINT: {
		// Get the display bitmap of the window
		void* bits;
		SendMsg(wndID, MSG_WND_DISPLAY, &bits);
		// if (bits == NULL)
		SysPaintWnd(hWnd, bits);
	}break;
	default:
		return false;
	}
	return true;
}
