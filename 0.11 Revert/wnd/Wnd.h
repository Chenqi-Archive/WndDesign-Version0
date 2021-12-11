#pragma once

#include "../basic/wndtypes.h"

namespace WndDesign {

class Wnd :public Figure{
private:
	Figure* background = nullptr;
public:
	void SetBackground(Figure* figure) {
		background = figure;
		background->SetBoard(board);
		//background->Paint();
	}
	virtual bool AddSubWnd(WID wid, Wnd* wnd, Rect region) { return false; }
	virtual bool MsgHandler(Msg msg, void* para) {
		switch (msg) {
			case Msg::WND_SETBG:SetBackground((Figure*)para); break;
			default:return false;
		}
		return true;
	}
public:
	virtual Figure* Clone() const { return new Wnd(); }
	virtual void Paint() {}
	virtual ~Wnd() {
		if (background != nullptr)delete background;
	}
};

} // namespace WndDesign