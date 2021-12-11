#pragma once

#include "./Shape.h"
#include "../lib/matrix.h"

class Bitmap :public Shape, public matrix<Color32> {
public:
	// Ignore the rect size and Pen. (May support rescale later)
	virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) const {
		Pos& pos = rect.pos;

		// May first calculate the overlapped region.

		for (uint j = 0; j < height; ++j) {
			for (uint i = 0; i < width; ++i) {
				bitmap.Set(i + pos.x, j + pos.y, bits[j * width + i]);
			}
		}
	}

public:
	Bitmap() :matrix<Color32>() {}
	Bitmap(uint width, uint height, const Color32& color = COLOR_WHITE)
		:matrix<Color32>(width, height, color) {}
	Bitmap(Size size, const Color32& color = COLOR_WHITE)
		:matrix<Color32>(size.width, size.height, color) {}
	void* GetBits() const { return bits; }

	// Get bitmap from file. Supports .bmp .jpg .png .ico .
	bool OpenFromFile(const wchar_t* fileName);

	// Save bitmap to file. 
	bool SaveToFile(const wchar_t* fileName);
};
