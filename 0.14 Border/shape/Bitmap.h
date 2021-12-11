#pragma once

#include "../Figure.h"
#include "../lib/matrix.h"

namespace WndDesign {

class Color :public Figure {
	Color32 color;
public:
	void PaintOn(Board& board)const override;

public:
	Color(Color32 color) :color(color) {}
	void Set(Color32 color) { this->color = color; }
};


enum class BitmapType { FixedNormal, Normal, Large };

#define MAX_SINGLE_BITMAP_WIDTH 1024
#define MAX_SINGLE_BITMAP_HEIGHT 768

#define DEFAULT_BLOCK_WIDTH 256
#define DEFAULT_BLOCK_HEIGHT 256

class Bitmap :public Figure {
private:
	Size size;
	Color32 defaultColor;
	BitmapType type;
	// For normal size bitmaps.
	matrix<Color32> bitmap;
	// For large bitmaps.
	matrix<matrix<Color32>*> blocks;
	Size blockSize;

private:
	inline void DirectSet(Pos pos, Color32 color) {  // No boundary checking!
		if (type != BitmapType::Large) { return bitmap.DirectSet(pos, color); }
		matrix<Color32>* block;
		Pos matPos = { pos.x / blockSize.width, pos.y / blockSize.height };
		Pos blockPos = { pos.x % blockSize.width, pos.y % blockSize.height };
		blocks.Get(matPos, block);
		if (block == nullptr) { block = new matrix<Color32>(blockSize, defaultColor); blocks.Set(matPos, block); }
		block->Set(blockPos, color);
	}
	inline Color32 DirectGet(Pos pos) const {  // No boundary checking!
		Color32 color = defaultColor;
		if (type != BitmapType::Large) { bitmap.DirectGet(pos, color);  return color; }
		matrix<Color32>* block = nullptr;
		Pos matPos = { pos.x / blockSize.width, pos.y / blockSize.height };
		Pos blockPos = { pos.x % blockSize.width, pos.y % blockSize.height };
		blocks.DirectGet(matPos, block);
		if (block == nullptr) { return color; }
		block->DirectGet(blockPos, color); return color;
	}

public:
	// Paint the whole bitmap on the board, stretch if size is not equal.	
	void PaintOn(Board& board)const override;   
	// Paint a bitmap region starting at pos on a board region.
	void PaintOn(Pos pos, Board& board, Rect region = {})const;

public:
	Bitmap() :size(), defaultColor(COLOR_WHITE), type(BitmapType::Normal), blockSize({ DEFAULT_BLOCK_WIDTH, DEFAULT_BLOCK_HEIGHT }) {}
	Bitmap(Size size, Color32 color = COLOR_WHITE, BitmapType type = BitmapType::Large) :
		size(size), defaultColor(color), type(type), blockSize(size) {
		if (type != BitmapType::FixedNormal) {
			type = BitmapType::Normal;
			if (size.width > MAX_SINGLE_BITMAP_WIDTH) { type = BitmapType::Large; blockSize.width = DEFAULT_BLOCK_WIDTH; }
			if (size.height > MAX_SINGLE_BITMAP_HEIGHT) { type = BitmapType::Large; blockSize.height = DEFAULT_BLOCK_HEIGHT; }
		}
		if (type == BitmapType::Large) { blocks = matrix<matrix<Color32>*>( {DivideCeil(size.width,blockSize.width), DivideCeil(size.height,blockSize.height)} ); }
		else { bitmap = matrix<Color32>(size, defaultColor); }
	}
	~Bitmap() {
		if (type != BitmapType::Large) { return; }
		matrix<Color32>* block;
		for (uint j = 0; j < blocks.GetSize().height; ++j) {
			for (uint i = 0; i < blocks.GetSize().width; ++i) {
				if (blocks.Get({ i,j }, block) && block != nullptr) {
					delete block;
				}
			}
		}
	}
	inline Size GetSize() const { return size; }
	inline void Resize(Size size, Color32 color = COLOR_WHITE) {
		if (type == BitmapType::FixedNormal) {

		}

		//

	}
	inline bool Set(Pos pos, Color32 color) {
		if (type != BitmapType::Large) { return bitmap.Set(pos, color); }
		matrix<Color32>* block;
		Pos matPos = { pos.x / blockSize.width, pos.y / blockSize.height };
		if (!blocks.Get(matPos, block)) { return false; }
		if (block == nullptr) { block = new matrix<Color32>(blockSize, defaultColor); blocks.Set(matPos, block); }
		Pos blockPos = { pos.x % blockSize.width, pos.y % blockSize.height };
		return block->Set(blockPos, color);
	}
	inline Color32 Get(Pos pos) const {
		Color32 color = defaultColor;
		if (type != BitmapType::Large) { bitmap.Get(pos, color);  return color; }
		matrix<Color32>* block = nullptr;
		Pos matPos = { pos.x / blockSize.width, pos.y / blockSize.height };
		blocks.Get(matPos, block);
		if (block == nullptr) { return color; }
		Pos blockPos = { pos.x % blockSize.width, pos.y % blockSize.height };
		block->Get(blockPos, color); return color;
	}
	inline const void* GetBits()const {
		if (type == BitmapType::Large) { return nullptr; }
		return bitmap.GetBits();
	}
};



} // namespace WndDesign