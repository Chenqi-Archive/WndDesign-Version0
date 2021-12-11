#pragma once

#include "../types.h"

namespace WndDesign {

typedef uint WID;
#define INVALID_WID (WID)-1


struct MouseMsg {
	Pos pos;
	uint keys;  // virtual keys down, consistent with Windows.
	inline bool LEFT() const { return keys & 0x0001; }
	inline bool RIGHT()	const { return keys & 0x0002; }
	inline bool SHIFT()	const { return keys & 0x0004; }
	inline bool CTRL() const { return keys & 0x0008; }
	inline bool MIDDLE() const { return keys & 0x0010; }
};


#define MSG_BEGIN(type)		BEGIN_##type
#define MSG_END(type)		END_##type
#define MSG_TYPE(msg, type)	(Msg::MSG_BEGIN(type) < msg && msg < Msg::MSG_END(type))

enum class Msg {
	// Messages		        // parameter

	MSG_BEGIN(WND),
	WND_PAINT,
	MSG_END(WND),

	MSG_BEGIN(MOUSE),	  		// MouseMsg*
	MOUSE_MOVEON,
	MOUSE_MOVEOFF,
	MOUSE_LEFTDOWN,
	MOUSE_LEFTUP,
	MOUSE_RIGHTDOWN,
	MOUSE_RIGHTUP,
	MSG_END(MOUSE),

	MSG_BEGIN(KEY),
	KEY_DOWN,
	KEY_UP,
	MSG_END(KEY),
};


typedef bool (*MsgHandler)(Wnd* wnd, Msg msg, void* para);


} // namespace WndDesign