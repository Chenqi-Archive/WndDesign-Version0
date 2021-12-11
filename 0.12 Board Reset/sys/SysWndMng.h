#pragma once

#include "../wnd/Wnd.h"

#include <list>

namespace WndDesign {

typedef void* SysID;

enum class WndState { Show, Hide };

// The system window manager. All wnds are subwindow of SysWnd.
class SysWndMng:public Wnd {
	struct SysWndInfo {
		Wnd* wnd;
		SysID sysid;
		Bitmap* bitmap;
		WndState state;
	};
	std::list<SysWndInfo>sysWnds;   // The front window at the backmost of the list.

	inline const SysWndInfo* GetSysWnd(const Wnd* wnd) {
		for (const auto& it : sysWnds) { if (it.wnd == wnd) { return &it; } }
		return nullptr;
	}
	inline const SysWndInfo* GetSysWnd(SysID sysid) {
		for (const auto& it : sysWnds) { if (it.sysid == sysid) { return &it; } }
		return nullptr;
	}
	inline const SysWndInfo* GetSysWnd(Bitmap* bitmap) {
		for (const auto& it : sysWnds) { if (it.bitmap == bitmap) { return &it; } }
		return nullptr;
	}

private:
	bool AllowToPaint(const Bitmap* bitmap, Rect region) const override;
	void RefreshBoard(const Bitmap* bitmap, Rect region) const override;
private:
	void PaintOn(Board& board) const override {} // Do nothing.
	bool MsgHandler(Msg msg, void* para) override { return false; }; // Do nothing.
public:
	bool AddSubWnd(Wnd* wnd, Rect region, WndState state = WndState::Show);
	bool DestroySubWnd(Wnd* wnd);
	bool ReceiveSysMsg(SysID sysid, Msg msg, void* para);
};



} // n