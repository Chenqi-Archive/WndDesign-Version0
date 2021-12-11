#pragma once

#include "List.h"

typedef unsigned char uchar;
typedef unsigned int uint;

// Pos: position of a subwindow relative to main window.
typedef struct {
	uint x;
	uint y;
}Pos;

typedef struct {
	uint width;
	uint height;
}Size;




typedef uint Style;
#define WND_SHOW 0x0
#define WND_HIDE 0x1

struct Color32 {
	uchar blue;
	uchar green;
	uchar red;
	uchar alpha;
};

class Wnd;

// SubWndInfo structure defination
struct SubWndInfo {
	Wnd* wnd;
	Pos pos;  // Left top coordinate of sub window
	Style style; // show, hide, 
};

template<class T>
class Matrix {
protected:
	uint width;
	uint height;
	T* bits;

public:
	Matrix<T>() { width = 0; height = 0; bits = nullptr; }
	Matrix<T>(uint width, uint height, const T& item = 0) {
		Init(width, height, item);
	}
	void Init(uint width, uint height, const T& item = 0) {
		this->width = width; this->height = height;
		bits = new T[width * height];
		for (uint i = 0; i < width * height; ++i) {
			bits[i] = item;
		}
	}
	// Set point(i,j), i is column number and j is row number.
	bool Set(uint i, uint j, const T& item) {
		if (i < width && j < height) {
			bits[j * width + i] = item;
			return true;
		}
		return false;
	}
	bool Get(uint i, uint j, T& pItem) const{
		if (i < width && j < height) {
			pItem = bits[j * width + i];
			return true;
		}
		return false;
	}
	~Matrix() { if (bits != nullptr) { delete[]bits; } }
};

// 32-bit bitmap
class Bitmap:public Matrix<Color32> {
	friend class Wnd;
public:
	Bitmap():Matrix<Color32>(){}
	Bitmap(uint width, uint height, 
		const Color32& color = { 0xff, 0xff, 0xff, 0x0 })
		:Matrix<Color32>(width, height, color) {}
	void Paint(const Bitmap& bitmap, Pos pos = { 0,0 }) {
		// bitmap can't be itself
		Color32 color;
		for (uint j = 0; j < bitmap.height; ++j) {
			for (uint i = 0; i < bitmap.width; ++i) {
				bitmap.Get(i, j, color);
				Set(pos.x + i, pos.y + j, color);
				// May implement alpha mix
			}
		}
	}
	void* GetBits() { return bits; }
};

class WndIndex : public Matrix<SubWndInfo*> {
};

class Wnd
{
private:
	List<SubWndInfo> subWnds;
	const Wnd* const parentWnd;

	WndIndex subWndIndex;

	Bitmap display;

private:
	void CreateSubWnd(Pos pos, Size size, Style stype);
	// Add a created Wnd as subwindow
	void PaintSubWnd(const SubWndInfo& subWnd);

private:
	void InitDisplay();
	void InitSubWndIndex();

public:	
	void AddSubWnd(const SubWndInfo& subWnd);

	// Must specify a parent window while creating a subwindow, 
	//   fill NULL if the window is directly created under system.
	Wnd(uint width, uint height, const Wnd* parentWnd);
	~Wnd();

	void* GetBitmap();
	void OnClick(uint x, uint y);
	void OnRedraw();
};


// Button class:
//   No subwindows or subWndIndex, receives click message only
class Button:public Wnd{

public:	
	Button(uint width, uint height, const Wnd* parentWnd) :Wnd(width, height, parentWnd) {
	}

	void OnHover() {

	}
	void OnClick() {

	}
};