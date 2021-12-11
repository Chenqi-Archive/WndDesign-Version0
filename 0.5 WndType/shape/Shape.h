// Shape.h:
//   Defines the Shape class.

#pragma once

#include "./Bitmap.h"

// The Shape base class, which can be convert to a bitmap object.
class Shape {
public:
	virtual Bitmap Paint(Size size) const = 0;
};

class Color :public Shape{
	Color32 color;
public:
	Color(Color32 color):color(color){}
	void Set(Color32 color) { this->color = color; }
	Bitmap Paint(Size size) const{
		return Bitmap(size, color);
	}
};