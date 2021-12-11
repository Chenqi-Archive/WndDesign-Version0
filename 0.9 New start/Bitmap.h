#pragma once

#include "Figure.h"

namespace FreeUI {
	class Bitmap : public Figure{
	public:
		// Clone an object itself.
		virtual Figure* Clone() const { return new Bitmap(); }
		// All figures can be painted on a bitmap.
		virtual void PaintOn(Bitmap& bitmap) const {}
		// Destructor.
		virtual ~Bitmap() {}
	};
}