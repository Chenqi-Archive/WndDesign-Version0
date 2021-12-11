#include "BasicWnds.h"

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

WndDesign::FinalWnd::FinalWnd(Figure* background) {
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
	Size oldsize = size, newsize = oldsize;
	if (posType == PositionType::Absolute) {
		// The board size must be no larger than the original size.
		Scrollize(oldsize);
	} else { newsize = this->board.Size(); }

	// Set board for background and all subwindows.
	Board subBoard = Board(this, this->board);
	background->SetBoard(subBoard);
	for (auto& item : subWnds) {
		subBoard = Board(this, this->board, item.region.Scale(oldsize, newsize));
		item.wnd->SetBoard(subBoard);
	}

	size = newsize;
}

void WndDesign::TileWnd::PaintOn(Board& board) const {
	background->Paint();
	for (auto& item : subWnds) { item.wnd->Paint(); }
}

WndDesign::TileWnd::TileWnd(Size size, Figure* background, PositionType posType, UserMsgHandler handler)
	:size(size), background(background), posType(posType), Wnd(handler) {
	if (!this->size) { this->size = MinWndSize; }
	if (this->background == nullptr) { this->background = new Color(COLOR_WHITE); }
	//this->background->SetBoard(Board(this, board));
}

WndDesign::TileWnd::~TileWnd() {
	delete background;
	for (auto& item : subWnds) { delete item.wnd; }
}



Wnd* WndDesign::SplitWnd::GetSubWnd(Pos pos) {
	uint linePos = this->linePos;
	if (posType == PositionType::Relative) {
		if (splitDirection == VerticalSplit) { linePos = linePos * size.width / 100; } 
		else { linePos = linePos * size.height / 100; }
	}
	uint queryPos;
	if (splitDirection == VerticalSplit) { queryPos = pos.x; } else { queryPos = pos.y; }
	uint posLower = linePos - lineStyle.Thickness() / 2,
		posUpper = linePos + lineStyle.Thickness() / 2;
	if (queryPos < posLower) { return first; } else if (queryPos <= posUpper) { return nullptr; }  // Hit on the line.
	else { return second; }
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
	if (posType == PositionType::Absolute) {
		Scrollize(size);  //
	} else { size = this->board.Size(); }

	Rect firstRgn = { {}, size }, secondRgn = firstRgn;
	uint linePos = this->linePos;
	if (posType == PositionType::Relative) {
		if (splitDirection == VerticalSplit) { linePos = linePos * size.width / 100; } else { linePos = linePos * size.height / 100; }
	}
	uint posLower = linePos - lineStyle.Thickness() / 2,
		posUpper = linePos + lineStyle.Thickness() / 2;
	if (splitDirection == VerticalSplit) {
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
	// Draw the line.
	uint linePos = this->linePos;
	if (posType == PositionType::Relative) {
		if (splitDirection == VerticalSplit) { linePos = linePos * size.width / 100; } else { linePos = linePos * size.height / 100; }
	}
	if (splitDirection == VerticalSplit) {
		for (uint j = 0; j < size.height; ++j) {
			lineStyle.Paint(board, Pos{ (int)linePos, (int)j });
		}
	} else {
		for (uint i = 0; i < size.width; ++i) {
			lineStyle.Paint(board, Pos{ (int)i, (int)linePos });
		}
	}
	first->Paint();
	second->Paint();
}

WndDesign::SplitWnd::SplitWnd(Size size, SplitDirection splitDirection, uint linePos, Wnd* first, Wnd* second, PositionType posType, UserMsgHandler handler)
	:size(size), splitDirection(splitDirection), linePos(linePos),
	first(first), second(second), posType(posType), Wnd(handler) {
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

WndDesign::OverlapWnd::OverlapWnd(Figure* background, UserMsgHandler handler)
	:background(background), Wnd(handler) {
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