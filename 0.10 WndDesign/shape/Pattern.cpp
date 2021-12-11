#include "Pattern.h"

void WndDesign::Color::Paint() {
	Size size = board.GetSize();
	for (uint j = 0; j < size.height; ++j) {
		for (uint i = 0; i < size.width; ++i) {
			board.Set(Pos{i, j}, color);
		}
	}
	board.Flush();
}

void WndDesign::Bitmap::Refresh(Rect region) {


}

void WndDesign::Bitmap::Paint() {
	Size size = board.GetSize();
	if (size == this->size) {
		for (uint j = 0; j < size.height; ++j) {
			for (uint i = 0; i < size.width; ++i) {
				board.Set(Pos{i, j}, bits[j * this->size.width + i]);
			}
		}
	} else {
		for (uint j = 0; j < size.height; ++j) {
			for (uint i = 0; i < size.width; ++i) {
				uint myi = i * this->size.width / size.width;
				uint myj = j * this->size.height / size.height;
				board.Set(Pos{i, j}, bits[myj * this->size.width + myi]);
			}
		}
	}
}
