#include "Bitmap.h"

namespace WndDesign{

void WndDesign::Color::PaintOn(Board& board) const {
	//const Size& size = board.Size();
	//for (uint j = 0; j < size.height; ++j) {
	//	for (uint i = 0; i < size.width; ++i) {
	//		board.Set(Pos{i, j}, color);
	//	}
	//}

	// Directly set on the bitmap.
	Bitmap* bitmap = board.bitmap; // Already checked for permission.
	Rect region = board.accessibleRegion.Intersect({ {}, bitmap->GetSize() });  // Incase the 
	for(uint i = region.pos)
}

////////////////////  Bitmap ///////////////////////////

void WndDesign::Bitmap::PaintOn(Board& board) const {
	const Size& size = board.Size();
	if (size == this->GetSize()) {
		for (uint j = 0; j < size.height; ++j) {
			for (uint i = 0; i < size.width; ++i) {
				board.Set(Pos{ i, j }, DirectGet({ i,j }));
			}
		}
	} else {
		for (uint j = 0; j < size.height; ++j) {
			for (uint i = 0; i < size.width; ++i) {
				Pos pos = {i * this->GetSize().width / size.width, j * this->GetSize().height / size.height};
				board.Set(Pos{i, j}, DirectGet(pos));
			}
		}
	}
}

void Bitmap::PaintOn(Pos pos, Board& board, Rect region) const {
	if (!region.size) { region.size = board.Size(); }
	region = region.Intersect({ {}, board.Size() });
	const Size& size = region.size;
	for (uint j = 0; j < size.height; ++j) {
		for (uint i = 0; i < size.width; ++i) {
			board.Set(region.pos + Pos{ i, j }, DirectGet(pos + Pos{ i,j }));
		}
	}
}

} // namespace WndDesign