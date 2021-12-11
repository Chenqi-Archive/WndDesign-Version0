#pragma once

#include "./WndMng.h"

WndID CreateWnd(WndID parent, Pos pos,Size size, WndMsgHandler handler);


// May also support read a Wnd defination file and create multiple windows on init.

