#pragma once

#include "../basic/basictypes.h"

namespace WndDesign {

struct MouseMsg {
	Pos pos;
	uint keys;  // virtual keys down, consistent with Windows.
	inline bool left() const { return keys & 0x0001; }
	inline bool right()	const { return keys & 0x0002; }
	inline bool shift()	const { return keys & 0x0004; }
	inline bool ctrl() const { return keys & 0x0008; }
	inline bool middle() const { return keys & 0x0010; }
};


#define MSG_BEGIN(type)		BEGIN_##type
#define MSG_END(type)		END_##type
#define MSG_TYPE(msg, type)	(Msg::MSG_BEGIN(type) < msg && msg < Msg::MSG_END(type))

enum class Msg {
	//Messages			// parameter		// Note

	MSG_BEGIN(WND),
	WND_CREATE, 		// NULL.   			// Sent when the window is added as a subwindow. For creating sub objects.
	WND_DESTROY,
	WND_PAINT,
	MSG_END(WND),

	MSG_BEGIN(MOUSE),	// MouseMsg*
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


DECLARE_CLASS(Wnd)
typedef bool (*UserMsgHandler)(Wnd* wnd, Msg msg, void* para);

bool SendMsg(Wnd* wnd, Msg msg, void* para);

} // namespace WndDesign