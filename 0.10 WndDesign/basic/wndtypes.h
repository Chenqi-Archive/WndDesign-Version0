#pragma once

#include "Figure.h"

namespace WndDesign {

typedef uint WID;
#define INVALID_WID (WID)-1

typedef void* SysID;   // HWND

enum class Msg {
	// Message		  // para

	WND_SETBG,        // Figure* 
	WND_PAINT,

	MOUSE_MOVEON,		
	MOUSE_MOVEOFF,		
	MOUSE_LEFTDOWN,		
	MOUSE_LEFTUP,		
	MOUSE_RIGHTDOWN,	
	MOUSE_RIGHTUP,		

};


typedef bool (*WndMsgHandler)(WID wndID, Msg msg, void* para);

enum class WndType {
	FINAL,
	GRID,
	SPLIT,
	OVERLAP
};


} // namespace WndDesign