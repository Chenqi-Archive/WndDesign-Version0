#pragma once

#include "Board.h"

namespace WndDesign {

class Figure {
private:
	Board board;
public:
	inline virtual void SetBoard(const Board& board) { this->board = board; }
	inline Board& GetBoard() { return board; }

public:
	Figure():board(){}
	virtual ~Figure() {}

public:
	virtual void PaintOn(Board& board) const = 0;
	inline void Paint() const {
		PaintOn(const_cast<Board&>(board));
		board.Flush();
	}
};


} // namespace WndDesign