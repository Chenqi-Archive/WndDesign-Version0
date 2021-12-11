#pragma once

#include "../basic/wndtypes.h"
#include "../wnd/Wnd.h"

namespace WndDesign {

bool CreateSysWnd(WID wid, Wnd* wnd, Rect region);

bool DestroySysWnd(WID wid);

bool ReceiveSysMsg(SysID sysid, Msg msg, void* para);

} // namespace WndDesign