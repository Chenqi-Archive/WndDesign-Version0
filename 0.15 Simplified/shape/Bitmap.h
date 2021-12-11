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
		for (uint i = 0; i < size.area(); ++i) { bits[i] = color; }
	}
	Bitmap(const Bitmap& bitmap) :size(bitmap.size) {
		bits = new Color32[size.area()];
		for (uint i = 0; i < size.area(); ++i) {
			bits[i] = bitmap.bits[i];
		}
	}
	Bitmap(const wchar_t* fileName) :Bitmap() {
		LoadFromFile(fileName);
	}
	Bitmap& operator=(const Bitmap& bitmap) {
		if (&bitmap == this) { return *this; };
		if (bits == nullptr || (bits != nullptr && size.area() != bitmap.size.area())) {
			if (bits != nullptr) { delete[]bits; }
			bits = new Color32[bitmap.size.area()];
		}
		size = bitmap.size;
		for (uint i = 0; i < size.area(); ++i) { bits[i] = bitmap.bits[i]; }
		return *this;
	}
	~Bitmap() override { if (bits != nullptr) { delete[]bits; } }
	void PaintOn(Board& board) const override {}

public:
	inline void Resize(Size size) {  // No initializing!
		if (bits == nullptr || (bits != nullptr && this->size.area() != size.area())) {
			if (bits != nullptr) { delete[]bits; }
			bits = new Color32[size.area()];
		}
		this->size = size;
	}
	inline Size GetSize() const { return size; }
	inline const void* GetBits() const { return bits; }
	inline void Clear(Color32 color) {
		for (uint i = 0; i < size.area(); ++i) { bits[i] = color; }
	}
	inline void Set(Pos pos, Color32 color) {
		if ((uint)pos.x >= size.width || (uint)pos.y >= size.height) { return; }
		bits[pos.y * size.width + pos.x] = color;
	}
	inline Color32 Get(Pos pos) const {
		if ((uint)pos.x >= size.width || (uint)pos.y >= size.height) { return COLOR_WHITE; }
		return bits[pos.y * size.width + pos.x];
	}

public:
	bool LoadFromFile(const wchar_t* fileName);
	bool SaveToFile(const wchar_t* fileName);
};


} // namespace WndDesign