#pragma once

#include "Wnd.h"
#include "../shape/Bitmap.h"
#include "../sys/SysWnd.h"
#include <list>

namespace WndDesign {

class SysWndMng :public Wnd {
private:
	struct SysWndInfo {
		Wnd* wnd;
		WID wid;
		Bitmap* bitmap;
	};
	std::list<SysWndInfo>wnds;   // The front window at the backmost of the list.
private:
	inline const SysWndInfo* GetSysWnd(const Wnd* wnd) const {
		for (const auto& it : wnds) { if (it.wnd == wnd) { return &it; } }
		return nullptr;
	}
	inline const SysWndInfo* GetSysWnd(WID wid) const {
		for (const auto& it : wnds) { if (it.wid == wid) { return &it; } }
		return nullptr;
	}
	inline const SysWndInfo* GetSysWnd(const Bitmap* bitmap) const {
		for (const auto& it : wnds) { if (it.bitmap == bitmap) { return &it; } }
		return nullptr;
	}
private:
	void FlushBitmap(const WndBitmap* bitmap, Rect region) override;

public:
	bool AddWnd(Wnd* wnd, Rect region);
	bool DestroyWnd(Wnd* wnd);
};


} // namespace WndDesign