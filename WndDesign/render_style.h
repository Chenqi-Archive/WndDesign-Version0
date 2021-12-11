#pragma once

using uint = unsigned int;


// Directly interact with system APIs.

struct RenderStyleBase {
	// Type
	enum class Type{ None, Wnd, TextBox, Bitmap} type = Type::None;

	// Painting Rectangle
	int x;
	int y;
	uint width;
	uint height;

	// Effect
	uint opacity;
	//  Blur filter

	// Transform
	//  Matrix transform

	// Clipping
};

struct RenderStyleWnd : public RenderStyleBase {
	RenderStyleWnd() {
		RenderStyleBase::type = Type::Wnd;
	}
};

struct RenderStyleTextBox : public RenderStyleBase {

};