// WndMng.cpp:
//   Maintains all windows ID.
//   Dispatch users', wnds' and system's messages.


#include "./WndMng.h"
#include "./SysWndMng.h"
#include "./lib/allocator.h"
#include "./Wnd.h"

#define MAX_WND_CNT   1024

#define MASTER_WND_PTR     (Wnd*)-1

struct WndMng {
	Wnd* wnd;
	WndMsgHandler handler;
	// Message queue
	WndMng(Wnd* wnd, WndMsgHandler handler) :wnd(wnd), handler(handler) {}
	~WndMng() { if (wnd != MASTER_WND_PTR) { delete wnd; } }
};

// May add some additional info to Wnd*, like its handler, message queue, etc.
static allocator<WndMng> wndMap(MAX_WND_CNT);

// Master window's message handler.
static bool MasterMsgHandler(Msg msg, void* para) {
	switch (msg)
	{
	case MSG_WND_CREATE: {
		// Get WndInfo.
		WndInfo* wndInfo = (WndInfo*)para;
		// Add a new Wnd and get its ID.
		WndMng* wndMng = new WndMng(new Wnd(wndInfo->size, wndInfo->parent), wndInfo->handler);
		WndID wndID;
		if (!wndMap.addItem(wndMng, wndID)) {
			// No more position!
			delete wndMng;
			return false;
		}
		// Send ID, pos and size to its parent Wnd.
		SubWndInfo subWnd = { wndID, wndInfo->pos, wndInfo->size };
		SendMsg(wndInfo->parent, MSG_WND_ADDSUBWND, &subWnd);
	}break;
	case MSG_WND_DESTROY:
		break;
	case MSG_WND_ADDSUBWND: {
		// When master adds a subwindow, it creates a new system window.
		SubWndInfo* subWnd = (SubWndInfo*)para;
		SysWndInfo sysWnd = { subWnd->pos,subWnd->size };
		SendSysMsg(subWnd->wndID, MSG_SYS_CREATE, &sysWnd);
	}break;
	case MSG_WND_PAINT:
		SendSysMsg(*(WndID*)para, MSG_SYS_PAINT, nullptr);
		break;
	case MSG_WND_MOVESUBWND: {
		MoveWndInfo* moveWnd = (MoveWndInfo*)para;
		SendSysMsg(moveWnd->subWnd, MSG_SYS_MOVE, &moveWnd->vector);
	}break;
	default:
		break;
	}
	return false;
}

bool WndMngInit() {
	uint master;  // not used
	wndMap.addItem(new WndMng{ MASTER_WND_PTR, nullptr }, master);

	// May create a new thread to handle all messages.


	return true;
}

bool WndMngInitEnd()
{
	return SendSysMsg(MASTER_WND, MSG_SYS_MSGLOOP, nullptr);
}

bool SendMsg(WndID wndID, Msg msg, void* para)
{
	// Check if wndMap is already initialized and master window is added.

	// The master's handler is exceptional.
	if (wndID == MASTER_WND) {
		return MasterMsgHandler(msg, para);
	}

	// Get Wnd and send the message to the Wnd's message handler.
	WndMng* theWnd;
	if(!wndMap.getItem(wndID, theWnd)){
		// Wnd not found
		return false;
	}

	// WND messages are handled inside the Wnd class.
	if (MSG_TYPE(msg, WND)) {
		return theWnd->wnd->MsgHandler(wndID, msg, para);
	}

	if (MSG_TYPE(msg, SYS)) {
		// Do not receive system messages here!
		return false;
	}
	
	// Call handlers manually.
	return theWnd->handler(wndID, msg, para);
}
