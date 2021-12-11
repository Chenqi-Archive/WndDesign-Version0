// WndMng.h:
//   Manage all Wnds and do message dispatch.

#pragma once
#include "./wndtypes.h"

#define MASTER_WND  0

// Initialize Wnds manager, create the 0th Wnd as the main message handler.
bool WndMngInit();

// All initialized, go to system's message loop.
// May be abandoned when uses multi-thread.
int WndMngInitEnd();

bool SendMsg(WndID wndID, Msg msg, void* para);

WndID GetParent(WndID wndID);