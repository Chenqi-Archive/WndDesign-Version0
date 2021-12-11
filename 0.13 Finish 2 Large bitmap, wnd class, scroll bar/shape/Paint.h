#pragma once

#include "../types.h"

namespace WndDesign {

class Board;

enum class PenStyle { None, Solid, Spray };

class Pen {
	Color32 color;
	uint thickness;  // diameter
	PenStyle style;
public:
	Pen():color(COLOR_BLACK), thickness(1), style(PenStyle::Solid) {}
	Pen(Color32 color, uint thickness, PenStyle style = PenStyle::Solid)
		:color(color), thickness(thickness), style(style) {}
	inline void SetColor(Color32 color) { this->color = color; }
	inline Color32 Color() const { return this->color; }
	inline void SetThickness(uint thickness) { this->thickness = thickness; }
	inline uint Thickness() const { return thickness; }
	inline void SetStyle(PenStyle style) { this->style = style; }
	inline PenStyle Style() const { return style; }
	void Paint(Board& board, Pos center) const;
};


enum class BrushStyle { None, Solid, Spray };

class Brush {
	Color32 color;
	BrushStyle style;
public:
	Brush(Color32 color, BrushStyle style = BrushStyle::Solid)
		:color(color), style(style) {}
	inline void SetColor(Color32 color) { this->color = color; }
	inline void SetStyle(BrushStyle style) { this->style = style; }
	inline BrushStyle Style() const { return style; }
	void Paint(Board& board, Pos center, uint maxSpanPixel = (uint)-1) const;
};


} // namespace WndDesign