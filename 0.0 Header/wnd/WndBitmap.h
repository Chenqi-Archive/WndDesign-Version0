#pragma once

#include "Wnd.h"
#include "../shape/Bitmap.h"

namespace WndDesign {

class WndBitmap :public Bitmap {
private:
	Wnd* parent;
public:
	inline void Flush(Rect region) {
		parent->FlushBitmap(this, region);
	}
};


} // namespace WndDesign