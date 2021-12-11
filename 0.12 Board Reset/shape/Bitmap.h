#pragma once

#include "../Figure.h"
#include "../lib/matrix.h"

namespace WndDesign {

class Color:public Figure {
	Color32 color;
public:
	void PaintOn(Board& board)const override;

public:
	Color(Color32 color):color(color) {}
	void Set(Color32 color) { this->color = color; }
};

#define MAX_SINGLE_BITMAP_WIDTH 1024
#define MAX_SINGLE_BITMAP_HEIGHT 768

#define DEFAULT_BLOCK_WIDTH 256
#define DEFAULT_BLOCK_HEIGHT 256

class Bitmap:public Figure{
private:
	Size size;
	Color32 defaultColor; 
	bool isLarge;
	// For normal size bitmaps.
	matrix<Color32> bitmap;
	// For large bitmaps.
	matrix<matrix<Color32>*> blocks;
	Size blockSize;

private:
	void PaintOn(Board& board)const override;

public:
	Bitmap() :size(), defaultColor(COLOR_WHITE), isLarge(false), blockSize({ DEFAULT_BLOCK_WIDTH, DEFAULT_BLOCK_HEIGHT }) {}
	Bitmap(Size size, Color32 color = COLOR_WHITE):size(size), defaultColor(color), isLarge(false){
		if (size.width > MAX_SINGLE_BITMAP_WIDTH) { isLarge = true; blockSize.width = DEFAULT_BLOCK_WIDTH; } else { blockSize.width = size.width; }
		if (size.height > MAX_SINGLE_BITMAP_HEIGHT) { isLarge = true; blockSize.height = DEFAULT_BLOCK_HEIGHT; } else { blockSize.height = size.height; }
		if (isLarge) { blocks = { {DivideCeil(size.width,blockSize.width), DivideCeil(size.height,blockSize.height)} }; } else { bitmap = { size }; }
	}
	~Bitmap() {
		if (!isLarge) { return; }
		matrix<Color32>* block;
		for (uint j = 0; j < blocks.GetSize().height; ++j) {
			for (uint i = 0; i < blocks.GetSize().width; ++i) {
				if (blocks.Get({i,j}, block) && block != nullptr) {
					delete block;
				}
			}
		}
	}
	inline Size GetSize() const { return size; }
	inline void Resize(Size size, Color32 color = COLOR_WHITE) {
		if (!this->size) {  // Uninitialized.
			this->size = size; defaultColor = color; isLarge = false;
			if (size.width > MAX_SINGLE_BITMAP_WIDTH) { isLarge = true; blockSize.width = DEFAULT_BLOCK_WIDTH; } else { blockSize.width = size.width; }
			if (size.height > MAX_SINGLE_BITMAP_HEIGHT) { isLarge = true; blockSize.height = DEFAULT_BLOCK_HEIGHT; } else { blockSize.height = size.height; }
			if (isLarge) { blocks = { {DivideCeil(size.width,blockSize.width), DivideCeil(size.height,blockSize.height)} }; } else { bitmap = { size }; }
		} else {

		}

	}
	inline bool Set(Pos pos, Color32 color) {
		if (!isLarge) { return bitmap.Set(pos, color); }
		matrix<Color32>* block;
		Pos matPos = {pos.x / blockSize.width, pos.y / blockSize.height};
		if (!blocks.Get(matPos, block)) { return false; }
		if (block == nullptr){block = new matrix<Color32>(blockSize);blocks.Set(matPos, block);}
		Pos blockPos = {pos.x % blockSize.width, pos.y % blockSize.height};
		return block->Set(blockPos, color);
	}
	inline Color32 Get(Pos pos) const {
		Color32 color = defaultColor;
		if (!isLarge) { bitmap.Get(pos, color);  return color; }
		matrix<Color32>* block = nullptr;
		Pos matPos = {pos.x / blockSize.width, pos.y / blockSize.height};
		blocks.Get(matPos, block);
		if(block == nullptr) { return color; }
		Pos blockPos = {pos.x % blockSize.width, pos.y % blockSize.height};
		block->Get(blockPos, color); return color;
	}
};



} // namespace WndDesign