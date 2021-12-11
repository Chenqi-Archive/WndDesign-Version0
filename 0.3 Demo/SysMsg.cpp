#include "Wnd.h"
#include <Windows.h>

#define MSG_SYS_GETWND   0x02
#define MSG_SYS_GETHWND  0x03


typedef struct { HWND hWnd; Wnd* wnd; } WndPair;
List<WndPair> pairs;

static void WndMapInsert(const WndPair& pair) {
	pairs.push_back(pair);
}

static Wnd* WndMapGetWnd(HWND hWnd) {
	for (auto it = pairs.begin(); it; ++it) {
		const WndPair& pair = it.Item();
		if (pair.hWnd == hWnd) {
			return pair.wnd;
		}
	}
	// Error!
	return NULL;
}

static HWND WndMapGetHWND(Wnd* wnd) {
	for (auto it = pairs.begin(); it; ++it) {
		const WndPair& pair = it.Item();
		if (pair.wnd == wnd) {
			return pair.hWnd;
		}
	}
	// Error!
	return NULL;
}

// wnd is the message receiver, fill NULL if sender is system.
bool SendSysMsg(Wnd* wnd, uint msg, void* para) {
	if (msg == 0) {
		HWND hWnd = WndMapGetHWND(wnd);
		PostMessage(hWnd, WM_DESTROY, 0, 0); // Deadlock when use SendMessage.
	}
	return true;
}