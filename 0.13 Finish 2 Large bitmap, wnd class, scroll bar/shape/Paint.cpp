#include "Paint.h"
#include "../Board.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include <queue>  // may use lib instead

namespace WndDesign {

void WndDesign::Pen::Paint(Board& board, Pos center) const {
	if (style == PenStyle::None) { return; }
	uint diameter = thickness;
	Rect region = {center - Pos{diameter / 2, diameter / 2}, {diameter, diameter}};
	while (diameter--) {
		double increment = atan(2.0 / diameter);
		for (double t = 0; t < 2 * M_PI; t += increment) {
			if (style == PenStyle::Spray && rand() % 10 != 0) {
				continue;
			}
			Pos pos = center + Pos{diameter / 2 * cos(t) ,diameter / 2 * sin(t)};
			board.Set(pos, color);
		}
		if (0 == diameter--) { break; }
	}
}


static inline uint Distance(const Pos& pos1, const Pos& pos2) {
	Pos dis = pos1 - pos2;
	return (uint)sqrt((float)(dis.x * dis.x + dis.y * dis.y));
}

void WndDesign::Brush::Paint(Board& board, Pos center, uint maxSpanPixel) const {
	if (style == BrushStyle::None) { return; }

	Rect region = {{},board.Size()};
	if (!PosInRect(center, region)) { return; }

	const Pos direction[4] = {{1,0},{0,1},{-1,0},{0,-1}};
	Color32 startColor = board.Get(center);

	std::queue<Pos> q;
	q.push(center);
	while (!q.empty()) {
		Pos pos = q.front(); q.pop();
		Color32 color = board.Get(pos);
		if (this->color == color || Distance(pos, center) > maxSpanPixel) {
			continue;
		}
		board.Set(pos, color);
		for (int i = 0; i < 4; ++i) {
			Pos next = pos + direction[i];
			if (PosInRect(next, region) && board.Get(next) == startColor) {
				q.push(next);
			}
		}
	}
}


} // namespace WndDesign