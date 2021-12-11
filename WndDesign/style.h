#pragma once



using uint = unsigned int;

// Style




// Direct interact with users.

struct WndStyle {
	// Dimension: px, pct(%)
	// Conflict?
	uint width;
	uint height;
	uint max_width;
	uint max_height;
	uint min_width;
	uint min_height;


	int left;
	int top;
	//int right;
	//int bottom;

	// 
	uint border_size;
	uint border_color;


	// 
	uint background;

	//
	uint opacity;


	bool allow_child_transparent;
};


struct FontStyle {

};


// Static Text Box
struct TextBoxStyle {

	// Border

	// Padding
	int padding_left;
	int padding_right;
	int padding_top;
	int padding_bottom;

};



struct ButtonStyle
