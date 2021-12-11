#pragma once

#include "types.h"

namespace WndDesign {

class Bitmap;
class Wnd;

// Board is created by Wnd and used for Figure to paint on.
class Board {
	friend class Wnd;
// For directly painting on the bitmap.
private: 
	Bitmap* bitmap;         
	Rect accessibleRegion;  
	Pos visibleRegionOffset;  // The size of visible region is the same as relative region.

private:
	// For direct set for optimizing.
	friend class Color;

// For getting permission to paint.
private:
	const Wnd* parent;
	Rect relativeRegion;   // The region relative to parent board.
	bool isAllowed;

public:
	Board();
	// Create a board from parent window's own board.
	Board(const Wnd* parent, const Board& board, Rect region = {});
	// Create a board from a bitmap owned by parent window.
	Board(const Wnd* parent, Bitmap* bitmap, Rect region = {});
	bool BeginPaint();      // Get permission to start painting.
	void EndPaint();        // Update the painted region.
	bool BeginPaintRegion(Rect region = {}) const; // Get the permission to paint for a sub board.
	void EndPaintRegion(Rect region = {}) const; // Update the painted region.
	inline bool IsAllowed() const { return isAllowed; }
	void Set(Pos pos, Color32 color);
	Color32 Get(Pos pos)const;
	inline Size Size() const { return relativeRegion.size; }
};


} // namespace WndDesign
