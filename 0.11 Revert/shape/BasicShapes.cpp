#include "BasicShapes.h"

#define _USE_MATH_DEFINES
#include <cmath>

inline void WndDesign::Circle::Paint() {
	Size size = board.GetSize();
	uint diameter = min(size.height, size.width);
	if (diameter > pen.GetThickness()) { diameter -= pen.GetThickness(); } else { diameter = 0; }
	Pos center = {size.width / 2,size.height / 2};
	if (pen.GetStyle() != PenStyle::None) {
		double increment = atan(2.0 / diameter);
		if (pen.GetThickness() == 1) { // No need to call pen
			Color32 color = pen.GetColor();
			for (double t = 0; t < 2 * M_PI; t += increment) {
				Pos pos = center + Pos{diameter / 2 * cos(t) ,diameter / 2 * sin(t)};
				board.Set(pos, color);
			}
		} else {
			for (double t = 0; t < 2 * M_PI; t += increment) {
				Pos pos = center + Pos{diameter / 2 * cos(t) ,diameter / 2 * sin(t)};
				pen.Paint(board, pos);
			}
		}
	}
	if (brush.GetStyle() != BrushStyle::None) {
		brush.Paint(board, center);
	}
	board.Flush();
}
