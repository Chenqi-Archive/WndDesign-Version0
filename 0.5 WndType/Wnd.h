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

	// May also keep a self WndID.

	list<SubWndInfo> subWnds;

	class WndIndex :public matrix<SubWndInfo*> {
	public:
		void PasteOn(Pos pos, Size size, SubWndInfo* subWnd) {
			for (uint j = 0; j < size.height; ++j) {
				for (uint i = 0; i < size.width; ++i) {
					Set(i + pos.x, j + pos.y, subWnd);
				}
			}
		}
	} subWndIndex;

	Bitmap background;  // Too heavy, may use a generator instead.
	Bitmap display;

private:
	void InitDisplay();
	void InitSubWndIndex();

	bool AddSubWnd(const SubWndInfo& subWndInfo);
	bool RemoveSubWnd(WndID wndID);
	bool PaintSubWnd(WndID wndID);
	inline bool MoveSubWnd(WndID wndID, Pos vector);
	bool ChangeBG(const Shape& shape);

public:
	Wnd(Size size, WndID parent, WndID myself, Shape& shape);

	~Wnd();

	// Find the subWnd at pos by subWndIndex, if no subwindow, return nullptr.
	const SubWndInfo* GetSubWnd(Pos pos);

	Bitmap& GetDisplay();
	WndID GetParent();

	// This handler is only used for Wnd messages, and can be derived 
	//   for user defined sub classes and messages.
	// Not virtual, and can be redefined in derived classes.
	bool MsgHandler(Msg msg, void* para);
};


class Button :public Wnd {

};