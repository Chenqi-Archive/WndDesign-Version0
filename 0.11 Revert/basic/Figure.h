#pragma once

#include "display.h"

namespace WndDesign {

class Figure {
protected:
	Board board;
public:
	Figure() {}
	inline void SetBoard(const Board& board) { this->board = board; }
	inline const Board& GetBoard() const { return board; }
	virtual Figure* Clone() const = 0;
	virtual void Paint() = 0;  
	virtual ~Figure(){}
};

} // namespace WndDesign