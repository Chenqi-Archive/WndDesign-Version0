#include "display.h"
#include "../shape/Pattern.h"
namespace WndDesign {

Board::Board(const Board& board, Rect region):display(board.display) {
	if (region.size == Size{0, 0}) { region.size = board.visibleRegion.size; }
	visibleRegion = {region.pos + board.visibleRegion.pos, region.size};
	availableRegion = visibleRegion.Intersect({{},display->GetSize()});
}

Board::Board(Bitmap* display, Rect region) : display(display), visibleRegion(region) {
	if (region.size == Size{0, 0}) { region.size = display->GetSize(); }
	visibleRegion = region;
	availableRegion = visibleRegion.Intersect({{},display->GetSize()});
}

Color32 Board::Get(Pos pos) {
	pos = pos + visibleRegion.pos;
	if (PosInRect(pos, availableRegion)) {
		return display->Get(pos);
	}
	return COLOR_WHITE;
}

bool Board::Set(Pos pos, Color32 color) {
	pos = pos + visibleRegion.pos;
	if (PosInRect(pos, availableRegion)) {
		display->Set(pos, color);
		return true;
	}
	return false;
}

void Board::Flush() {
	display->Refresh(availableRegion);
}


} // namespace WndDesign