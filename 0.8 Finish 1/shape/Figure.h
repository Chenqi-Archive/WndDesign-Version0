// Shape.h:
//   Defines the Shape class.

#pragma once

#include "../types.h"
#include "./Pen.h"

class Bitmap;

// The Shape base class, which can be drawed on a bitmap object with pen.
class Figure {
public:
	// For cloning a derived object.
	virtual Figure* Clone(void* para) const = 0;

	// Paint Shape on an initialized bitmap, at offset with size(fill 0 if bitmap size) with Pen.
	// You may use parameter equations to draw the outline.

	// May support rotation.

	// The pen may not be necessary. (Only necessary for lines drawing, other shapes like bitmap don't need it)

	// May combine them to a single structure: PaintInfo{pen, pos, size, rotation}
	virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) = 0;


	// Deconstructor is needed!
	virtual ~Figure() = 0;  // Defination is in Pen.cpp.
};