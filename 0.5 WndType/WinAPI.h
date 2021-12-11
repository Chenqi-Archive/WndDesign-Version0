#pragma once

#include "./wndtypes.h"
#include <Windows.h>

// Receive a message from system callback function.
bool ReceiveSysMsg(HWND hWnd, Msg msg, void* para);

HWND SysCreateWnd(Pos pos, Size size);

// Only used after creating a window.
void SysShowWnd(HWND hWnd);

void SysPaintWnd(HWND hWnd, const Bitmap& bitmap);

void SysMoveWnd(HWND hWnd, Pos vector);

int SysMsgLoop();
