#include "display.h"
#include "../shape/Pattern.h"
namespace WndDesign {

Board::Board(const Board& board, Rect region):display(board.display) {
	if (region.size == Size{0, 0}) { region.size = board.availableRegion.size; }
	availableRegion = board.availableRegion.Intersect(region);
}

Board::Board(Bitmap* display, Rect region) : display(display){
	if (region.size == Size{0, 0}) { region.size = display->GetSize(); }
	availableRegion = region.Intersect({{},display->GetSize()});
}

Color32 Board::Get(Pos pos) {
	if (PosInRect(pos, availableRegion)) {
		return display->Get(availableRegion.pos + pos);
	}
	return COLOR_WHITE;
}

bool Board::Set(Pos pos, Color32 color) {
	if (PosInRect(pos, availableRegion)) {
		display->Set(availableRegion.pos + pos, color);
		return true;
	}
	return false;
}

void Board::Flush() {
	display->Refresh(availableRegion);
}


} // namespace WndDesign