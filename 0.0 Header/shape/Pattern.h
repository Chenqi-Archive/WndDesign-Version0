#pragma once

#include "../basic/Figure.h"

namespace WndDesign {

class Color :public Figure {
public:
	Color32 color;
	Color(Color32 color):color(color){}
	inline void PaintOn(Board& board)const override {
		board.Clear(color);
	}
};


} // namespace WndDesign