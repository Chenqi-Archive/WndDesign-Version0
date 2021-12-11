#pragma once

#include "../Figure.h"
#include "../shape/Bitmap.h"
#include "wndtypes.h"

namespace WndDesign {

enum class ScrollType { None, Vertical, Horizontal, Both };

class ScrollBar;

class Wnd :public Figure {

	////////// For scrollable windows.//////////
private:
	ScrollType scrollType;
	Bitmap bitmap;          // The whole bitmap.
	Pos offset;				// The displaying offset
	ScrollBar* verticalScrollBar;
	ScrollBar* horizontalScrollBar;
protected:
	Board board;
	////////////////////////////////////////////

protected:
	MsgHandler handler;  // The user's msg handler.
private:
	friend class Board;
	virtual bool AllowToPaint(const Bitmap* bitmap, Rect region) const = 0;
	virtual void RefreshBoard(const Bitmap* bitmap, Rect region) const = 0;

public:
	Wnd(Size realSize = {}, MsgHandler handler = nullptr) {

	}
	virtual bool MsgHandler(Msg msg, void* para) = 0;
	Size GetSize() { return bitmap.GetSize(); } // RealSize
	void PaintOn(Board& board) const override {
		if (scrollType != ScrollType::None) {
			Size size = board.Size();
			// Should minus the size of scroll bar.

			for (uint j = 0; j < size.height; ++j) {
				for (uint i = 0; i < size.width; ++i) {
					board.Set(Pos{ i, j }, bitmap.Get(offset + Pos{ i,j }));
				}
			}
		}
	}
	void SetBoard(const Board& board) override {
		if (board.Size != GetSize()) {
			// Need to add scrollBar.

		}
	}
};


} // n