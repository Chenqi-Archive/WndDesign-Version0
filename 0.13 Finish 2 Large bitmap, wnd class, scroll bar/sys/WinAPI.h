#pragma once

using namespace WndDesign;

SysID SysCreateWnd(Rect region);

void SysDestroyWnd(SysID sysid);

void SysUpdateWnd(SysID sysid, Rect region);
