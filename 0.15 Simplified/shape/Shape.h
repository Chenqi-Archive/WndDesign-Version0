#pragma once

#include "../basic/Figure.h"
#include "Paint.h"
namespace WndDesign {

class Shape :public Figure {
public:
	Pen pen;
	Brush brush;
};

} // namespace WndDesign