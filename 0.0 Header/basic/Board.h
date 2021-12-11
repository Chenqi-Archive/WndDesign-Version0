#pragma once

#include "basictypes.h"

namespace WndDesign {

DECLARE_CLASS(WndBitmap)
class Board {
	WndBitmap* bitmap;
	Rect region;
public:
	Board() :bitmap(nullptr), region() {};
	Board(WndBitmap* bitmap, Rect region);
	Board(const Board& board, Rect region);
	inline Size GetSize() const { return region.size; }
	void Clear(Color32 color = COLOR_WHITE);
	void Set(Pos pos);
	Color32 Get(Pos pos) const;
	void Flush() const;
};

} // namespace WndDesign