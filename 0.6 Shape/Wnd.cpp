#include "Wnd.h"
#include "WndMng.h"

bool Wnd::AddSubWnd(const SubWndInfo& subWndInfo){
	if (subWnds.size() == 0) {
		// The first subwindow, initialize subWndIndex
		subWndIndex.Init(display.GetSize(), nullptr);
	}
	SubWndInfo& subWnd = subWnds.push_back(subWndInfo);
	// Update display and index.
	subWnd.display.PaintOn(display, { subWnd.pos, subWnd.size });
	subWndIndex.Fill(subWnd.pos, subWnd.size, &subWnd);

	// Notify parent window to redraw.
	WndID self = myself;
	return SendMsg(parent, MSG_WND_PAINT, &self);
}

bool Wnd::RemoveSubWnd(WndID wndID){
	// If on destroy, there is no need to repaint.
	if (!DestroyFlag) {
		// Paint background first.
		display = background;
		subWndIndex.Fill({ 0,0 }, subWndIndex.GetSize(), nullptr);
	}


	// May first find the wnd, remove it, and repaint all. (With invalid rect region as parameter)


	for (auto it = subWnds.begin(); it;) {
		SubWndInfo& subWnd = it.Item();
		if (subWnd.wndID == wndID) {
			subWnds.remove(it);
			continue;
		}
		if (!DestroyFlag) {
			subWnd.display.PaintOn(display, { subWnd.pos, subWnd.size });
			subWndIndex.Fill(subWnd.pos, subWnd.size, &subWnd);
		}
		++it;
	}

	// Check if subWnds is empty. If so, clear subWndIndex, or it will be newed twice when adding a new subwindow.


	// Notify parent window to redraw.
	if (!DestroyFlag) {
		WndID self = myself;
		return SendMsg(parent, MSG_WND_PAINT, &self);
	}

	return true;
}

bool Wnd::PaintSubWnd(WndID wndID){
	// Find the subwindow.
	auto it = subWnds.begin();
	for (; it; ++it) {
		const SubWndInfo& subWnd = it.Item();
		if (subWnd.wndID == wndID) {
			break;
		}
	}
	if (!it) { return false; }  // Not found!
	// Repaint all windows above the subwindow.
	for (; it; ++it) {
		const SubWndInfo& subWnd = it.Item();
		subWnd.display.PaintOn(display, { subWnd.pos, subWnd.size });
		// No need to update index.
	}
	// Notify parent window to redraw.
	WndID self = myself;
	return SendMsg(parent, MSG_WND_PAINT, &self);
}

inline bool Wnd::MoveSubWnd(WndID wndID, Pos vector)
{		
	// Reapint all subwindows. Can calculate all invalid rects for optimization.
	// First paint background.(Need to update index)
	display = background;
	subWndIndex.Fill({ 0,0 }, subWndIndex.GetSize(), nullptr);  // Costy
	for (auto it = subWnds.begin(); it; ++it) {
		SubWndInfo& subWnd = it.Item();
		if (subWnd.wndID == wndID) {
			subWnd.pos = subWnd.pos + vector;
		}
		subWnd.display.PaintOn(display, { subWnd.pos, subWnd.size });
		subWndIndex.Fill(subWnd.pos, subWnd.size, &subWnd);
	}
	// Notify parent window to redraw.
	WndID self = myself;
	return SendMsg(parent, MSG_WND_PAINT, &self);
}

bool Wnd::ChangeBG(const Shape& shape){
	shape.PaintOn(display);  // Copied twice, can be optimized.

	// Repaint all subwindows
	auto it = subWnds.begin();
	if (it) {
		return PaintSubWnd(it.Item().wndID);
	}
	else {
		// Notify parent window to redraw.
		WndID self = myself;
		return SendMsg(parent, MSG_WND_PAINT, &self);
	}
}

Wnd::Wnd(Size size, WndID parent, WndID myself, Shape& shape):parent(parent),myself(myself),background(size){
	// May add some additional features.

	shape.PaintOn(background);
	display = background;
}

Wnd::~Wnd(){
	DestroyFlag = 1;

	// Destroy all subwindows.
	for (auto it = subWnds.begin(); it; ) {
		WndID sub = it.Item().wndID;
		++it; // The content pointed will be deleted! Point to next earlier.
		SendMsg(MASTER_WND, MSG_WND_DESTROY, &sub);
	}
	// Inform parent window.
	WndID self = myself;
	SendMsg(parent, MSG_WND_REMOVESUBWND, &self);
}

bool Wnd::MsgHandler(Msg msg, void* para) {
	switch (msg)
	{
	case MSG_WND_ADDSUBWND: AddSubWnd(*(SubWndInfo*)para);break;
	case MSG_WND_REMOVESUBWND: RemoveSubWnd(*(WndID*)para); break;
	case MSG_WND_CHANGEBG: ChangeBG(*(Shape*)para);break;
	case MSG_WND_PAINT: PaintSubWnd(*(WndID*)para);break;
	case MSG_WND_MOVESUBWND: MoveSubWnd(((MoveWndInfo*)para)->subWnd, ((MoveWndInfo*)para)->vector); break;
	default:break;
	}
	return true;
}