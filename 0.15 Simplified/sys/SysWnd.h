#pragma once

#include "../basic/basictypes.h"

namespace WndDesign {

typedef void* WID;

WID SysCreateWnd(Rect region);

bool SysDestroyWnd(WID wid);

bool SysUpdateWnd(WID wid, Rect region);


} // namespace WndDesign