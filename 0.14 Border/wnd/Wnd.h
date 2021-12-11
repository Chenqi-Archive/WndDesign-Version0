#pragma once

#include "../Figure.h"
#include "../shape/Bitmap.h"
#include "../shape/Paint.h"
#include "wndtypes.h"

namespace WndDesign {

class ScrollFrame;
class Wnd :public Figure {

/////////////// For moving and resizing ///////////////




/////////////// For borders ///////////////
private:
	Pen borderStyle;  // size and color.
	Board wholeBoard;  // Store previous board in case the border changes.
private:
	enum class BorderPos{None, Top, Bottom, Left, Right, LeftTop, RightTop, LeftBottom, RightBottom, Other};
	BorderPos GetBorderPos(Size regionSize, uint borderSize, Pos pos) const;
private:
	// Handle border messages for moving and resizing.
	bool BorderMsgHandler(Msg msg, MouseMsgPara mouse);
public:
	inline bool HasBorder()const { return borderStyle.Thickness() > 0; }
	// The border is inside the wnd region, so if you add a border, the wnd size will shrink by 2 * borderSize.
	bool SetBorder(Pen borderStyle = Pen());


/////////////// For scrollable windows ///////////////
private:
	friend class ScrollFrame;
	ScrollFrame* scrollFrame;
protected:
	// Create a scrollFrame as its parent to take over the painting work.
	bool Scrollize(Size size, const Board& board, Pos initOffset = {});


/////////////// For sub board managing ///////////////
private:
	friend class Board;
	virtual bool AllowToPaint(const Board& board, Rect region = {}) const = 0;
	virtual void RefreshBoard(const Board& board, Rect region = {}) const = 0;
protected:
	static inline const Pos& GetBoardRelativePos(const Board& board) { return board.relativeRegion.pos; }
	static inline const Bitmap* GetBoardBitmap(const Board& board) { return board.bitmap; }


/////////////// For painting ///////////////
public: 
	void PaintOn(Board& board) const override {}
	void SetBoard(const Board& board) override final;
private:
	// You should override this function to set board for subwindows.
	virtual void WndSetBoard(const Board& board) {}


/////////////// For handling messages ///////////////
private: 
	UserMsgHandler handler;  // The user defined handler.
	// You should override this function to handle basic messages.
	virtual bool WndMsgHandler(Msg msg, void* para) { return false; }
public:
	// Returns true if a message is handled, false if not.
	bool MsgHandler(Msg msg, void* para);


/////////////// Constructor and Destructor ///////////////
public:
	Wnd(Pen borderStyle = Pen(COLOR_BLACK, 0), UserMsgHandler handler = nullptr)
		:handler(handler), scrollFrame(nullptr), borderStyle(borderStyle), wholeBoard(){}
	virtual ~Wnd();
};



// Scrollable windows are subwindows of ScrollFrame.
class ScrollBar;
class ScrollFrame :public Wnd {

/////////////// For subwindow and board managing ///////////////
private:
	Bitmap bitmap;  // The large bitmap for the wnd to paint.
	Pos offset;
	Wnd* wnd;
	ScrollBar* right, * bottom;
private:
	bool AllowToPaint(const Board& board, Rect region = {}) const override { 
		if (!region.size) {
			// Not allowed if the whole window requests repaint,
			// this is redirected to the scrollframe's painting job.
			Paint();
			return false; 
		}
		return true; 
	}
	void RefreshBoard(const Board& board, Rect region = {}) const override;
	void WndSetBoard(const Board& board) override {
		// Set board for scroll bars.


	}
	void PaintOn(Board& board) const override;


/////////////// For handling messages ///////////////
private:
	friend class Wnd;
	bool WndMsgHandler(Msg msg, void* para) override {
		return false;
	}


/////////////// Constructor and Destructor ///////////////
public:
	ScrollFrame(Wnd* wnd, const Board& board, Size size, Pos initOffset, Pen borderStyle);
	~ScrollFrame();
};


} // namespace WndDesign