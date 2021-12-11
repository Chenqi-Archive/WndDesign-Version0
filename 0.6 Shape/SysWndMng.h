// SysWndMng.h:
//   Translate between Wnd messages and system messages.
//   Send messages to system by calling functions defined in SysAPI(WinAPI)
//   Receive messages by functions called in SysAPI(WinAPI)


#pragma once
#include "./Wnd.h"

// Send a message to system, such as creating a new window, do painting, 
bool SendSysMsg(WndID wndID, Msg msg, void* para);

int EnterMsgLoop();