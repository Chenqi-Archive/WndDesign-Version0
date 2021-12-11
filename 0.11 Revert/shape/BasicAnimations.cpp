#include "BasicAnimations.h"
#include "paint.h"

#define _USE_MATH_DEFINES
#include <cmath>

void WndDesign::Clock::Paint() {
	Size size = board.GetSize();

	Pen pen(COLOR_WHITE, 10);

	uint diameter = min(size.height, size.width);
	if (diameter > pen.GetThickness()) { diameter -= pen.GetThickness(); }
	else { diameter = 0; }

	Pos center = {size.width / 2,size.height / 2};

	// Clear previous state.
	double t = angle / 180.0 * M_PI;
	Pos pos = center + Pos{diameter / 2 * cos(t) ,diameter / 2 * sin(t)};
	pen.Paint(board, pos);

	angle = (angle + 6) % 360;
	t = angle / 180.0 * M_PI;

	// Draw next state.
	pos = center + Pos{diameter / 2 * cos(t) ,diameter / 2 * sin(t)};
	pen.SetColor(COLOR_RED);
	pen.Paint(board, pos);

	board.Flush();
}
