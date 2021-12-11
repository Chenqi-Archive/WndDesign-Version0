#include "BasicWnds.h"
#include "../sys/Cursor.h"

namespace WndDesign{

bool WndDesign::FinalWnd::AllowToPaint(const Board& board, Rect region) const {
	// Must be the background, ignore the parameters.
	if (this->board.IsAllowed()) { return true; }  // The whole window is painting.
	return this->board.BeginPaintRegion();
}

void WndDesign::FinalWnd::RefreshBoard(const Board& board, Rect region) const {
	if (this->board.IsAllowed()) { return; }  // The whole window is painting, will refresh later.
	this->board.EndPaintRegion();
}

WndDesign::FinalWnd::FinalWnd(Figure* background, Pen borderStyle, UserMsgHandler handler)
	:Wnd(borderStyle, handler) {
	if (background == nullptr) { background = new Color(COLOR_WHITE); }
	this->background = background;
	//this->background->SetBoard(Board(this, board));  // No need. Board is not set when initializing.
}

void WndDesign::FinalWnd::WndSetBoard(const Board& board) {
	background->SetBoard(Board(this, this->board));
}

void WndDesign::FinalWnd::PaintOn(Board& board) const {
	background->Paint();
}

WndDesign::FinalWnd::~FinalWnd() { delete background; }

bool WndDesign::TileWnd::IsOverlapping(Rect region) {
	for (const auto& it : subWnds) {
		if (it.region.Intersect(region).size) { return true; }
	}
	return false;
}

bool WndDesign::TileWnd::AddSubWnd(Wnd* wnd, Rect region) {
	if (IsOverlapping(region)) { return false; }
	wnd->SetBoard(Board(this, board, region));
	subWnds.push_back({ wnd, region });
	return true;
}

bool WndDesign::TileWnd::AllowToPaint(const Board& board, Rect region) const {
	if (this->board.IsAllowed()) { return true; }  // The window is painting.
	if (!region.size) { region = { {}, board.Size() }; }  // The whole board.
														  // Refuse the background's paint request if there is a subwindow.
	if (board.Size() == this->board.Size() && !subWnds.empty()) { return false; }
	// Send the subwindow's request along.
	return this->board.BeginPaintRegion(region + GetBoardRelativePos(board));
}

void WndDesign::TileWnd::RefreshBoard(const Board& board, Rect region) const {
	if (this->board.IsAllowed()) { return; }  // The window is painting, will refresh later.
	if (!region.size) { region = { {}, board.Size() }; }  // The whole board.
														  // Send the subwindow's request along.
	this->board.EndPaintRegion(region + GetBoardRelativePos(board));
}

void WndDesign::TileWnd::WndSetBoard(const Board& board) {
	Size oldsize = size;

	// If the window is not relative and the new board size doesnot 
	//   equal the original size, then add scrollbar to it, a new board
	//   will be set by the scrollbar later.
	// If already has scrollbar, then set the board for subwindows.
	if (posType == PositionType::Absolute && board.Size() != size && Scrollize(size, board)) {
		return;
	} else { size = board.Size(); }

	// Set board for background and all subwindows.
	Board subBoard = Board(this, this->board);
	background->SetBoard(subBoard);
	for (auto& item : subWnds) {
		subBoard = Board(this, this->board, item.region.Scale(oldsize, size));
		item.wnd->SetBoard(subBoard);
	}
}

void WndDesign::TileWnd::PaintOn(Board& board) const {
	background->Paint();
	for (auto& item : subWnds) { item.wnd->Paint(); }
}

WndDesign::TileWnd::TileWnd(Size size, Figure* background, PositionType posType, Pen borderStyle, UserMsgHandler handler)
	:size(size), background(background), posType(posType), Wnd(borderStyle, handler) {
	if (!this->size) { this->size = MinWndSize; }
	if (this->background == nullptr) { this->background = new Color(COLOR_WHITE); }
	//this->background->SetBoard(Board(this, board));
}

WndDesign::TileWnd::~TileWnd() {
	delete background;
	for (auto& item : subWnds) { delete item.wnd; }
}

bool WndDesign::SplitWnd::AllowToPaint(const Board& board, Rect region) const {
	if (this->board.IsAllowed()) { return true; }
	if (!region.size) { region = { {}, board.Size() }; }
	return this->board.BeginPaintRegion(region + GetBoardRelativePos(board));
}

void WndDesign::SplitWnd::RefreshBoard(const Board& board, Rect region) const {
	if (this->board.IsAllowed()) { return; }
	if (!region.size) { region = { {}, board.Size() }; }
	this->board.EndPaintRegion(region + GetBoardRelativePos(board));
}

void WndDesign::SplitWnd::WndSetBoard(const Board& board) {
	Size oldsize = size;

	// If the window is not relative and the new board size doesnot 
	//   equal the original size, then add scrollbar to it, a new board
	//   will be set by the scrollbar later.
	// If already has scrollbar, then set the board for subwindows.
	if (posType == PositionType::Absolute && board.Size() != size && Scrollize(size, board)) {
		return;
	} else { size = this->board.Size(); }

	uint radius = lineStyle.Thickness() / 2;
	if (splitDirection == SplitDirection::Vertical) {
		linePos = linePos * size.width / oldsize.width;
		if (linePos >= size.width - radius) { linePos = size.width - radius; }
	} else {
		linePos = linePos * size.height / oldsize.height;
		if (linePos >= size.height - radius) { linePos = size.height - radius; }
	}
	if (linePos < radius) { linePos = radius; }
	uint posLower = linePos - radius, posUpper = linePos + radius;

	firstRgn = { {}, size }; secondRgn = firstRgn;
	if (splitDirection == SplitDirection::Vertical) {
		firstRgn.size.width = posLower;
		secondRgn.pos.x = posUpper + 1;
		secondRgn.size.width = size.width - secondRgn.pos.x;
	} else {
		firstRgn.size.height = posLower;
		secondRgn.pos.y = posUpper + 1;
		secondRgn.size.height = size.height - secondRgn.pos.y;
	}
	this->first->SetBoard(Board(this, this->board, firstRgn));
	this->second->SetBoard(Board(this, this->board, secondRgn));
}

void WndDesign::SplitWnd::PaintOn(Board& board) const {
	// Paint the line.
	uint linePos = this->linePos;
	if (splitDirection == SplitDirection::Vertical) {
		for (uint j = 0; j < size.height; ++j) {lineStyle.Paint(board, Pos{ (int)linePos, (int)j });}
	} else {
		for (uint i = 0; i < size.width; ++i) {lineStyle.Paint(board, Pos{ (int)i, (int)linePos });}
	}
	// Paint the subwindows.
	first->Paint();
	second->Paint();
}

bool SplitWnd::WndMsgHandler(Msg msg, void* para) {
	if (MSG_TYPE(msg, MOUSE)) {
		static bool isMovingLine = false;

		MouseMsgPara mouseMsg = *(MouseMsgPara*)para;
		if (isMovingLine) {
			if (msg == Msg::MOUSE_LEFTUP) {
				// Move the split line.
				if (splitDirection == SplitDirection::Vertical) {
					SetLinePos(mouseMsg.pos.x);
				} else if (splitDirection == SplitDirection::Horizontal) {
					SetLinePos(mouseMsg.pos.y);
				}
				isMovingLine = false;
			}
			return true;
		}
	
		if (PosInRect(mouseMsg.pos, firstRgn)) { 
			ChangeCursor(CursorStyle::Arrow);
			mouseMsg.pos = mouseMsg.pos - firstRgn.pos;
			first->MsgHandler(msg, &mouseMsg); 
			return true; 
		}
		if (PosInRect(mouseMsg.pos, secondRgn)) {
			ChangeCursor(CursorStyle::Arrow);
			mouseMsg.pos = mouseMsg.pos - secondRgn.pos;
			second->MsgHandler(msg, &mouseMsg);
			return true;
		}

		// Hit on the line.
		if (splitDirection == SplitDirection::Vertical) {
			ChangeCursor(CursorStyle::Size0);
		} else if (splitDirection == SplitDirection::Horizontal) {
			ChangeCursor(CursorStyle::Size90);
		}
		if (msg == Msg::MOUSE_LEFTDOWN) { 
			isMovingLine = true; 
		}
		else { isMovingLine = false; }
		return true;
	}
	return false;
}

SplitWnd::SplitWnd(Size size, SplitDirection splitDirection, Wnd* first, Wnd* second, 
				   Pen lineStyle, PositionType posType, uint linePos, Pen borderStyle, UserMsgHandler handler) 
	:size(size), splitDirection(splitDirection), first(first), second(second), lineStyle(lineStyle), 
	posType(posType), linePos(linePos), Wnd(borderStyle, handler) {
	if (this->linePos == -1) {
		if (splitDirection == SplitDirection::Vertical) {
			this->linePos = size.width / 2;
		} else if (splitDirection == SplitDirection::Horizontal) {
			this->linePos = size.height / 2;
		}
	}
	if (!this->size) { this->size = MinWndSize; }
	if (!this->first) { this->first = new FinalWnd; }
	if (!this->second) { this->second = new FinalWnd; }
}

WndDesign::SplitWnd::~SplitWnd() { delete first; delete second; }

Wnd* WndDesign::OverlapWnd::GetSubWnd(Pos pos) {
	for (auto it = subWnds.rbegin(); it != subWnds.rend(); ++it) {
		if (PosInRect(pos, it->region)) { return it->wnd; }
	}
	return nullptr;
}

bool WndDesign::OverlapWnd::AddSubWnd(Wnd* wnd, Rect region, WndState state) {
	if (wnd == nullptr) { return false; }
	Bitmap* bitmap = new Bitmap(region.size);
	wnd->SetBoard(Board(this, bitmap));
	subWnds.push_back({ wnd, region, bitmap, state });
	return true;
}

bool WndDesign::OverlapWnd::RemoveSubWnd(Wnd* wnd) {
	return false;
}

bool WndDesign::OverlapWnd::ShowSubWnd(Wnd* wnd) {
	return false;
}

bool WndDesign::OverlapWnd::HideSubWnd(Wnd* wnd) {
	return false;
}

bool WndDesign::OverlapWnd::BringForward(Wnd* wnd, uint cnt) {
	return false;
}

bool WndDesign::OverlapWnd::SendBackward(Wnd* wnd, uint cnt) {
	return false;
}

bool WndDesign::OverlapWnd::AllowToPaint(const Board& board, Rect region) const { return true; }

void WndDesign::OverlapWnd::RefreshBoard(const Board& board, Rect region) const {
	if (!region.size) { region = { {}, board.Size() }; }  // The whole board.
														  // Find the bitmap to update.
	bool isbg = false;
	auto it = subWnds.begin();
	if (bgbitmap == GetBoardBitmap(board)) { isbg = true; } else {
		for (; it != subWnds.end(); ++it) {
			if (it->bitmap == GetBoardBitmap(board)) {
				if (it->state == WndState::Hide) { return; }
				region = region + it->region.pos;
				break;
			}
		}
		if (it == subWnds.end()) { return; }
	}
	// Create a temporary board for painting.
	Board tempboard(nullptr, this->board, region);
	if (!tempboard.BeginPaint()) { return; }
	Rect invalidRgn = region;
	if (isbg) { bgbitmap->PaintOn(invalidRgn.pos, tempboard); }
	for (; it != subWnds.end(); ++it) {
		if (it->state == WndState::Hide) { continue; }
		invalidRgn = it->region.Intersect(region);
		if (!invalidRgn.size) { continue; }
		it->bitmap->PaintOn(invalidRgn.pos - it->region.pos, tempboard, invalidRgn - region.pos);
	}
	tempboard.EndPaint();
}

void WndDesign::OverlapWnd::WndSetBoard(const Board& board) {
	bgbitmap->Resize(board.Size());
	background->SetBoard(Board(this, bgbitmap));

	// May support scrolling if the board is too small.
}

void WndDesign::OverlapWnd::PaintOn(Board& board) const {
	bgbitmap->PaintOn(board);
}

WndDesign::OverlapWnd::OverlapWnd(Figure* background, Pen borderStyle ,UserMsgHandler handler)
	:background(background), Wnd(borderStyle, handler) {
	if (this->background == nullptr) { this->background = new Color(COLOR_WHITE); }
	bgbitmap = new Bitmap;
	this->background->SetBoard(Board()); // Empty board.
}

WndDesign::OverlapWnd::~OverlapWnd() {
	delete background;
	delete bgbitmap;
	for (auto& item : subWnds) {
		delete item.wnd;
		delete item.bitmap;
	}
}

} // namespace WndDesign