#include "Wnd.h"


// Message dispatcher and windows controller
// May need to create a new thread to handle all messages

// Below defines some basic messages

#define MSG_MOUSE_MOVEON			0x
#define MSG_MOUSE_MOVEOFF			0x
#define MSG_MOUSE_LEFTDOWN			0x
#define MSG_MOUSE_LEFTUP     		0x
#define MSG_MOUSE_RIGHTDOWN			0x

#define MSG_WND_CLOSE
#define MSG_WND_
#define MSG_WND_
#define MSG_WND_
#define MSG_WND_
#define MSG_WND_



// 

extern bool SendSysMsg(Wnd* wnd, uint msg);

bool SendMsg(Wnd* wnd, uint msg)
{
	SendSysMsg(wnd, msg);
	return true;
}

