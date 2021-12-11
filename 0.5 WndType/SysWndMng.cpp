#include "./SysWndMng.h"
#include "./WndMng.h"
#include "./WinAPI.h"

struct WndPair {
	WndID wnd;
	HWND hWnd;
	const Bitmap& display;
}INVALID_WND_PAIR{ INVALID_WND_ID, NULL, Bitmap() };

typedef list<WndPair> WndPairList;
typedef listIter<WndPair> WndPairListIter;

WndPairList sysWndList;

static const WndPair& GetWndPair(WndID wndID) {
	// Linear search.
	for (auto it = sysWndList.begin(); it; ++it) {
		const WndPair& pair = it.Item();
		if (pair.wnd == wndID) {
			return pair;
		}
	}
	return INVALID_WND_PAIR;
}

static const WndPair& GetWndPair(HWND hWnd) {
	// Linear search.
	for (auto it = sysWndList.begin(); it; ++it) {
		const WndPair& pair = it.Item();
		if (pair.hWnd == hWnd) {
			return pair;
		}
	}
	return INVALID_WND_PAIR;
}

// Just like addsubwindow
static bool CreateSysWnd(const SubWndInfo& sysWnd) {
	HWND hWnd = SysCreateWnd(sysWnd.pos, sysWnd.size);
	if (hWnd == NULL) {
		return false;
	}
	sysWndList.push_back({ sysWnd.wndID, hWnd ,sysWnd.display });
	// Must show window after mapping created, or the hWnd will not be found.
	SysShowWnd(hWnd);
	return true;
}

static bool DestroySysWnd(WndID wndID) {
	auto it = sysWndList.begin();
	for (; it; ++it) {
		const WndPair& pair = it.Item();
		if (pair.wnd == wndID) {
			break;
		}
	}
	if (!it) {
		return false;
	}

	// Post destroy message
	PostMessage(it.Item().hWnd, WM_DESTROY, NULL, NULL);

	// Delete wnd pair from list
	sysWndList.remove(it);

	return true;
}

static inline void PaintSysWnd(WndID wndID) {
	HWND hWnd = GetWndPair(wndID).hWnd;
	InvalidateRect(hWnd, NULL, true);
	UpdateWindow(hWnd);
}

// Actually, WndID should not be used!
bool SendSysMsg(WndID wndID, Msg msg, void* para)
{
	switch (msg) {
	case MSG_WND_CREATE: return CreateSysWnd(*(SubWndInfo*)para); break;
	case MSG_WND_DESTROY: return DestroySysWnd(*(WndID*)para); break;
	case MSG_WND_PAINT: PaintSysWnd(wndID); break;
	case MSG_WND_MOVESUBWND: SysMoveWnd(GetWndPair(wndID).hWnd, *(Pos*)para); break;
	default:return false;
	}
	return true;
}

bool ReceiveSysMsg(HWND hWnd, Msg msg, void* para)
{
	WndPair wndPair = GetWndPair(hWnd);
	WndID wndID = wndPair.wnd;
	if (wndID == INVALID_WND_ID) {
		// Wnd not found! There must be something wrong!
		return false;
	}

	if (MSG_TYPE(msg, MOUSE)) {
		return SendMsg(wndID, msg, para);
	}

	switch (msg) {
	case MSG_WND_PAINT: SysPaintWnd(hWnd, wndPair.display); break;
	default:return false;
	}
	return true;
}


int EnterMsgLoop()
{
	return SysMsgLoop();
}