#pragma once

#include "Shape.h"

namespace WndDesign {

class Circle:public Shape {
public:
	virtual Figure* Clone() const { return new Circle(*this); }
	virtual ~Circle() {}
	virtual void Paint() ;
};



} // namespace WndDesign