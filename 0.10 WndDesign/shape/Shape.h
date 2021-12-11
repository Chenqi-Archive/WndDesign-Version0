#pragma once

#include "../basic/Figure.h"
#include "paint.h"

namespace WndDesign {

class Shape:public Figure {
protected:
	Pen pen = Pen(COLOR_BLACK, 1);
	Brush brush = Brush(COLOR_WHITE, BrushStyle::None);
public:
	void SetPenBrush(const Pen& pen, const Brush& brush) {
		this->pen = pen; this->brush = brush;
	}
	//virtual Figure* Clone() const = 0;
	//virtual void Paint() = 0;
	//virtual ~Shape() {}
};

} // namespace WndDesign