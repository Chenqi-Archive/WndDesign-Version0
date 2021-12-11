#pragma once

#include "Shape.h"

namespace WndDesign {

class Rectangle :public Shape {
public:
	void PaintOn(Board& board)const override;
};



DECLARE_CLASS(Font)
class TextBox :public Shape {
private:
	const wchar_t* text;
	uint len;

	Font* font;
	uint fontSize;
};

} // namespace WndDesign