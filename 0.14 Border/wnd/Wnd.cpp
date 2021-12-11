#include "Wnd.h"
#include "BasicControls.h"
#include "../shape/BasicShapes.h"
#include "../sys/Cursor.h"

namespace WndDesign{

Wnd::BorderPos Wnd::GetBorderPos(Size regionSize, uint borderSize, Pos pos) const {
	// Suppose that min(regionSize.width, regionSize.height) > 2 * borderSize.
	if (PosInRect(pos, { { borderSize, borderSize },{ regionSize.width - 2 * borderSize, regionSize.height - 2 * borderSize } }) ||
		!PosInRect(pos, { {}, regionSize })) {
		return BorderPos::None;  // Not on border.
	}
	if (pos.x <= (int)min(regionSize.width / 2, 20)) {
		if (pos.y <= (int)min(regionSize.height / 2, 20)) { return BorderPos::LeftTop; }
		if (regionSize.height - pos.y <= (int)min(regionSize.height / 2, 20)) { return BorderPos::LeftBottom; }
	}
	if (regionSize.width - pos.x <= (int)min(regionSize.width / 2, 20)) {
		if (pos.y <= (int)min(regionSize.height / 2, 20)) { return BorderPos::RightTop; }
		if (regionSize.height - pos.y <= (int)min(regionSize.height / 2, 20)) { return BorderPos::RightBottom; }
	}
	if (PointInInterval(pos.y, regionSize.height / 2, regionSize.height / 16)) {
		if (pos.x <= (int)regionSize.width / 2) { return BorderPos::Left; }
		return BorderPos::Right;
	}
	if (PointInInterval(pos.x, regionSize.width / 2, regionSize.width / 16)) {
		if (pos.y <= (int)regionSize.height / 2) { return BorderPos::Top; }
		return BorderPos::Bottom;
	}
	return BorderPos::Other;
}
inline bool Wnd::BorderMsgHandler(Msg msg, MouseMsgPara mouse) {
	// Stores the mouse pos when clicked down. 
	// It will not cause inconsistency across different windows.	
	static Pos mouseDownPos = {};
	static BorderPos mouseDownBorderPos = BorderPos::None;

	if (msg == Msg::MOUSE_MOVEON && mouse.left()) {

	}

	BorderPos borderPos = GetBorderPos(wholeBoard.Size(), borderStyle.Thickness(), mouse.pos);
	switch (borderPos) {
		case BorderPos::None:ChangeCursor(CursorStyle::Arrow); return false;
		case BorderPos::Top:ChangeCursor(CursorStyle::Size90); break;
		case BorderPos::Bottom:ChangeCursor(CursorStyle::Size90); break;
		case BorderPos::Left:ChangeCursor(CursorStyle::Size0); break;
		case BorderPos::Right:ChangeCursor(CursorStyle::Size0); break;
		case BorderPos::LeftTop:ChangeCursor(CursorStyle::Size135); break;
		case BorderPos::RightTop:ChangeCursor(CursorStyle::Size45); break;
		case BorderPos::LeftBottom:ChangeCursor(CursorStyle::Size45); break;
		case BorderPos::RightBottom:ChangeCursor(CursorStyle::Size135); break;
		case BorderPos::Other:ChangeCursor(CursorStyle::Move); break;
		default:return false;
	}

	if (msg == Msg::MOUSE_LEFTDOWN) {
		// Store the down pos for moving or resizing.
		mouseDownPos = mouse.pos;
		mouseDownBorderPos = borderPos;
	}
	return true;
}

bool Wnd::SetBorder(Pen borderStyle) {
	// If the window has a scroll frame, then the scroll frame set the border.
	if (scrollFrame != nullptr) { return scrollFrame->SetBorder(borderStyle); }

	// Check size.
	uint borderSize = borderStyle.Thickness();
	Size regionSize = this->wholeBoard.Size();
	if (min(regionSize.width, regionSize.height) < 2 * borderSize) { return false; }

	// Paint the border as a rectangle
	this->borderStyle = borderStyle;
	Rectangle border; border.SetPenBrush(this->borderStyle); border.SetBoard(this->wholeBoard);
	border.Paint();

	// Reset the window board. Should not directly call SetBoard() here, or will be stuck in loop.
	Rect newBoardRect = { { borderSize, borderSize },{ regionSize.width - 2 * borderSize, regionSize.height - 2 * borderSize } };
	Board internalBoard(nullptr, this->wholeBoard, newBoardRect);
	Figure::SetBoard(internalBoard);
	WndSetBoard(internalBoard);
	return true;
}

bool WndDesign::Wnd::Scrollize(Size size, const Board& board, Pos initOffset) {
	if (scrollFrame != nullptr) { return false; }
	
	// If has a border, move the border to scrollframe.
	Pen frameBorder = borderStyle;
	borderStyle = Pen(COLOR_BLACK, 0); // Clear the border.
	
	// The new board is set before scrollFrame gets its address, so there won't be any loop.
	scrollFrame = new ScrollFrame(this, board, size, initOffset, frameBorder);
	Paint();

	return true;
}

void WndDesign::Wnd::SetBoard(const Board& board) {
	if (scrollFrame) { return scrollFrame->SetBoard(board); }

	this->wholeBoard = board;
	if (HasBorder() && SetBorder(this->borderStyle)) { return; }

	Figure::SetBoard(board); 
	WndSetBoard(board);
}

bool WndDesign::Wnd::MsgHandler(Msg msg, void* para) {
	// First handle the mouse message on border if the wnd has a border not on scrollframe.
	if (MSG_TYPE(msg, MOUSE) && HasBorder() && BorderMsgHandler(msg, *(MouseMsgPara*)para)) {
		return true;
	}

	// If the window has a scrollFrame, redirect the message to scrollframe.
	if (scrollFrame != nullptr) { return scrollFrame->MsgHandler(msg, para); }

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

	// May leave the scrollbar's position.

	tempboard.EndPaint();
}

void WndDesign::ScrollFrame::PaintOn(Board& board) const {
	bitmap.PaintOn(offset, board);

	// Paint scroll bar here.
}

WndDesign::ScrollFrame::ScrollFrame(Wnd* wnd, const Board& board, Size size, Pos initOffset, Pen borderStyle) 
	:bitmap(size), offset(initOffset), wnd(wnd), Wnd(borderStyle) {
	// Set the wnd's original board as its board.
	SetBoard(board);  
	// Set the new board for window. Must not directly call SetBoard here!
	Board wndboard(this, &bitmap);
	wnd->Figure::SetBoard(wndboard); wnd->WndSetBoard(wndboard);
	// Set board for scroll bar.
	right = new ScrollBar;
	bottom = new ScrollBar;

}

WndDesign::ScrollFrame::~ScrollFrame() { delete right; delete bottom; }

} // namespace WndDesign