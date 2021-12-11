#include "Wnd.h"
#include "WndMng.h"

Wnd::Wnd(Size size, WndID parent):parent(parent){
	// May add some additional features.
	Color32 color = { 0xff, 0xff, 0xff, 0 };

	// Should use background here and draw it on display.
	display.Init(size.width, size.height, color);
}

bool Wnd::MsgHandler(WndID self, Msg msg, void* para)
{
	switch (msg)
	{
	case MSG_WND_ADDSUBWND: {
		SubWndInfo* subWnd = (SubWndInfo*)para;
		subWnds.push_back(*subWnd);


		// Update display

	}break;
	case MSG_WND_DISPLAY:
		*(void**)para = display.GetBits();
		break;
	case MSG_WND_CHANGECOLOR:
		display.SetColor(*(Color32*)para);
		SendMsg(parent, MSG_WND_PAINT, &self);
		break;
	case MSG_WND_PAINT:
		// A subwindow requests repaint.

		break;
	case MSG_WND_PARENT:
		*(WndID*)para = parent;
		break;
	default:
		break;
	}
	return true;
}
