#include "Board.h"
#include "shape/Bitmap.h"
#include "wnd/Wnd.h"

namespace WndDesign {

Board::Board():parent(nullptr), bitmap(nullptr), accessibleRegion(), isAllowed(false), visibleRegion() {
}

Board::Board(const Wnd* parent, const Board& board, Rect region) :parent(parent), bitmap(board.bitmap), isAllowed(false) {
	if (parent == nullptr) { parent = board.parent; }
	if (!region.size) { region.size = board.Size(); }
	visibleRegion = {region.pos + board.visibleRegion.pos, region.size};
	accessibleRegion = visibleRegion.Intersect({{},bitmap->GetSize()});
}

Board::Board(const Wnd* parent, Bitmap* bitmap, Rect region):parent(parent), bitmap(bitmap), isAllowed(false) {
	if (!region.size) { region.size = bitmap->GetSize(); }
	visibleRegion = region;
	accessibleRegion = visibleRegion.Intersect({{},bitmap->GetSize()});
}

bool WndDesign::Board::BeginPaint() {
	// Send a request to parent window for painting.
	if (isAllowed) { return isAllowed; }
	if (parent == nullptr) {
		isAllowed = false;
	} else {
		isAllowed = parent->AllowToPaint(bitmap, accessibleRegion);
	}
	return isAllowed;
}

void WndDesign::Board::EndPaint() {
	if (!isAllowed) { return; }
	parent->RefreshBoard(bitmap, accessibleRegion);
	isAllowed = false;
}

void WndDesign::Board::Set(Pos pos, Color32 color) {
	if (!isAllowed) { return; }
	pos = pos + visibleRegion.pos;
	if (PosInRect(pos, accessibleRegion)) {
		bitmap->Set(pos, color);
	}
}

Color32 WndDesign::Board::Get(Pos pos) const {
	pos = pos + visibleRegion.pos;
	if (PosInRect(pos, accessibleRegion)) {
		return bitmap->Get(pos);
	}
	return COLOR_BLACK;
}


} // namespace WndDesign
