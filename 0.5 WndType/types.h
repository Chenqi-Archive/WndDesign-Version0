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

typedef struct { uchar blue; uchar green; uchar red; uchar alpha; }Color32;

#define COLOR_BLUE Color32{0xff, 0, 0, 0}
#define COLOR_GREEN Color32{0, 0xff, 0, 0}
#define COLOR_RED Color32{0, 0, 0xff, 0}



