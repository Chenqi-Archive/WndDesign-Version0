#pragma once

#include "Shape.h"

namespace WndDesign {

class Plot :public Shape {


public:
	void PaintOn(Board& board)const override;

};

} // namespace WndDesign