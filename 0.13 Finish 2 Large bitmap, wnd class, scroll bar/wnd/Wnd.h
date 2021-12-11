#pragma once

#include "../Figure.h"
#include "../shape/Bitmap.h"
#include "wndtypes.h"

namespace WndDesign {

class ScrollFrame;
class Wnd :public Figure {

/////////////// For scrollable windows ///////////////
private:
	ScrollFrame* scrollFrame;
protected:
	// Create a scrollFrame as its parent to take over the painting work.
	void Scrollize(Size size, Pos initOffset = {});

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
	// You should override this function to refresh subwindows' boards.
	void SetBoard(const Board& board) override final;
private:
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
	Wnd(UserMsgHandler handler = nullptr) :handler(handler), scrollFrame(nullptr){}
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
	void WndSetBoard(const Board& board) override {} // Do nothing.
	void PaintOn(Board& board) const override;


/////////////// For handling messages ///////////////
private:
	friend class Wnd;
	bool WndMsgHandler(Msg msg, void* para) override {
		return false;
	}


/////////////// Constructor and Destructor ///////////////
public:
	ScrollFrame(Wnd* wnd, const Board& board, Size size, Pos initOffset);
	~ScrollFrame();
};


} // namespace WndDesign