#include "BasicAnimations.h"
#include "BasicShapes.h"
#include "Bitmap.h"
#include "Paint.h"

#define _USE_MATH_DEFINES
#include <cmath>

void WndDesign::Blinking::PaintOn(Board& board) const {
	Color color(color1);
	if (state) { color.Set(color2); }
	color.PaintOn(board);
}

void WndDesign::SimpleClock::PaintOn(Board& board) const {
	Pen pen(COLOR_WHITE, 10);

	Size size = board.Size();
	uint diameter = min(size.height, size.width);
	if (diameter > pen.Thickness()) { diameter -= pen.Thickness(); } else { diameter = 0; }
	Pos center = {size.width / 2,size.height / 2};

	// Clear previous state.
	double t = ((int)angle - 6) / 180.0 * M_PI;
	Pos pos = center + Pos{diameter / 2 * cos(t) ,diameter / 2 * sin(t)};
	pen.Paint(board, pos);

	// Draw next state.
	pen.SetColor(COLOR_RED);
	t = angle / 180.0 * M_PI;
	pos = center + Pos{diameter / 2 * cos(t) ,diameter / 2 * sin(t)};
	pen.Paint(board, pos);
}
