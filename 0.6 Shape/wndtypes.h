
#pragma once

#include "./types.h"
#include "./shape/Bitmap.h"

typedef uint WndID;
#define INVALID_WND_ID (uint)-1


struct SubWndInfo {
	WndID wndID;
	Pos pos;  // Relative to left top corner, could be negetive.
	Size size;  // Size is both saved by parent and sub windows.
	const Bitmap& display;  // Pointer to subWnd's display bitmap.
};

// May combine these window structures later.
typedef struct {
	WndID subWnd;
	Pos vector;
}MoveWndInfo;
typedef struct {
	WndID subWnd;
	Size newSize;
}ResizeWndInfo;

// Messages definatins below:
// 
// Some messages are posted and returns directly (handled by the msg manager thread),
//   like mouse messages or keyboard messages. 
// Other messages must be handled before return, like wnd messages.
// Messages pass address as parameter must be handled(or parameters be saved) before return.
//
// There are two kinds of message handlers, one defined within the Wnd class, only for
//   WND messages that must be handled by the member functions. The other is declared at
//   at Handlers.h and defined at Handlers.cpp, which is used for other operations that 
//   are not window related, but can do window jobs by sending WND messages.
//
// Messages headers and tails in MSG_BEGIN_XXX or MSG_END_XXX are 
//   invalid messages used to compare, for example:
//   
//   if(MSG_TYPE(msg, WND)){
//       // Do something about wnd processes.
//   }
//  

#define MSG_BEGIN(type)		MSG_BEGIN_##type
#define MSG_END(type)		MSG_END_##type

#define MSG_TYPE(msg, type)	(MSG_BEGIN(type) < msg && msg < MSG_END(type))

enum Msg :uint {

	// Messages             Parameter structure (pass an address) -> Return value

	// Wnd messages: 
	//   Handled directly at Wnd manager by calling internal functions, 
	//   not passed to WndMsgHandler.
	MSG_BEGIN(WND),
	MSG_WND_CREATE,			// WndInfo* -> WndInfo*(WndID as parent) (Must be sent to MASTER_WND)		
	MSG_WND_DESTROY,		// WndID* (Sent to parent window)
	MSG_WND_ADDSUBWND,      // SubWndInfo* (Adds a created window)
	MSG_WND_REMOVESUBWND,   // WndID*
	MSG_WND_MOVESUBWND,		// MoveWndInfo*
	MSG_WND_CHANGEBG,       // Shape*

	MSG_WND_PAINT,          // WndID* (Inform parent wnd to repaint itself) (Could add an invalid rect para)

	MSG_END(WND),

	// Mouse messages:
	MSG_BEGIN(MOUSE),
	MSG_MOUSE_MOVEON,		// Pos* (May later add keyboard commands)
	MSG_MOUSE_MOVEOFF,		// Pos*
	MSG_MOUSE_LEFTDOWN,		// Pos*
	MSG_MOUSE_LEFTUP,		// Pos*
	MSG_MOUSE_RIGHTDOWN,	// Pos*
	MSG_MOUSE_RIGHTUP,		// Pos*
	MSG_END(MOUSE),


	// User messages:
	MSG_BEGIN(USER),

	MSG_END(USER)
};

// Wnd message handler, some messages will not be passed to the handler.
// WndID is used for the handler to do something directly related to Wnd,
//   like sending more messages for request or do some operations.
typedef bool (*WndMsgHandler)(WndID wndID, Msg msg, void* para);

typedef struct {
	WndID parent;
	Pos pos;               // position relative to parent window, only for parent window.
	Size size;
	Shape& shape;    // Might use pointer instead.
	WndMsgHandler handler; // Message handler, must be predefined.

	// More info to be defined, such as background, styles and system styles.


	// And also user-defined derived Wnd classes should be able to created.


}WndInfo;