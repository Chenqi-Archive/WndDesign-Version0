#pragma once

#include "Animation.h"
#include "Pattern.h"

namespace WndDesign {

class Blinking :public Animation {
private:
	Color color1, color2;
	bool state;
private:
	void Init() override { state = 0; }
	void Next() override { state = !state; }
	void PaintOn(Board& board) const override {
		if (state) { color2.PaintOn(board); } 
		else { color1.PaintOn(board); }
	}
public:
	Blinking(Color32 color1 = COLOR_WHITE, Color32 color2 = COLOR_BLACK, uint period = 1000) 
		:color1(color1), color2(color2), Animation(period) {
		Init();
	}
};



} // namespace WndDesign