#include "Wnd.h"
#include "BasicControls.h"

namespace WndDesign{

void WndDesign::Wnd::Scrollize(Size size, Pos initOffset) {
	if (scrollFrame != nullptr) { return; }
	scrollFrame = new ScrollFrame(this, board, size, initOffset);
	Paint();
}

void WndDesign::Wnd::SetBoard(const Board& board) {
	if (scrollFrame) { return scrollFrame->SetBoard(board); }
	this->board = board;
	WndSetBoard(board);
}

bool WndDesign::Wnd::MsgHandler(Msg msg, void* para) {
	// If the window has a scrollFrame, redirect the message to scrollframe.
	if (scrollFrame != nullptr) { return scrollFrame->WndMsgHandler(msg, para); }
	// Handle the message inside Wnd class.
	if (WndMsgHandler(msg, para)) { return true; }
	// If not handled, call the user's handler.
	if (handler != nullptr) { return handler(this, msg, para); }
	return false;
}

WndDesign::Wnd::~Wnd() { if (scrollFrame) { delete scrollFrame; } }

void WndDesign::ScrollFrame::RefreshBoard(const Board& board, Rect region) const {
	// Board must be the whole bitmap.
	if (!region.size) { region.size = board.Size(); }
	Rect invalidRgn = region.Intersect({ offset, this->board.Size() });
	if (!invalidRgn.size) { return; }
	// Create a temporary board for painting.
	Board tempboard(nullptr, this->board, invalidRgn - offset);
	if (!tempboard.BeginPaint()) { return; }
	bitmap.PaintOn(invalidRgn.pos, tempboard);

	// May leave the scrollbar's position 

	tempboard.EndPaint();
}

void WndDesign::ScrollFrame::PaintOn(Board& board) const {
	bitmap.PaintOn(offset, board);

	// Paint scroll bar here.
}

WndDesign::ScrollFrame::ScrollFrame(Wnd* wnd, const Board& board, Size size, Pos initOffset) :bitmap(size), offset(initOffset), wnd(wnd) {
	SetBoard(board);  // Set the wnd's original board as its board.
	wnd->SetBoard(Board(this, &bitmap));
	right = new ScrollBar;
	bottom = new ScrollBar;
}

WndDesign::ScrollFrame::~ScrollFrame() { delete right; delete bottom; }

} // namespace WndDesign