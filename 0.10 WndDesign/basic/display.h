#pragma once

#include "types.h"

namespace WndDesign {

class Bitmap;

// Board is a region on a bitmap for diaplay.
class Board {
private:
	Bitmap* display;
	Rect availableRegion;
private:
	Board(const Board& board, Rect region);
	Board(Bitmap* display, Rect region);
	friend Board CreateBoard(const Board& board, Rect region);
	friend Board CreateBoard(Bitmap* display, Rect region);
public:
	Board():display(nullptr), availableRegion() {}
	Color32 Get(Pos pos);
	inline Size GetSize() const { return availableRegion.size; }
	bool Set(Pos pos, Color32 color);
	void Flush();
};

inline Board CreateBoard(Bitmap* display, Rect region = {}) {
	return Board(display, region);
}

inline Board CreateBoard(const Board& board, Rect region = {}) {
	return Board(board, region);
}

} // namespace WndDesign