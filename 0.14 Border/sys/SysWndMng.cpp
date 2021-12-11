#include "SysWndMng.h"
#include "WinAPI.h"
#include "../shape/Bitmap.h"


bool WndDesign::SysWndMng::AllowToPaint(const Board& board, Rect region) const {
	return true;
}

void WndDesign::SysWndMng::RefreshBoard(const Board& board, Rect region) const {
	const SysWndInfo* syswnd = GetSysWnd(GetBoardBitmap(board));
	if (syswnd == nullptr) { return; }
	SysUpdateWnd(syswnd->sysid, region);
}

bool WndDesign::SysWndMng::AddSubWnd(Wnd* wnd, Rect region) {
	SysID sysid = SysCreateWnd(region);
	if (sysid == NULL) { return false; }
	Bitmap* bitmap = new Bitmap(region.size, COLOR_WHITE, BitmapType::FixedNormal);
	wnd->SetBoard(Board(this, bitmap));
	subWnds.push_back({ wnd, sysid, bitmap });
	return true;
}

bool WndDesign::SysWndMng::DestroySubWnd(Wnd* wnd) {
	auto it = subWnds.begin();
	for (; it != subWnds.end(); ++it) { if (it->wnd == wnd) { break; } }
	if (it == subWnds.end()) { return false; }
	SysDestroyWnd(it->sysid);
	delete it->bitmap;
	subWnds.erase(it);
	return true;
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


