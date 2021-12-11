#include "Wnd.h"
#include <cstdlib>

void Wnd::CreateSubWnd(Pos pos = { 0,0 }, Size size = { 0,0 }, Style style = WND_SHOW) {
	SubWndInfo subWnd;
	subWnd.wnd = new Wnd(size.width, size.height, this);
	subWnd.pos = pos;
	subWnd.style = style;
	AddSubWnd(subWnd);
}

void Wnd::AddSubWnd(const SubWndInfo& subWnd){
	if (subWnds.size() == 0) {
		// The first subwindow, allocate memory for subWndIndex
		subWndIndex.Init(display.width, display.height);
	}

	PaintSubWnd(subWnds.push_back(subWnd));
}

void Wnd::PaintSubWnd(const SubWndInfo& subWnd){
	if (subWnd.style != WND_SHOW) {
		return;
	}

	Wnd* wnd = subWnd.wnd;
	Pos pos = subWnd.pos;		
	Color32 color;
	for (uint j = 0; j < wnd->display.height; ++j) {
		for (uint i = 0; i < wnd->display.width; ++i) {
			wnd->display.Get(i, j, color);
			display.Set(pos.x + i, pos.y + j, color);
			subWndIndex.Set(pos.x + i, pos.y + j, &(SubWndInfo &)subWnd);
		}
	}
}

Wnd::Wnd(uint width, uint height, const Wnd* parentWnd) :parentWnd(parentWnd){
	Color32 color = { rand() % 0xff, rand() % 0xff, rand() % 0xff, 0 };
	display.Init(width, height, color);
}

void* Wnd::GetBitmap() {
	return display.GetBits();
}

Wnd::~Wnd() {
	for (auto it = subWnds.begin(); it; ++it) {
		delete it.Item().wnd;
	}
}


extern bool SendMsg(Wnd* wnd, uint msg);

void Wnd::OnClick(uint x, uint y) {
	SubWndInfo* sub;
	subWndIndex.Get(x, y, sub);
	if ( sub == nullptr) {
		//CreateSubWnd({ x,y }, { 50,50 });
		return;
	}
	else {
		if (sub->pos.x == 765&&sub->pos.y==5) {
			// close button
			SendMsg(this, 0);
		}
		sub->wnd->OnRedraw();
		PaintSubWnd(*sub);
	}
}

void Wnd::OnRedraw(){
	Color32 color = { rand() % 0xff, rand() % 0xff, rand() % 0xff, 0 };
	for (uint j = 0; j < display.height; ++j) {
		for (uint i = 0; i < display.width; ++i) {
			display.Set(i, j, color);
		}
	}
}
