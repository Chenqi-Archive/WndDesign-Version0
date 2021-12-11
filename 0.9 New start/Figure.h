#pragma once

#include "display.h"

#include <list>
#include <vector>

namespace FreeUI {

// Figure: pure virtual class, base for Animation, Shape and Wnd.
class Figure {
public:
	// Clone an object itself.
	virtual Figure* Clone() const = 0;
	// All figures can be painted on a display area.
	virtual void PaintOn(DisplayArea& displayArea) const = 0;
	// Destructor.
	virtual ~Figure() {}
};

class Line:public Figure {

};

// GridLine has grids that are seperated by multiple lines.
class GridLine:public Figure {
	// Columns are more fixed than rows.
	std::vector<uint> colSize;
	std::vector<Line*> colLine; // One more than size.
	std::list<uint> rowSize;
	std::list<Line*> rowLine;
};

} // namespace FreeUI