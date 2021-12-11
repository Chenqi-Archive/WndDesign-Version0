#pragma once

#include "../basic/wndtypes.h"
#include "Wnd.h"

namespace WndDesign {

#define MASTER  0

bool InitWndMng();

bool InitEnd();

WID CreateWnd(WID parent, WndType type, Rect region, Figure&& background, MsgHandler handler = nullptr);

// Register the created window.
// You must register the window before you can send or receive message.
WID RegisterWnd(WID parent, Wnd* wnd, Rect region, MsgHandler handler = nullptr);

//WID CreateWnd(WID parent, HWND hWnd, )

bool DestroyWnd(WID wid);

bool SendMsg(WID wid, Msg msg, void* para);




} // namespace WndDesign