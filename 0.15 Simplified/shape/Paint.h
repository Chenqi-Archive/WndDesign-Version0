#pragma once

#include "../basic/basictypes.h"

namespace WndDesign {

DECLARE_CLASS(Board)
class Pen {
public:
	Color32 color;
	uint thickness;
	Pen(Color32 color = COLOR_BLACK, uint thickness = 1)
		:color(color),thickness(thickness){}
	void Paint(Board& board, Pos center) const;
};

class Brush {
public:
	Color32 color;
	uint radius;
	Brush(Color32 color = COLOR_WHITE, uint radius = 0)
		:color(color), radius(radius) {}
	void Paint(Board& board, Pos center) const;
};

} // namespace WndDesign