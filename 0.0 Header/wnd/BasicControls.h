#pragma once

#include "BasicWnds.h"

namespace WndDesign {

class Button :public FinalWnd {
	Figure* moveon, * clickdown;
	// Textbox* text;

};



class TitleBar :public TileWnd {
	Bitmap* icon;
	TextBox* title;
	Button* minimize, * maximize, * close;

};

} // namespace WndDesign