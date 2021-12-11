#include "Board.h"
#include "shape/Bitmap.h"
#include "wnd/Wnd.h"

namespace WndDesign {

Board::Board() :
	bitmap(nullptr), accessibleRegion(), visibleRegionOffset(),
	parent(nullptr), relativeRegion(), isAllowed(false) {}

Board::Board(const Wnd* parent, const Board& board, Rect region) :
	bitmap(board.bitmap), parent(parent), isAllowed(false) {
	if (this->parent == nullptr) { this->parent = board.parent; }
	if (!region.size) { region.size = board.Size(); }
	relativeRegion = region;
	visibleRegionOffset = relativeRegion.pos + board.visibleRegionOffset;
	accessibleRegion = board.accessibleRegion.Intersect({ visibleRegionOffset, relativeRegion.size });
}

Board::Board(const Wnd* parent, Bitmap* bitmap, Rect region):
	 bitmap(bitmap), parent(parent),isAllowed(false) {
	if (!region.size) { region.size = bitmap->GetSize(); }
	relativeRegion = region;
	visibleRegionOffset = relativeRegion.pos;
	accessibleRegion = relativeRegion.Intersect({ {}, bitmap->GetSize() });
}

bool WndDesign::Board::BeginPaint() {
	// Send a request to parent window for painting.
	if (!isAllowed) {
		if (parent == nullptr) { isAllowed = false; } 
		else { isAllowed = parent->AllowToPaint(*this); }
	}
	return isAllowed;
}

void WndDesign::Board::EndPaint() {
	if (!isAllowed) { return; }
	parent->RefreshBoard(*this);
	isAllowed = false;
}

bool Board::BeginPaintRegion(Rect region) const {
	if (!region.size) { region.size = Size(); }
	region = region.Intersect({ {}, Size() });

	// May check the accessibleRegion again in case the bitmap changes.

	if (parent == nullptr) { return false; }
	else { return parent->AllowToPaint(*this, region); }
}

void Board::EndPaintRegion(Rect region) const {
	if (!region.size) { region.size = Size(); }
	region = region.Intersect({ {}, Size() });
	if (parent != nullptr) { return parent->RefreshBoard(*this, region); }
}

void WndDesign::Board::Set(Pos pos, Color32 color) {
	if (!isAllowed) { return; }
	pos = pos + visibleRegionOffset;
	if (PosInRect(pos, accessibleRegion)) {
		bitmap->Set(pos, color);
	}
}

Color32 WndDesign::Board::Get(Pos pos) const {
	pos = pos + visibleRegionOffset;
	if (PosInRect(pos, accessibleRegion)) {
		return bitmap->Get(pos);
	}
	return COLOR_BLACK;
}


} // namespace WndDesign
