#pragma once

#include "../Figure.h"
#include "paint.h"

namespace WndDesign {

class Shape:public Figure {
protected:
	Pen pen = Pen(COLOR_BLACK, 1);
	Brush brush = Brush(COLOR_WHITE, BrushStyle::None);
public:
	inline void SetPenBrush(const Pen& pen, const Brush& brush = Brush(COLOR_WHITE, BrushStyle::None)) { 
		this->pen = pen; this->brush = brush; 
	}
};

} // namespace WndDesign