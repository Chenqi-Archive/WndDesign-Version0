// Shape.h:
//   Defines the Shape class.

#pragma once

#include "../types.h"
#include "./Pen.h"

class Bitmap;

// The Shape base class, which can be drawed on a bitmap object with pen.
class Shape {
public:
	// For cloning a derived object.
	virtual Shape* Clone(void* para) const = 0;

	// Paint Shape on an initialized bitmap, at offset with size(fill 0 if bitmap size) with Pen.
	// You may use parameter equations to draw the outline.
	virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) const = 0;

	// Deconstructor is needed!
	virtual ~Shape() = 0;  // Defination is in Pen.cpp.
};