#pragma once

#include "../basic/types.h"
#include "../basic/display.h"

namespace WndDesign {


enum class PenStyle { None, Solid, Spray };

class Pen {
	Color32 color;
	uint thickness;  // diameter
	PenStyle style;
public:
	Pen(Color32 color, uint thickness, PenStyle style = PenStyle::Solid)
		:color(color), thickness(thickness), style(style) {}
	inline void SetColor(Color32 color) { this->color = color; }
	inline Color32 GetColor() const { return this->color; }
	inline void SetThickness(uint thickness) { this->thickness = thickness; }
	inline uint GetThickness() const { return thickness; }
	inline void SetStyle(PenStyle style) { this->style = style; }
	inline PenStyle GetStyle() const { return style; }
	void Paint(Board& board, Pos center) const ;
};


enum class BrushStyle{None, Solid, Spray};

#define BRUSH_SPAN_INF (uint)-1

class Brush {
	Color32 color;
	BrushStyle style;
	uint maxSpan;
public:
	Brush(Color32 color, BrushStyle style = BrushStyle::Solid, uint maxSpan = BRUSH_SPAN_INF)
		:color(color), style(style), maxSpan(maxSpan) {}
	inline void SetColor(Color32 color) { this->color = color; }
	inline void SetStyle(BrushStyle style) { this->style = style; }
	inline BrushStyle GetStyle() const { return style; }
	void Paint(Board& board, Pos center) const;
};


} // namespace WndDesign