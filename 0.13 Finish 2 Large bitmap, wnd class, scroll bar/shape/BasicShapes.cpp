#include "BasicShapes.h"

#define _USE_MATH_DEFINES
#include <cmath>

void WndDesign::Circle::PaintOn(Board& board)const {
	Size size = board.Size();
	uint diameter = min(size.height, size.width);
	if (diameter > pen.Thickness()) { diameter -= pen.Thickness(); } else { diameter = 0; }
	Pos center = {size.width / 2,size.height / 2};
	if (pen.Style() != PenStyle::None) {
		double increment = atan(2.0 / diameter);
		if (pen.Thickness() == 1) { // No need to call pen
			Color32 color = pen.Color();
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
	if (brush.Style() != BrushStyle::None) {
		brush.Paint(board, center, diameter / 2);
	}
}
