#pragma once

#include "../basic/wndtypes.h"

namespace WndDesign {

class Wnd :public Figure{
protected:
	Figure* background = nullptr;
public:
	void SetBG(const Figure& figure) {
		if (background != nullptr)delete background;
		background = figure.Clone();
		background->SetBoard(board);
		background->Paint();
	}
	virtual void AddSubWnd(Wnd* wnd, Rect region) {}
	virtual bool MsgHandler(Msg msg, void* para) {
		switch (msg) {
			case Msg::WND_SETBG:SetBG(*(Figure*)para); break;
			default:return false;
		}
		return true;
	}
public:
	virtual Figure* Clone() const { return new Wnd(); }
	virtual void Paint() {}
	virtual ~Wnd() {}
};

} // namespace WndDesign