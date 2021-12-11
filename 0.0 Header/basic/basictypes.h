#pragma once

#include "type.h"

namespace WndDesign {

#ifndef max
#define max(a, b)   (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a, b)   (((a) < (b)) ? (a) : (b))
#endif

#define DivideCeil(a, b) (((a - 1) / b) + 1)

struct Size {
	uint width; uint height;
	inline bool operator==(const Size& size) const { return width == size.width && height == size.height; }
	inline uint area() const { return width * height; }
	operator bool() const { return width != 0 && height != 0; }
};

struct Pos {
	int x; int y;
	Pos() :x(0), y(0) {}
	Pos(int x, int y) :x(x), y(y) {}
	Pos(uint x, uint y) :x((int)x), y((int)y) {}
	Pos(float x, float y) :x((int)x), y((int)y) {}
	Pos(double x, double y) :x((int)x), y((int)y) {}
	inline Pos operator+(const Pos& pos) const { return { x + pos.x, y + pos.y }; }
	inline Pos operator+(const Size& size) const { return { x + (int)size.width, y + (int)size.height }; }
	inline Pos operator-(const Pos& pos) const { return { x - pos.x, y - pos.y }; }
	inline bool operator==(const Pos& pos) const { return x == pos.x && y == pos.y; }
	inline bool operator!=(const Pos& pos) const { return x != pos.x || y != pos.y; }
	inline bool operator>(const Pos& pos) const { return x > pos.x && y > pos.y; }
	inline bool operator<(const Pos& pos) const { return x < pos.x&& y < pos.y; }
	inline bool operator>=(const Pos& pos) const { return x >= pos.x && y >= pos.y; }
	inline bool operator<=(const Pos& pos) const { return x <= pos.x && y <= pos.y; }
};

struct Rect {
	Pos pos; Size size;
	inline Pos EndPos()const { return pos + size; }
	inline Rect operator+(const Pos& pos)const { return { this->pos + pos, size }; }
	inline Rect operator-(const Pos& pos)const { return { this->pos - pos, size }; }
	inline bool operator==(const Rect& rect) const { return pos == rect.pos && size == rect.size; }
	inline Rect Intersect(const Rect& rect) const {
		Pos posl1 = pos, posh1 = pos + size,
			posl2 = rect.pos, posh2 = rect.pos + rect.size;
		Pos posl = { max(posl1.x, posl2.x), max(posl1.y, posl2.y) },
			posh = { min(posh1.x, posh2.x), min(posh1.y, posh2.y) };
		if (posh > posl) { return { posl, {uint(posh.x - posl.x), uint(posh.y - posl.y)} }; }
		return { posl, {} };
	}
	inline Rect Union(const Rect& rect) const {
		Pos posl1 = pos, posh1 = pos + size,
			posl2 = rect.pos, posh2 = rect.pos + rect.size;
		Pos posl = { min(posl1.x, posl2.x), min(posl1.y, posl2.y) },
			posh = { max(posh1.x, posh2.x), max(posh1.y, posh2.y) };
		return { posl, {uint(posh.x - posl.x), uint(posh.y - posl.y)} };
	}
};

inline bool PosInRect(const Pos& pos, const Rect& rect) {
	return (pos >= rect.pos) && (pos < (rect.pos + rect.size));
}

inline bool RectInRect(const Rect& smallOne, const Rect& largeOne) {
	return (smallOne.pos >= largeOne.pos) && (smallOne.EndPos() <= largeOne.EndPos());
}

struct Color32 {
	uchar blue; uchar green; uchar red; uchar alpha;
	Color32() :Color32(0x00FFFFFF) {}
	Color32(uchar alpha, uchar red, uchar green, uchar blue) :alpha(alpha), red(red), green(green), blue(blue) {}
	Color32(int alpha, int red, int green, int blue) :alpha(alpha), red(red), green(green), blue(blue) {}
	Color32(uint color) { alpha = red = green = blue = 0; *(uint*)this = color; }
	inline operator int() const { return *(int*)this; }
	inline bool operator==(const Color32& color) const { return (int)*this == (int)color; }
};

#define COLOR_BLUE	Color32(0xFF0000FF)
#define COLOR_GREEN Color32(0xFF00FF00)
#define COLOR_RED	Color32(0xFFFF0000)
#define COLOR_WHITE	Color32(0xFFFFFFFF)
#define COLOR_BLACK	Color32(0xFF000000)

} // namespace WndDesign