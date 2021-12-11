#pragma once

#include "BasicWnds.h"

namespace WndDesign {

class Button :public FinalWnd {
	Figure* moveOn;
	Figure* clickDown;
public:
	Button(Figure* background = new Color(COLOR_GREY), Figure* moveOn = nullptr, Figure* clickDown = nullptr) :
		FinalWnd(background), moveOn(moveOn), clickDown(clickDown) {
		if (this->moveOn != nullptr) { this->moveOn->SetBoard(Board(this, this->board)); }
		if (this->clickDown != nullptr) { this->clickDown->SetBoard(Board(this, this->board)); }
	}
	bool WndMsgHandler(Msg msg, void* para) override {
		switch (msg) {
			case WndDesign::Msg::MOUSE_MOVEON:if (moveOn != nullptr) { moveOn->Paint(); }break;
			case WndDesign::Msg::MOUSE_MOVEOFF:background->Paint(); break;
			case WndDesign::Msg::MOUSE_LEFTDOWN:if (clickDown != nullptr) { clickDown->Paint(); }break;
			case WndDesign::Msg::MOUSE_LEFTUP:return false;  // Call the user's handler.
			default: break;
		}
		return true;
	}
};


class ScrollBar {
private:
	Wnd* parent;



};



class Blinking;
class TextEditor :public FinalWnd {
	Blinking* cursor;


};


} // namespace WndDesign