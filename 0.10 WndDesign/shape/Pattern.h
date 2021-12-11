#pragma once

#include "../basic/Figure.h"

namespace WndDesign {

class Color:public Figure {
	Color32 color;
public:
	Color(Color32 color):color(color) {}
	void Set(Color32 color) { this->color = color; }
	virtual Figure* Clone() const { return new Color(*this); }
	virtual ~Color() {}
	virtual void Paint();
};

class Bitmap:public Figure {
private:
	Size size;
	Color32* bits;
public:
	Bitmap():size(), bits(nullptr) {}
	Bitmap(Size size, Color32 color = COLOR_WHITE):size(size) {
		bits = new Color32[size.height * size.width];
		for (uint i = 0; i < size.width * size.height; ++i) {
			bits[i] = color;
		}
	}
	Bitmap(const Bitmap& bitmap):size(bitmap.size) {
		bits = new Color32[size.height * size.width];
		for (uint i = 0; i < size.width * size.height; ++i) {
			bits[i] = bitmap.bits[i];
		}
	}
	inline Size GetSize() const { return size; }
	inline const void* GetBits() const { return bits; }
	inline void Set(Pos pos, Color32 color) {
		bits[pos.y * size.width + pos.x] = color;  // No boundary checking!
	}
	inline Color32 Get(Pos pos) const {
		return bits[pos.y * size.width + pos.x];
	}
	virtual void Refresh(Rect region);
public:
	virtual Figure* Clone() const { return new Bitmap(*this); }
	virtual ~Bitmap() { if (bits != nullptr) { delete[]bits; } }
	virtual void Paint();
};
} // namespace WndDesign