#pragma once

#include "../wnd/Wnd.h"

#include <list>

namespace WndDesign {

typedef void* SysID;

/** SysWndMng: the system window manager. 
	  All wnds are subwindow of SysWnd.
*/
class SysWndMng :public Wnd {

/////////////// For subwindows ///////////////
private:
	struct SysWndInfo {
		Wnd* wnd;
		SysID sysid;
		Bitmap* bitmap;
	};
	std::list<SysWndInfo>subWnds;   // The front window at the backmost of the list.
private:
	inline const SysWndInfo* GetSysWnd(const Wnd* wnd) const {
		for (const auto& it : subWnds) { if (it.wnd == wnd) { return &it; } }
		return nullptr;
	}
	inline const SysWndInfo* GetSysWnd(SysID sysid) const {
		for (const auto& it : subWnds) { if (it.sysid == sysid) { return &it; } }
		return nullptr;
	}
	inline const SysWndInfo* GetSysWnd(const Bitmap* bitmap) const {
		for (const auto& it : subWnds) { if (it.bitmap == bitmap) { return &it; } }
		return nullptr;
	}
public:
	bool AddSubWnd(Wnd* wnd, Rect region);
	bool DestroySubWnd(Wnd* wnd);
	inline uint SubWndCnt() { return subWnds.size(); }

/////////////// For sub board managing ///////////////
private:
	bool AllowToPaint(const Board& board, Rect region = {}) const override;
	void RefreshBoard(const Board& board, Rect region = {}) const override;

/////////////// For handling system messages ///////////////
public:
	bool ReceiveSysMsg(SysID sysid, Msg msg, void* para);

/////////////// Constructor and Destructor ///////////////
public:
	~SysWndMng() {
		for (auto& item : subWnds) { delete item.wnd; delete item.bitmap; }
	}
};



} // n