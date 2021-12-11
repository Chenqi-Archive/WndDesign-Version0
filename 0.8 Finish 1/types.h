// types.h
//   Defines basic types and messages.

#pragma once

typedef	unsigned int uint;
typedef unsigned char uchar;

// Position could be negetive.
struct Pos{ 
	int x; int y; 
	Pos operator+(const Pos& pos) const { return { x + pos.x, y + pos.y }; }
	Pos operator-(const Pos& pos) const{return { x - pos.x, y - pos.y };}
	bool operator==(const Pos& pos) const { return x == pos.x && y == pos.y; }
	bool operator!=(const Pos& pos) const { return x != pos.x || y != pos.y; }
};
typedef struct { uint width; uint height; }Size;

typedef struct { Pos pos; Size size; }Rect;


struct Color32 {
	uchar blue; uchar green; uchar red; uchar alpha;
	Color32():Color32(0xFFFFFF00){}
	Color32(uchar blue, uchar green, uchar red, uchar alpha) :
		blue(blue), green(green), red(red), alpha(alpha) {}
	Color32(int blue, int green, int red, int alpha) :
		blue((uchar)blue), green((uchar)green), red((uchar)red), alpha((uchar)alpha) {}
	Color32(int color):blue((color>>24)&0xff), green((color >> 16) & 0xff), 
		red((color >> 8) & 0xff), alpha(color & 0xff) {}
	//Color32(const Color32& color) :
	//	blue(color.blue), green(color.green), red(color.red), alpha(color.alpha) {}
	operator int() { return (blue << 24) | (green << 16) | (red << 8) | alpha; }
};

// Color should be in int (alpha red green blue)

#define COLOR_BLUE Color32(0xFF000000)
#define COLOR_GREEN Color32(0x00FF0000)
#define COLOR_RED Color32(0x0000FF00)
#define COLOR_WHITE Color32(0xFFFFFF00)
#define COLOR_BLACK Color32(0x00000000)
#define COLOR_RAND Color32(rand()%0xff, rand()%0xff,rand()%0xff,0)




