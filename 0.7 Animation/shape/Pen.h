#pragma once

#include "../types.h"

class Bitmap;
class Shape;

enum PenStyle { Brush, Spray };

class Pen {
	Color32 color;
	uint thickness;  // radius
	PenStyle style;
public:
	Pen(Color32 color, uint thickness, PenStyle style = Brush) :color(color), thickness(thickness), style(style) {}
	inline void SetColor(Color32 color) { this->color = color; }
	inline void SetThickness(uint thickness) { this->thickness = thickness; }
	inline uint GetThickness() { return thickness; }
	inline void SetStyle(PenStyle style) { this->style = style; }
	void PaintPoint(Bitmap& bitmap, Pos center);
	//inline void PaintOn(Bitmap& bitmap, Rect rect, const Shape& shape) {
	//	shape.PaintOn(bitmap, rect, *this); 
	//}
};
