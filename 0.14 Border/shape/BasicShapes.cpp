#include "BasicShapes.h"

#define _USE_MATH_DEFINES
#include <cmath>

void WndDesign::Circle::PaintOn(Board& board)const {
	Size size = board.Size();
	uint diameter = min(size.height, size.width);
	if (diameter > pen.Thickness()) { diameter -= pen.Thickness(); } else { diameter = 0; }
	Pos center = { size.width / 2,size.height / 2 };

	float increment = atan((float)2.0 / diameter);
	for (float t = 0; t < 2 * M_PI; t += increment) {
		Pos pos = center + Pos{ diameter / 2 * cos(t) ,diameter / 2 * sin(t) };
		pen.Paint(board, pos);
	}

	brush.Paint(board, center, diameter / 2);
}

void WndDesign::Line::PaintOn(Board& board) const {
	Pos vector = end - begin;
	if (vector == Pos{ 0, 0 }) {  // Just a point.
		pen.Paint(board, begin);
		return;
	}
	uint length = max(abs(vector.x), abs(vector.y));
	for (uint t = 0; t <= length; ++t) {
		Pos center = begin + Pos{ vector.x * t / length, vector.y * t / length };
		pen.Paint(board, center);
	}
	
	// No brush.
}

void WndDesign::Rectangle::PaintOn(Board& board) const {
	const Size& size = board.Size();
	uint thick = pen.Thickness();
	Color32 color = pen.Color();
	for (uint s = 0; s < thick; ++s) {
		for (uint t = 0; t < size.width; ++t) {
			board.Set({ t,s }, color);
		}
	}
	for (uint s = size.height - thick; s < size.height; ++s) {
		for (uint t = 0; t < size.width; ++t) {
			board.Set({ t,s }, color);
		}
	}
	for (uint t = thick; t < size.height - thick; ++t) {
		for (uint s = 0; s < thick; ++s) {
			board.Set({ s,t }, color);
		}
		for (uint s = size.width - thick; s < size.width; ++s) {
			board.Set({ s,t }, color);
		}
	}

	brush.Paint(board, { size.width / 2 , size.height / 2 });
}
