#pragma once

#include "./Shape.h"
#include "./Font.h"

class Character :public Shape {
	wchar_t ch;
	Font font;  // May use a reference type.
public:
	Character(wchar_t ch, const Font& font):ch(ch), font(font){}
	void SetCh(wchar_t ch) { this->ch = ch; }
	void SetFont(const Font& font) { this->font = font; }
	virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) const {

	}
};