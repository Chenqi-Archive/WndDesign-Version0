#pragma once

#include "common/core.h"

namespace WndDesign {

struct Size {
	uint width;
	uint height;
};

struct Wnd {
public:
	static Wnd* Create(Wnd* parent, const Style* style, Handler handler);

public:
	bool SetStyle(const Style* style);
	bool SetHandler(Handler handler);


public:
	void Show();
	void Hide();
	void Destroy();
};

struct Button : public Wnd {
	using OnClick = bool (*)();
	static Button* Create(Wnd* parent, const Style* style, OnClick handler);
};


// literal
constexpr int operator ""px(unsigned long long width) {
	return width;
}
constexpr int operator ""pct(unsigned long long width) {
	return width | 0x80000000;
}
constexpr int operator ""deg(unsigned long long degree) {
	return degree;
}

class FontFamily {

};

struct Style {
	enum type {Base, Window, TextBox, Picture, } type = Base;


	// Layout Style:
	Size size = { 800px, 480px };
	Size min_size = { 200px, 200px };

	// Attribute:
	int opacity = 100pct;
	int rotation = 90deg;

	// Font:
	FontFamily* font_family = nullptr;  // Default Font Family
	int font_size = 20px;
	int font_weight;

	int font_color;


	// Border:
	int border_size = 5px;
};

enum class Msg {

	LeftDown,
	LeftUp,
	MouseMove,
};

using Handler = bool (*)(Msg msg, void* para);

constexpr Wnd* desktop = (Wnd*)-1;

#define DefStyle(style, base) struct style : public base { style () { 
//#define DefStyle(style) DefStyle(style, Style)
#define EndDefStyle(style) } };

} // namespace WndDesign

