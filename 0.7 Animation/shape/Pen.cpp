#include "./BasicShapes.h"
#include "Pen.h"

void Pen::PaintPoint(Bitmap& bitmap, Pos center) {
	int radius = thickness;
	while (radius--) {
		double increment = atan(1.0 / radius);
		for (double t = 0; t < 2 * M_PI; t += increment)
		{
			if (style == Spray && rand() % 10 != 0) {
				continue;
			}
			int x = radius * cos(t) + center.x;
			int y = radius * sin(t) + center.y;
			bitmap.Set(x, y, color);
		}
	}
}

Shape::~Shape() {}