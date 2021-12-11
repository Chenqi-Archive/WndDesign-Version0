// Shape.h:
//   Defines the Shape class.

#pragma once

#include "./Figure.h"

class Bitmap;

// The Shape base class, which can be drawed on a bitmap object with pen.
class Shape : public Figure {
public:
	// For cloning a derived object.
	virtual Shape* Clone(void* para) const = 0;

	// Redirected to the const PaintOn function.
	virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) {
		((const Shape*)this)->PaintOn(bitmap, rect, pen);
	}

	// Paint Shape on an initialized bitmap, at offset with size(fill 0 if bitmap size) with Pen.
	// You may use parameter equations to draw the outline.
	virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) const = 0;

	// Deconstructor is needed!
	virtual ~Shape() {}
};