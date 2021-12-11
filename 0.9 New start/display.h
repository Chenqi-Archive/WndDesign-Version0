#pragma once

#include "types.h"

namespace FreeUI {


class Bitmap;

// Each Figure has a display area.
struct DisplayArea {
	Rect rect;
	Bitmap* bitmap;
};


}