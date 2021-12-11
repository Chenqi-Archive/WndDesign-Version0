#include "Bitmap.h"

namespace WndDesign{

void WndDesign::Color::PaintOn(Board& board) const {
	const Size& size = board.Size();
	for (uint j = 0; j < size.height; ++j) {
		for (uint i = 0; i < size.width; ++i) {
			board.Set(Pos{i, j}, color);
		}
	}
}

////////////////////  Bitmap ///////////////////////////

void WndDesign::Bitmap::PaintOn(Board& board) const {
	const Size& size = board.Size();
	if (size == this->GetSize()) {
		for (uint j = 0; j < size.height; ++j) {
			for (uint i = 0; i < size.width; ++i) {
				board.Set(Pos{ i, j }, Get({ i,j }));
			}
		}
	} else {
		for (uint j = 0; j < size.height; ++j) {
			for (uint i = 0; i < size.width; ++i) {
				Pos pos = {i * this->GetSize().width / size.width, j * this->GetSize().height / size.height};
				board.Set(Pos{i, j}, Get(pos));
			}
		}
	}
}

} // namespace WndDesign