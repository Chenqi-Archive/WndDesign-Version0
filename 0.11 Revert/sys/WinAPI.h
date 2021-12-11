#pragma once
#include "../basic/wndtypes.h"

using namespace WndDesign;

SysID SysCreateWnd(Rect region);

void SysDestroyWnd(SysID sysid);

void SysUpdateWnd(SysID sysid, Rect region);

//void SysMoveWnd(SysID sysid, Pos vector);
