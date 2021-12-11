#pragma once

#include "types.h"

namespace WndDesign {

class Bitmap;
class Wnd;

// Board is created by Wnd and used for Figure to paint on.
class Board {
private:  
	// These members are protected and should be managed by Wnd. Put them here for convenience.
	const Wnd* parent;      // The parent wnd.
	Bitmap* bitmap;         // The bitmap to paint on.
	Rect accessibleRegion;  // The accessible region.
	bool isAllowed;         // The permission to paint. 
	                        //   If false, then all painting operations will have no effect.

private:
	Rect visibleRegion;

public:
	Board();
	Board(const Wnd* parent, const Board& board, Rect region = {});
	Board(const Wnd* parent, Bitmap* bitmap, Rect region = {});
	bool BeginPaint();      // Get permission to start painting.
	void EndPaint();        // Update the painted region.
	void Set(Pos pos, Color32 color);
	Color32 Get(Pos pos)const;
	inline Size Size() const { return visibleRegion.size; }
};


} // namespace WndDesign
