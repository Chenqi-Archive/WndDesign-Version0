// Wnd.h:
// Defines the Wnd class.
// windows can only communicate with sending messages.

#pragma once

#include "./lib/list.h"
#include "./lib/matrix.h"
#include "./shape/Bitmap.h"
#include "./types.h"

class Wnd;

struct SubWndInfo {
	WndID wndID;
	Pos pos;  // Relative to left top corner, could be negetive.
	Size size;  // Size is both saved by parent and sub windows.
};

class Wnd {
private:
	const WndID parent;

	// May also keep a self WndID.

	list<SubWndInfo> subWnds;
	class WndIndex :public matrix<SubWndInfo*> {} subWndIndex;

	Bitmap background;  // Too heavy, may use a generator instead.
	Bitmap display;

private:
	void InitDisplay();
	void InitSubWndIndex();

	void AddSubWnd(const SubWndInfo& subWndInfo);

	// Find the subWnd at pos by subWndIndex, if no subwindow, return INVALID_WND_ID
	WndID GetSubWnd(Pos pos);

public:
	Wnd(Size size, WndID parent);

	// This handler is only used for Wnd messages, and can be derived 
	//   for user defined sub classes and messages.
	// Not virtual, and can be redefined in derived classes.
	bool MsgHandler(WndID self, Msg msg, void* para);
};


class Button :public Wnd {

};