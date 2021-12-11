#pragma once

#include "wndtypes.h"
#include "../basic/Figure.h"
#include "../shape/Paint.h"
#include "../shape/BasicShapes.h"

namespace WndDesign {

DECLARE_CLASS(TitleBar)
class Wnd :public Figure {

/////////////// For borders ///////////////
protected:
	Board board;  // Overrides the Figure's board for painting.
private:
	Pen border;
	bool resizable;
private:
	enum class BorderPos { None, Top, Bottom, Left, Right, LeftTop, RightTop, LeftBottom, RightBottom, Other };
	BorderPos GetBorderPos(Pos pos) const {

	}
	inline void SetInnerBoard() {
		const Board& board = Figure::GetBoard();
		Rect innerRgn; innerRgn.pos = { border.thickness, border.thickness };
		Size wholeSize = board.GetSize();
		if (min(wholeSize.width, wholeSize.height) < 2 * border.thickness) {
			innerRgn.size = {};
		} else {
			innerRgn.size = { wholeSize.width - border.thickness, wholeSize.height - border.thickness };
		}
		this->board = Board(board, innerRgn);
		WndSetBoard(this->board);
	}
public:
	inline void SetBorder(Pen border) {
		if (border.thickness == this->border.thickness) { this->border = border; return; }
		this->border = border;
		SetInnerBoard();
	}

/////////////// For title bar ///////////////

//private:
//	TitleBar* titlebar;
//	uint titlebarHeight;


/////////////// For painting ///////////////
private:
	virtual void WndSetBoard(const Board& board) {}
	virtual void WndPaintOn(Board& board) const {}
public:
	inline void SetBoard(const Board& board) override final {
		Figure::SetBoard(board);
		SetInnerBoard();
	}
	inline void PaintOn(Board& board) const override final {
		if (border.thickness > 0) {
			Rectangle rectangle; rectangle.pen = border;
			rectangle.PaintOn(board);
		}
		WndPaintOn(const_cast<Board&>(this->board));
	}

/////////////// For board managing ///////////////
public:
	virtual void FlushBitmap(const WndBitmap* bitmap, Rect region) {};


/////////////// For handling messages ///////////////
private:
	UserMsgHandler handler;
	virtual bool WndMsgHandler(Msg msg, void* para) { return false; }
public:
	bool MsgHandler(Msg msg, void* para) {
		if (WndMsgHandler(msg, para)) { return true; }
		if (handler) { return handler(this, msg, para); }
		return false;
	}


/////////////// Constructor and Destructor ///////////////
public:
	Wnd(Pen border = Pen(COLOR_BLACK, 1),
		bool resizable = true,
		UserMsgHandler handler = nullptr)
		:border(border), resizable(resizable), handler(handler){
		SendMsg(this, Msg::WND_CREATE, nullptr);
	}
	virtual ~Wnd() {
		SendMsg(this, Msg::WND_DESTROY, nullptr);
	}
};


bool SendMsg(Wnd* wnd, Msg msg, void* para) {
	wnd->MsgHandler(msg, para);
}


DECLARE_CLASS(ScrollBar)
class ScrollWnd :public Wnd {
private:
	ScrollBar* vertical, * horizontal;

private:

};

} // namespace WndDesign