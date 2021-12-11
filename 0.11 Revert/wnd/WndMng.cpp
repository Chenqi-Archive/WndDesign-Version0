#include "WndMng.h"
#include "BasicWnds.h"
#include "../sys/SysWndMng.h"
#include "../lib/allocator.h"

namespace WndDesign {

#define MAX_WND_CNT   1024
#define MASTER_WND_PTR     (Wnd*)-1

struct WndItem {
	Wnd* wnd;
	MsgHandler handler;
	WndItem(Wnd* wnd, MsgHandler handler):wnd(wnd), handler(handler) {}
	~WndItem() { if (wnd != nullptr && wnd != MASTER_WND_PTR) { delete wnd; } }
};

static allocator<WndItem> wndmap(MAX_WND_CNT);

static bool MasterMsgHandler(Msg msg, void* para) {
	return true;
}

bool InitWndMng() {
	uint master;
	wndmap.addItem(new WndItem{MASTER_WND_PTR, nullptr}, master);
	return true;
}

bool InitEnd() {
	if (wndmap.size() <= 1) {
		return false;
	}
	return true;
}

WID CreateWnd(WID parent, WndType type, Rect region, Figure&& background, MsgHandler handler) {
	WndItem* item = new WndItem(nullptr, handler);
	WID wid;
	if (!wndmap.addItem(item, wid)) {
		delete item; // No more position!
		return INVALID_WID;
	}
	switch (type) {
		case WndType::FINAL:item->wnd = new FinalWnd; break;
		//case WndType::GRID:wnd = new GridWnd; break;
		case WndType::SPLIT:item->wnd = new SplitWnd; break;
		case WndType::TILE:item->wnd = new TileWnd; break;
		case WndType::OVERLAP:item->wnd = new OverlapWnd; break;
		default: wndmap.removeItem(wid); return INVALID_WID;
	}
	// Inform parent window to add the subwnd and allocate its board.
	if (parent == MASTER) {
		CreateSysWnd(wid, item->wnd, region);
	} else {
		WndItem* pitem;
		wndmap.getItem(parent, pitem);
		if (!pitem->wnd->AddSubWnd(wid, item->wnd, region)) {
			wndmap.removeItem(wid);
			return INVALID_WID;
		}
	}
	item->wnd->SetBackground(&background);
	return wid;
}

WID RegisterWnd(WID parent, Wnd* wnd, Rect region, MsgHandler handler) {
	WndItem* item = new WndItem(nullptr, handler);
	WID wid;
	if (!wndmap.addItem(item, wid)) {
		delete item;
		return INVALID_WID;
	}
	item->wnd = wnd;
	if (parent == MASTER) {
		CreateSysWnd(wid, item->wnd, region);
	} else {
		WndItem* pitem;
		wndmap.getItem(parent, pitem);
		if (!pitem->wnd->AddSubWnd(wid, item->wnd, region)) {
			wndmap.removeItem(wid);
			return INVALID_WID;
		}
	}
	wnd->Paint();
	return wid;
}

bool DestroyWnd(WID wid) {
	DestroySysWnd(wid);
	return wndmap.removeItem(wid);
}

bool SendMsg(WID wid, Msg msg, void* para) {
	if (wid == MASTER) { return MasterMsgHandler(msg, para); }
	WndItem* item;
	if (!wndmap.getItem(wid, item)) { return false; }

	if (item->wnd->MsgHandler(msg, para)) { return true; }
	if (item->handler&&item->handler(wid, msg, para)) { return true; }
	return false;
}

} // namespace WndDesign