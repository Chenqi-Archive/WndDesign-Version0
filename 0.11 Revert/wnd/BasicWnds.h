#pragma once

#include "Wnd.h"
#include "../shape/pattern.h"
#include <list>

namespace WndDesign {

class FinalWnd:public Wnd {
public:
	inline virtual bool MsgHandler(Msg msg, void* para) {
		switch (msg) {
			case Msg::WND_SETBG:SetBackground((Figure*)para); break;
			default:return false;
		}
		return true;
	}
};

class SplitWnd:public Wnd {


public:
	inline virtual bool MsgHandler(Msg msg, void* para) {
		switch (msg) {
			case Msg::WND_SETBG:break;  // You can't set backgound again! Or the subwindows will be erased.
			default:return false;
		}
		return true;
	}
};


// The background of TileWnd must be static!

class TileWnd:public Wnd {
	struct SubWnd {
		WID wid;
		Rect region;

		~SubWnd() {

		}
	};
	

private:
	std::list<SubWnd> subwnds;  // Used to check overlapping.
	
	inline bool IsOverlapping(Rect region) {
		for (const auto& it : subwnds) {
			if (it.region.Intersect(region).size == Size{0,0}) { continue; }
			return true;
		}
		return false;
	}

	inline const SubWnd* GetSubWnd(Pos pos) {
		for (const auto& it : subwnds) { if (PosInRect(pos, it.region)) { return &it; } }
		return nullptr;
	}

public:
	virtual bool AddSubWnd(WID wid, Wnd* wnd, Rect region) {
		if (IsOverlapping(region)) { return false; }
		wnd->SetBoard(CreateBoard(board, region));
		subwnds.push_back({wid, region});
		return true;
	}

	inline virtual bool MsgHandler(Msg msg, void* para) {
		if (MSG_TYPE(msg, MOUSE)) {
			static const SubWnd* lastsubwnd = nullptr;
			static Pos lastpos = {};

			Pos pos;
			const SubWnd* subwnd = GetSubWnd(*(Pos*)para);
			if (subwnd != lastsubwnd && lastsubwnd!=nullptr) {
				pos = lastpos - lastsubwnd->region.pos;
				SendMsg(lastsubwnd->wid, Msg::MOUSE_MOVEOFF, &pos);
			}
			lastsubwnd = subwnd; lastpos = *(Pos*)para;
			if (subwnd == nullptr) { return false; }
			pos = *(Pos*)para - subwnd->region.pos;
			SendMsg(subwnd->wid, msg, &pos);
			return true;
		}
		switch (msg) {
			case Msg::WND_SETBG:break;  // You can't set backgound again! Or the subwindows will be erased.
			default:return false;
		}
		return true;
	}
};

class OverlapWnd:public Wnd {
private:
	struct SubWnd {
		WID wid;
		Rect region;
		Bitmap* display;  // Every subwindow of OverlapWnd has an independent board.
	
		~SubWnd() {

		}
	};

	std::list<SubWnd> subwnds;

private:
	virtual bool AddSubWnd(WID wid, Wnd* wnd, Rect region) {
		Bitmap* display = new Bitmap(region.size);
		display->SetBoard(CreateBoard(board, region));
		wnd ->SetBoard(CreateBoard(display));
		SubWnd subwnd = {wid, region, display};
		subwnds.push_back(subwnd);
		return true;
	}

public:
	inline virtual bool MsgHandler(Msg msg, void* para) {
		switch (msg) {
			case Msg::WND_SETBG:SetBackground((Figure*)para); break;
			default:return false;
		}
		return true;
	}

};



} // namespace WndDesign