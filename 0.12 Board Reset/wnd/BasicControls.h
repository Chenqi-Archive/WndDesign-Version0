#pragma once

#include "BasicWnds.h"

namespace WndDesign {

class Button :public FinalWnd {
	// Figure* background;
	Figure* moveOn;
	Figure* clickDown;
public:
	Button(Figure* background = new Color(COLOR_GREY), Figure* moveOn = nullptr, Figure* clickDown = nullptr) :
		FinalWnd(background), moveOn(moveOn), clickDown(clickDown) {
		if (this->moveOn != nullptr) { this->moveOn->SetBoard(board); }
		if (this->clickDown != nullptr) { this->clickDown->SetBoard(board); }
	}
	bool MsgHandler(Msg msg, void* para) override { 
		switch (msg) {
		case WndDesign::Msg::MOUSE_MOVEON:if (moveOn != nullptr) { moveOn->Paint(); }break;
		case WndDesign::Msg::MOUSE_MOVEOFF:background->Paint(); break;
		case WndDesign::Msg::MOUSE_LEFTDOWN:if (clickDown != nullptr) { clickDown->Paint(); }break;
		case WndDesign::Msg::MOUSE_LEFTUP:return handler(this, msg, para);
		default: break;
		}
		return true;
	}
};

class Blinking;
class TextEditor :public FinalWnd {

	Blinking* cursor;


};



class ScrollBar {


};


} // namespace WndDesign