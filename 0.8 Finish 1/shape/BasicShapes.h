#pragma once

#include "./Bitmap.h"
#include "./Shape.h"

#define _USE_MATH_DEFINES
#include <cmath>

#define min(x, y)  ((x)<(y)?(x):(y))
#define max(x, y)  ((x)>(y)?(x):(y))

// Pure color
class Color :public Shape {
	Color32 color;
public:
	Color(Color32 color) :color(color) {}
	void Set(Color32 color) { this->color = color; }
	virtual Shape* Clone(void* para) const { return new Color(*this); }
	virtual ~Color() {}
	// Pure color, ignore the pen.
	virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) const {
		Size& size = rect.size;
		Pos& offset = rect.pos;
		if ((size.height || size.width) == 0) {
			size = bitmap.GetSize();
		}

		// May first calculate the overlapped region.

		for (uint j = 0; j < size.height; ++j) {
			for (uint i = 0; i < size.width; ++i) {
				bitmap.Set(i + offset.x, j + offset.y, color);
			}
		}
	}
};

class Point :public Shape {
public:
	virtual Shape* Clone(void* para) const { return new Point(*this); }
	virtual ~Point() {}
	// Ignore the rect size.
	virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) const {
		pen.PaintPoint(bitmap, rect.pos);
	}
};

class Circle :public Shape {
public:
	virtual Shape* Clone(void* para) const { return new Circle(*this); }
	virtual ~Circle() {}
	virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) const {
		Size& size = rect.size;
		Pos& offset = rect.pos;
		if ((size.height || size.width) == 0) {
			size = bitmap.GetSize();
		}
		uint radius = min(size.height, size.width) / 2;
		if (radius == 0) { return; }  // Too small!
		double increment = atan(1.0 / radius);
		int cx = offset.x + size.width / 2;
		int cy = offset.y + size.height / 2;
		for (double t = 0; t < 2 * M_PI; t += increment){
			int x = radius * cos(t) + cx;
			int y = radius * sin(t) + cy;
			pen.PaintPoint(bitmap, { x,y });
		}
	};
};

class Arc :public Shape {
	uint beginAngle, endAngle;
public:
	// Angle: 0-359. Point moves CLOCKWISE while angle increases!
	Arc(uint beginAngle, uint endAngle): beginAngle(beginAngle % 360), endAngle(endAngle % 360){}
	void Set(uint beginAngle, uint endAngle) { this->beginAngle = beginAngle % 360; this->endAngle = endAngle % 360; }
	virtual Shape* Clone(void* para) const { return new Arc(*this); }
	virtual ~Arc() {}
	virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) const {
		Size& size = rect.size;
		Pos& offset = rect.pos;
		if ((size.height || size.width) == 0) {
			size = bitmap.GetSize();
		}
		uint radius = min(size.height, size.width) / 2;
		if (radius == 0) { return; }  // Too small!
		double increment = atan(1.0 / radius);
		int cx = offset.x + size.width / 2;
		int cy = offset.y + size.height / 2;
		for (double t = beginAngle / 180.0 * M_PI; t < endAngle / 180.0 * M_PI; t += increment){
			int x = radius * cos(t) + cx;
			int y = radius * sin(t) + cy;
			pen.PaintPoint(bitmap, { x,y });
		}
	}
};

class Line :public Shape {
	Pos begin, end;
public:
	Line(Pos begin, Pos end) :begin(begin), end(end) {}
	void Set(Pos begin, Pos end) { this->begin = begin; this->end = end; }
	virtual Shape* Clone(void* para) const { return new Line(*this); }
	virtual ~Line() {}
	// Ignore the rect
	virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) const {
		Pos vector = end - begin;
		if (vector == Pos{ 0, 0 }) {  // Just a point.
			pen.PaintPoint(bitmap, begin);
			return;
		}
		double increment = 1.0 / max(abs(vector.x), abs(vector.y));
		for (double t = 0; t <= 1.0; t += increment) {
			int x = begin.x + vector.x * t;
			int y = begin.y + vector.y * t;
			pen.PaintPoint(bitmap, { x, y });
		}
	}
};

// Shape Rectangle, which is different from Rect structure.
class Rectangle :public Shape {
public:
	virtual Shape* Clone(void* para) const { return new Rectangle(*this); }
	virtual ~Rectangle() {}
	virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) const {
		Size& size = rect.size;
		Pos& offset = rect.pos;
		if ((size.height || size.width) == 0) {
			size = bitmap.GetSize();
		}
		for (uint t = 0; t < size.width; ++t) {
			int x = offset.x + t;
			pen.PaintPoint(bitmap, { x, offset.y });
			pen.PaintPoint(bitmap, { x, offset.y + (int)size.height - 1 });
		}
		for (uint t = 1; t < size.height - 1; ++t) {
			int y = offset.y + t;
			pen.PaintPoint(bitmap, { offset.x, y });
			pen.PaintPoint(bitmap, { offset.x + (int)size.width - 1 , y });
		}
	}
};

class RoundedRectangle :public Shape {
	uint radiusRatio;
public:
	RoundedRectangle(uint radiusRatio):radiusRatio(min(radiusRatio,100)){}
	void Set(uint radiusRatio) { this->radiusRatio = radiusRatio; }
	virtual Shape* Clone(void* para) const { return new RoundedRectangle(*this); }
	virtual ~RoundedRectangle() {}
	virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) const {

		pen.SetColor(COLOR_BLUE);
		pen.SetThickness(10);
		pen.SetStyle(Spray);

		Size& size = rect.size;
		Pos& offset = rect.pos;
		if ((size.height || size.width) == 0) {
			size = bitmap.GetSize();
		}
		//uint thickness = pen.GetThickness();
		//offset.x += thickness;
		//offset.y += thickness;
		//size.height -= 2 * thickness;
		//size.width -= 2 * thickness;

		uint radius = min(size.height, size.width) * radiusRatio / 200;

		// Rraw the arcs.
		if (radius != 0) {
			int cx[4] = { size.width - radius, radius, radius, size.width - radius };
			int cy[4] = { size.height - radius, size.height - radius, radius, radius };
			double increment = atan(1.0 / radius);
			double t = 0;
			for (int i = 1; i <= 4; ++i) {
				for (; t < M_PI * i / 2; t += increment) {
					int x = radius * cos(t) + cx[i - 1] + offset.x;
					int y = radius * sin(t) + cy[i - 1] + offset.y;
					pen.PaintPoint(bitmap, { x,y });
				}
			}
		}

		for (uint t = radius; t < size.width - radius; ++t) {
			int x = offset.x + t;
			pen.PaintPoint(bitmap, { x, offset.y });
			pen.PaintPoint(bitmap, { x, offset.y + (int)size.height - 1 });
		}
		for (uint t = radius; t < size.height - radius; ++t) {
			int y = offset.y + t;
			pen.PaintPoint(bitmap, { offset.x, y });
			pen.PaintPoint(bitmap, { offset.x + (int)size.width - 1 , y });
		}
	}
};
