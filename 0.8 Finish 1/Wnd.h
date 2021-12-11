// Wnd.h:
// Defines the Wnd class.
// windows can only communicate with sending messages.

#pragma once

#include "./lib/list.h"
#include "./lib/matrix.h"
#include "./shape/Bitmap.h"
#include "./wndtypes.h"

class Wnd {
private:
	const WndID parent;
	const WndID myself;

	bool DestroyFlag = 0;

	list<SubWndInfo> subWnds;

	class WndIndex :public matrix<SubWndInfo*> {
	public:
		void Fill(Pos pos, Size size, SubWndInfo* subWnd) {
			for (uint j = 0; j < size.height; ++j) {
				for (uint i = 0; i < size.width; ++i) {
					Set(i + pos.x, j + pos.y, subWnd);
				}
			}
		}
	} subWndIndex;

	Figure* background;
	Bitmap display;

private:
	//void InitDisplay();
	//void InitSubWndIndex();

	bool AddSubWnd(const SubWndInfo& subWndInfo);
	bool RemoveSubWnd(WndID wndID);
	bool PaintSubWnd(WndID wndID);
	inline bool MoveSubWnd(WndID wndID, Pos vector);
	bool ChangeBG(const Figure& figure);
	bool RefreshBG();

public:
	Wnd(Size size, WndID parent, WndID myself, const Figure& figure);

	~Wnd();

	// Find the subWnd at pos by subWndIndex, if no subwindow, return nullptr.
	inline const SubWndInfo* GetSubWnd(Pos pos) {
		SubWndInfo* subWnd = nullptr;  // SubWindowIndex may not be initialized.
		subWndIndex.Get(pos.x, pos.y, subWnd);
		return subWnd;
	}

	inline Bitmap& GetDisplay() { return display; }
	inline WndID GetParent() { return parent; }

	// This handler is only used for Wnd messages, and can be derived 
	//   for user defined sub classes and messages.
	// Not virtual, and can be redefined in derived classes.
	bool MsgHandler(Msg msg, void* para);
};