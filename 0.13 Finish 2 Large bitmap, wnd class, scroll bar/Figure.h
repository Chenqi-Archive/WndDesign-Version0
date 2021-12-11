#pragma once

#include "Board.h"

namespace WndDesign {

class Figure {
protected:
	Board board;      // The board to paint on.
public:
	inline virtual void SetBoard(const Board& board) { this->board = board; }

public:
	Figure():board() {}
	Figure(Board board):board(board) {}
	inline void Paint() const {
		Board& board = const_cast<Board&>(this->board);
		if (!board.BeginPaint()) { return; }
		PaintOn(board);
		board.EndPaint();
	}

public:
	// You should override this function to do painting work.
	virtual void PaintOn(Board& board)const = 0;
};

// The figure that does nothing.
class NullFigure :public Figure {
private:
	void PaintOn(Board& board) const override{}
};


} // namespace WndDesign