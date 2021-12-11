#pragma once

#include "Wnd.h"
#include "../shape/pattern.h"
#include <list>

namespace WndDesign {

class FinalWnd:public Wnd {
public:
	inline virtual bool MsgHandler(Msg msg, void* para) {
		switch (msg) {
			case Msg::WND_SETBG:SetBG(*(Figure*)para); break;
			default:return false;
		}
		return true;
	}
};


class SplitWnd:public Wnd {


public:
	inline virtual bool MsgHandler(Msg msg, void* para) {
		switch (msg) {
			case Msg::WND_SETBG:SetBG(*(Figure*)para); break;
			default:return false;
		}
		return true;
	}
};


class OverlapWnd:public Wnd {
private:
	struct SubWndMng {
		Wnd* wnd;
		Rect region;
		Bitmap* display;  // Every subwindow of OverlapWnd has an independent board.
	
		~SubWndMng() {

		}
	};

	std::list<SubWndMng> subwnds;

private:
	virtual void AddSubWnd(Wnd* wnd, Rect region) {
		Bitmap* display = new Bitmap(region.size);
		display->SetBoard(CreateBoard(board, region));
		SubWndMng subwnd = {wnd, region, display};
		subwnd.wnd->SetBoard(CreateBoard(display));
		subwnds.push_back(subwnd);
	}

public:
	inline virtual bool MsgHandler(Msg msg, void* para) {
		switch (msg) {
			case Msg::WND_SETBG:SetBG(*(Figure*)para); break;
			default:return false;
		}
		return true;
	}
};



} // namespace WndDesign