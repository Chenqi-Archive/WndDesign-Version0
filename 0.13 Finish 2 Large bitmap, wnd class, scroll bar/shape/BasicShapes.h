#pragma once

#include "Shape.h"

namespace WndDesign {

class Circle:public Shape {
private:
	void PaintOn(Board& board)const override;
};

class Font;

class Character :public Shape {
private:
	wchar_t ch;
	Font* font;
private:
	void PaintOn(Board& board)const override;
};

class TextShow :public Shape {
private:
	wchar_t* text;
	uint size;
private:
	void PaintOn(Board& board)const override;
public:
	TextShow(const wchar_t* text, uint size) :size(size){
		this->text = new wchar_t[size];
		for (uint i = 0; i < size; ++i) {
			this->text[i] = text[i];
		}
	}
};

} // namespace WndDesign