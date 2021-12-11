#pragma once

#include "Wnd.h"
#include <list>

namespace WndDesign {

class FinalWnd :public Wnd {
private:
	Figure* figure;
private:
	void WndSetBoard(const Board& board) override {

	}
	void WndPaintOn(Board& board) const override {

	}

public:
	FinalWnd(Figure* figure,
			 Pen border = Pen(COLOR_BLACK, 1),
			 bool resizable = true,
			 UserMsgHandler handler = nullptr)
		:figure(figure),Wnd(border, resizable, handler){
		this->figure->SetBoard(board);
	}
	~FinalWnd() { delete figure; }
};


class TileWnd :public Wnd {

public:
	bool AddWnd(Wnd* wnd, Rect region);

};


class SplitWnd :public Wnd {

};


} // namespace WndDesign