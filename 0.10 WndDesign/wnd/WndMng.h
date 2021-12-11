#pragma once

#include "../basic/wndtypes.h"

namespace WndDesign {

#define MASTER_WID  0

bool InitWndMng();

WID CreateWnd(WID parent, WndType type, Rect region, Figure& background, WndMsgHandler handler);

bool SendMsg(WID wid, Msg msg, void* para);




} // namespace WndDesign