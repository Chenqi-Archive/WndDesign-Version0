#include "paint.h"
#define _USE_MATH_DEFINES
#include <cmath>

#include <queue>  // may use lib instead

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

void WndDesign::Brush::Paint(Board& board, Pos center) const {
	if (style == BrushStyle::None) { return; }
	Rect region = {{},board.GetSize()};
	const Pos direction[4] = {{1,0},{0,1},{-1,0},{0,-1}};
	struct posInfo { Pos pos; uint span; };
	std::queue<posInfo> q;
	if (PosInRect(center, region)) {
		q.push({center, 1});
	}
	while (!q.empty()) {
		posInfo curPos = q.front(); q.pop();
		if (curPos.span>maxSpan) {
			continue;  // could return here.
		}
		board.Set(curPos.pos, color);
		for (int i = 0; i < 4; ++i) {
			Pos nextPos = curPos.pos + direction[i];
			if (PosInRect(nextPos, region)) {
				q.push({nextPos, curPos.span + 1});
			}
		}
	}
}
