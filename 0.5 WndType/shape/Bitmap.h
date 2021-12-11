#pragma once

#include "../lib/matrix.h"

class Bitmap :public matrix<Color32> {
public:
	Bitmap():matrix<Color32>(){}
	Bitmap(uint width, uint height, const Color32& color = { 0xff, 0xff, 0xff, 0x0 })
		:matrix<Color32>(width, height, color) {}
	Bitmap(Size size, const Color32& color = { 0xff, 0xff, 0xff, 0x0 })
		:matrix<Color32>(size.width, size.height, color) {}
	// Paste another Bitmap on at pos.
	void PasteOn(Pos pos, const Bitmap& bitmap) {
		for (uint j = 0; j < bitmap.height; ++j) {
			for (uint i = 0; i < bitmap.width; ++i) {
				Set(i + pos.x, j + pos.y, bitmap.bits[j * bitmap.width + i]);
			}
		}
	}
	void* GetBits() const{ return bits; }
};


//void Paint(const Bitmap& bitmap, Pos pos = { 0,0 }) {
//	// bitmap can't be itself
//	Color32 color;
//	for (uint j = 0; j < bitmap.height; ++j) {
//		for (uint i = 0; i < bitmap.width; ++i) {
//			bitmap.Get(i, j, color);
//			Set(pos.x + i, pos.y + j, color);
//			// May implement alpha mix
//		}
//	}
//}
