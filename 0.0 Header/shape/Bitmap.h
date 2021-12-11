#pragma once

#include "../basic/Figure.h"

namespace WndDesign {

class Bitmap : public Figure {
private:
	Size size;
	Color32* bits;
public:
	Bitmap() :size(), bits(nullptr) {}
	Bitmap(Size size) :size(size) {
		bits = new Color32[size.area()];
	}
	Bitmap(Size size, Color32 color) :Bitmap(size) {
		for (uint i = 0; i < size.area(); ++i) {
			bits[i] = color;
		}
	}
	Bitmap(const Bitmap& bitmap) :size(bitmap.size) {
		bits = new Color32[size.area()];
		for (uint i = 0; i < size.area(); ++i) {
			bits[i] = bitmap.bits[i];
		}
	}
	inline Size GetSize() const { return size; }
	inline const void* GetBits() const { return bits; }
	inline void Set(Pos pos, Color32 color) {
		if (pos.x >= size.width || pos.y >= size.height) { return; }
		bits[pos.y * size.width + pos.x] = color;
	}
	inline Color32 Get(Pos pos) const {
		if (pos.x >= size.width || pos.y >= size.height) { return COLOR_WHITE; }
		return bits[pos.y * size.width + pos.x];
	}
	~Bitmap() override { if (bits != nullptr) { delete[]bits; } }
	void PaintOn(Board& board) const override;
};


} // namespace WndDesign