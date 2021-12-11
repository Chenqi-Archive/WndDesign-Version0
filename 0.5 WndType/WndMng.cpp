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
	~WndMng() { if (wnd != nullptr && wnd != MASTER_WND_PTR) { delete wnd; } }
};

// May add some additional info to Wnd*, like its handler, message queue, etc.
static allocator<WndMng> wndMap(MAX_WND_CNT);

// Master window's message handler.
static bool MasterMsgHandler(Msg msg, void* para) {
	switch (msg)
	{
	case MSG_WND_CREATE: {
		WndInfo* wndInfo = (WndInfo*)para;
		WndMng* wndMng = new WndMng(nullptr, wndInfo->handler);

		// Try adding before newing a Wnd.
		WndID wndID;
		if (!wndMap.addItem(wndMng, wndID)) {
			// No more position!
			delete wndMng;
			return false;
		}
		wndMng->wnd = new Wnd(wndInfo->size, wndInfo->parent, wndID, wndInfo->shape);
		// Send ID, pos and size to its parent Wnd.
		SubWndInfo subWnd = { wndID, wndInfo->pos, wndInfo->size , wndMng->wnd->GetDisplay() };
		SendMsg(wndInfo->parent, MSG_WND_ADDSUBWND, &subWnd);
		// Send wndID back(using the parent id position)
		wndInfo->parent = wndID;
	}break;
	case MSG_WND_DESTROY: 
		wndMap.removeItem(*(WndID*)para);   // Automatically calls deconstructor, 
		break;                              // which destroys all subwindows and informs its parent.
	case MSG_WND_ADDSUBWND:
		// When master adds a subwindow, it creates a new system window.
		SendSysMsg(MASTER_WND, MSG_WND_CREATE, para);
		break;
	case MSG_WND_REMOVESUBWND:
		// Destroy a system window.
		SendSysMsg(MASTER_WND, MSG_WND_DESTROY, para);
		break;
	case MSG_WND_PAINT:
		SendSysMsg(*(WndID*)para, MSG_WND_PAINT, nullptr);
		break;
	case MSG_WND_MOVESUBWND: {
		MoveWndInfo* moveWnd = (MoveWndInfo*)para;
		SendSysMsg(moveWnd->subWnd, MSG_WND_MOVESUBWND, &moveWnd->vector);
	}break;
	default:
		break;
	}
	return true;
}

bool WndMngInit() {
	uint master;  // not used
	wndMap.addItem(new WndMng{ MASTER_WND_PTR, nullptr }, master);

	// May create a new thread to handle all messages.


	return true;
}

int WndMngInitEnd(){
	return EnterMsgLoop();
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

	// Mouse messages are sent to subwindow if there is.
	if (MSG_TYPE(msg, MOUSE)) {
		Pos pos = *(Pos*)para;
		const SubWndInfo* subWnd = theWnd->wnd->GetSubWnd(pos);
		// if not on subwindow, resend to handler.
		if (subWnd == nullptr) {
			return theWnd->handler(wndID, msg, para);
		}

		// Lift and repaint the subwindow


		// Get new pos of subwindow and send to subwindow.
		pos = pos - subWnd->pos;
		return SendMsg(subWnd->wndID, msg, &pos);
	}

	// WND messages are handled inside the Wnd class.
	if (MSG_TYPE(msg, WND)) {
		return theWnd->wnd->MsgHandler(msg, para);
	}

	return false;
}

WndID GetParent(WndID wndID)
{
	WndMng* theWnd;
	if (!wndMap.getItem(wndID, theWnd)) {
		return INVALID_WND_ID;
	}
	return theWnd->wnd->GetParent();
}