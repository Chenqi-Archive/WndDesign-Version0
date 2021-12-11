#pragma once

#include "BasicWnds.h"

namespace WndDesign {

class Button :public FinalWnd {
	Figure* moveon, * clickdown;
	// Textbox* text;

public:
	Button(const wchar_t* text,
		   UserMsgHandler handler,
		   Figure* background = new Color(0xFFCCCCCC),
		   Figure* moveOn = nullptr,
		   Figure* clickDown = nullptr,
		   Pen border = Pen(COLOR_BLACK, 1)) :
		FinalWnd(background, border, false, handler) {

	}
};



class TitleBar :public TileWnd {
	Bitmap* icon;
	TextBox* title;
	Button* minimize, * maximize, * close;

};

} // namespace WndDesign