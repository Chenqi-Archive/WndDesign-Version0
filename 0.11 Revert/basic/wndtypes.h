#pragma once

#include "Figure.h"

namespace WndDesign {

typedef uint WID;
#define INVALID_WID (WID)-1

typedef void* SysID;   // HWND

#define MSG_BEGIN(type)		BEGIN_##type
#define MSG_END(type)		END_##type

#define MSG_TYPE(msg, type)	(Msg::MSG_BEGIN(type) < msg && msg < Msg::MSG_END(type))

enum class Msg {
	// Message		      // para

	MSG_BEGIN(WND),
	WND_SETBG,            // Figure* 
	WND_PAINT,
	MSG_END(WND),

	MSG_BEGIN(MOUSE),	  // Pos*
	MOUSE_MOVEON,		
	MOUSE_MOVEOFF,		
	MOUSE_LEFTDOWN,		
	MOUSE_LEFTUP,		
	MOUSE_RIGHTDOWN,	
	MOUSE_RIGHTUP,		
	MSG_END(MOUSE),


};


typedef bool (*MsgHandler)(WID wndID, Msg msg, void* para);

enum class WndType {
	FINAL,
	GRID,
	SPLIT,
	TILE,
	OVERLAP
};


} // namespace WndDesign