#include "SysWndMng.h"
#include "WinAPI.h"
#include "../wnd/WndMng.h"
#include "../shape/pattern.h"
#include <list>

namespace WndDesign {

struct SysWndMng {
	WID wid;
	SysID sysid;
	Bitmap* display;
};

std::list<SysWndMng> syswnds;

static inline const SysWndMng* GetSysWnd(WID wid) {
	for (const auto& it : syswnds) { if (it.wid == wid) { return &it; } }
	return nullptr;
}

static inline const SysWndMng* GetSysWnd(SysID sysid) {
	for (const auto& it : syswnds) { if (it.sysid == sysid) { return &it; } }
	return nullptr;
}

static inline const SysWndMng* GetSysWnd(Bitmap* display) {
	for (const auto& it : syswnds) { if (it.display == display) { return &it; } }
	return nullptr;
}

class SysBitmap:public Bitmap {  // SysBitmap has no board.
public:
	SysBitmap(Size size):Bitmap(size) {}
	virtual inline void Refresh(Rect region) {
		SysID sysid = GetSysWnd(this)->sysid;
		SysUpdateWnd(sysid, region);
	}
};

bool CreateSysWnd(WID wid, Wnd* wnd, Rect region) {
	SysID sysid = SysCreateWnd(region);
	if (sysid == NULL) { return false; }
	SysWndMng syswnd = {wid, sysid, new SysBitmap(region.size)};
	wnd->SetBoard(CreateBoard(syswnd.display));
	syswnds.push_back(syswnd);
	return true;
}

bool DestroySysWnd(WID wid) {
	auto it = syswnds.begin();
	for (; it != syswnds.end(); ++it) { if (it->wid == wid) { break; } }
	if (it == syswnds.end()) { return false; }
	SysDestroyWnd(it->sysid);
	delete it->display;
	syswnds.erase(it);
	return true;
}


bool ReceiveSysMsg(SysID sysid, Msg msg, void* para) {
	const SysWndMng* wndmng = GetSysWnd(sysid);
	if (wndmng == nullptr) { return false; }
	WID wid = wndmng->wid;

	if (msg == Msg::WND_PAINT) {
		*(Bitmap**)para = wndmng->display;
		return true;
	}

	return SendMsg(wid, msg, para);
}



} // namespace WndDesign