#include "SysWndMng.h"
#include "WinAPI.h"
#include "../shape/Bitmap.h"


bool WndDesign::SysWndMng::AllowToPaint(const Bitmap* bitmap, Rect region) const {
	return true;
}

void WndDesign::SysWndMng::RefreshBoard(const Bitmap* bitmap, Rect region) const {

}

bool WndDesign::SysWndMng::ReceiveSysMsg(SysID sysid, Msg msg, void* para) {
	const SysWndInfo* syswnd = GetSysWnd(sysid);
	if (syswnd == nullptr) { return false; }

	if (msg == Msg::WND_PAINT) {
		*(Bitmap**)para = syswnd->bitmap;
		return true;
	}
	return syswnd->wnd->MsgHandler(msg, para);
}

bool WndDesign::SysWndMng::AddSubWnd(Wnd* wnd, Rect region, WndState state) {
	SysID sysid = SysCreateWnd(region);
	if (sysid == NULL) { return false; }
	SysWndInfo syswnd = {wnd, sysid, new Bitmap(region.size), state};
	wnd->SetBoard(Board(this, syswnd.bitmap));
	sysWnds.push_back(syswnd);
	return true;
}

bool WndDesign::SysWndMng::DestroySubWnd(Wnd* wnd) {
	auto it = sysWnds.begin();
	for (; it != sysWnds.end(); ++it) { if (it->wnd == wnd) { break; } }
	if (it == sysWnds.end()) { return false; }
	SysDestroyWnd(it->sysid);
	delete it->bitmap;
	sysWnds.erase(it);
	return true;
}
