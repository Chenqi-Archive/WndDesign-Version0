#pragma once

#include "wnd/BasicWnds.h"
#include "wnd/SysWndMng.h"
#include "wnd/BasicControls.h"
#include "shape/BasicShapes.h"
#include "shape/BasicAnimations.h"
#include "sys/Cursor.h"


namespace WndDesign {

extern SysWndMng* master;

inline bool CreateWnd(Wnd* wnd, Rect region) {
	return master->AddWnd(wnd, region);
}



} // namespace WndDesign