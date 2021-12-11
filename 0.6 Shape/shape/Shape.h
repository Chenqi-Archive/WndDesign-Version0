// Shape.h:
//   Defines the Shape class.

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
	inline void SetStyle(PenStyle style) { this->style = style; }
	void PaintPoint(Bitmap& bitmap, Pos center);
	inline void PaintOn(Bitmap& bitmap, Rect rect, const Shape& shape);
};

// The Shape base class, which can be drawed on a bitmap object with pen.
class Shape {
public:
	// Paint Shape on an initialized bitmap, at offset with size(fill 0 if bitmap size) with Pen.
	// You may use parameter equations to draw the outline.
	virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) const = 0;
};

void Pen::PaintOn(Bitmap& bitmap, Rect rect, const Shape& shape) { shape.PaintOn(bitmap, rect, *this); }
