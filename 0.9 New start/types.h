#pragma once

namespace FreeUI {
typedef unsigned int uint;

struct Pos {
	int x; int y;
	Pos operator+(const Pos& pos) const { return {x + pos.x, y + pos.y}; }
	Pos operator-(const Pos& pos) const { return {x - pos.x, y - pos.y}; }
	bool operator==(const Pos& pos) const { return x == pos.x && y == pos.y; }
	bool operator!=(const Pos& pos) const { return x != pos.x || y != pos.y; }
};
typedef struct { uint width; uint height; }Size;

typedef struct { Pos pos; Size size; }Rect;

}