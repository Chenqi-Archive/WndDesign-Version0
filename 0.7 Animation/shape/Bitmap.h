#pragma once

#include "./Shape.h"
#include "../lib/matrix.h"

class Bitmap :public Shape, public matrix<Color32> {
public:
	virtual Shape* Clone(void* para) const { return new Bitmap(*this); }
	virtual ~Bitmap() {}

	// Ignore the Pen. Default size is the size of myself.
	virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) const {
		Pos& pos = rect.pos;
		Size& size = rect.size;
		if ((size.height || size.width) == 0) {
			size = GetSize();
		}
		Size mySize = GetSize();

		// May first calculate the overlapped region.

		for (uint j = 0; j < size.height; ++j) {
			for (uint i = 0; i < size.width; ++i) {
				uint myi = i * mySize.width / size.width;
				uint myj = j * mySize.height / size.height;
				bitmap.Set(i + pos.x, j + pos.y, bits[myj * width + myi]);
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

	// Get bitmap from file. Supports .bmp .jpg .png .ico, etc. And self defined type: sbmp.
	bool OpenFromFile(const wchar_t* fileName);

	// Save bitmap to file. 
	bool SaveToFile(const wchar_t* fileName);
};
