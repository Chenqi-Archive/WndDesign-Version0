#pragma once

#include "Wnd.h"

#include <list>
#include <vector>

namespace WndDesign {

#define MinWndSize Size{10, 10}

enum class PositionType { Relative, Absolute };



class FinalWnd :public Wnd {

/////////////// For background /////////////// 
protected:
	Figure* background;

/////////////// For sub board managing ///////////////
private:
	bool AllowToPaint(const Board& board, Rect region = {}) const override;
	void RefreshBoard(const Board& board, Rect region = {}) const override;

/////////////// For painting and resizing ///////////////
private:
	void WndSetBoard(const Board& board) override;
	void PaintOn(Board& board) const override;

/////////////// Constructor and Destructor ///////////////
public:
	FinalWnd(Figure* background = new Color(COLOR_WHITE), 
			 Pen borderStyle = Pen(COLOR_BLACK, 0), 
			 UserMsgHandler handler = nullptr);
	~FinalWnd() override;
};



class TileWnd :public Wnd {

/////////////// For background /////////////// 
private:
	Figure* background;

/////////////// For subwindows ///////////////
private:
	struct SubWndInfo {
		Wnd* wnd;
		Rect region;
	};
	std::list<SubWndInfo>subWnds;   // The front window at the backmost of the list.
	inline bool IsOverlapping(Rect region);
public:
	bool AddSubWnd(Wnd* wnd, Rect region);

/////////////// For sub board managing ///////////////
private:
	bool AllowToPaint(const Board& board, Rect region = {}) const override;
	void RefreshBoard(const Board& board, Rect region = {}) const override;

/////////////// For painting and resizing ///////////////
private:
	Size size;
	PositionType posType;
	void WndSetBoard(const Board& board) override;
	void PaintOn(Board& board) const override;

/////////////// Constructor and Destructor ///////////////
public:
	TileWnd(Size size, 
			Figure* background = new Color(COLOR_WHITE), 
			PositionType posType = PositionType::Relative,
			Pen borderStyle = Pen(COLOR_BLACK, 0),
			UserMsgHandler handler = nullptr);
	~TileWnd() override;
};


enum class SplitDirection { Vertical, VerticalFixed, Horizontal, HorizontalFixed };

class SplitWnd :public Wnd {

/////////////// For subwindows ///////////////
private:
	Wnd* first; Rect firstRgn;
	Wnd* second; Rect secondRgn;
	SplitDirection splitDirection;
	uint linePos;
	Pen lineStyle;
private:
	inline void SetLinePos(uint linePos) {
		if (this->linePos == linePos) { return; }
		this->linePos = linePos;
		WndSetBoard(this->board);
		Paint();
	}


/////////////// For sub board managing ///////////////
private:
	bool AllowToPaint(const Board& board, Rect region = {}) const override;
	void RefreshBoard(const Board& board, Rect region = {}) const override;


/////////////// For painting and resizing ///////////////
private:
	Size size;
	PositionType posType;
	void WndSetBoard(const Board& board) override;
	void PaintOn(Board& board) const override;


/////////////// For handling messages ///////////////
private:
	bool WndMsgHandler(Msg msg, void* para) override;


/////////////// Constructor and Destructor ///////////////
public:
	SplitWnd(Size size, 
			 SplitDirection splitDirection, 
			 Wnd* first = new FinalWnd, 
			 Wnd* second = new FinalWnd, 
			 Pen lineStyle = Pen(COLOR_BLACK, 2),
			 PositionType posType = PositionType::Relative, 
			 uint linePos = -1, 
			 Pen borderStyle = Pen(COLOR_BLACK, 0),
			 UserMsgHandler handler = nullptr);
	~SplitWnd() override;
};



class OverlapWnd :public Wnd {

/////////////// For background /////////////// 
private:
	Figure* background;
	Bitmap* bgbitmap;

/////////////// For subwindows ///////////////
private:
	enum class WndState { Show, Hide };
	struct SubWndInfo {
		Wnd* wnd;
		Rect region;
		Bitmap* bitmap;
		WndState state;
	};
	std::list<SubWndInfo>subWnds;   // The front window at the backmost of the list.
private:
	Wnd* GetSubWnd(Pos pos);
public:
	bool AddSubWnd(Wnd* wnd, Rect region, WndState state = WndState::Show);
	bool RemoveSubWnd(Wnd* wnd);
	bool ShowSubWnd(Wnd* wnd);
	bool HideSubWnd(Wnd* wnd);
	// Bring a subwindow forward by cnt layers. (-1 means bring to front)
	bool BringForward(Wnd* wnd, uint cnt = 1);
	// Send a subwindow backward by cnt layers. (-1 means send to back)
	bool SendBackward(Wnd* wnd, uint cnt = 1);

/////////////// For sub board managing ///////////////
private:
	bool AllowToPaint(const Board& board, Rect region = {}) const override;
	void RefreshBoard(const Board& board, Rect region = {}) const override;

/////////////// For painting and resizing ///////////////
private:
	void WndSetBoard(const Board& board) override;
	void PaintOn(Board& board) const override;

/////////////// Constructor and Destructor ///////////////
public:
	OverlapWnd(Figure* background = new Color(COLOR_WHITE), 
			   Pen borderStyle = Pen(COLOR_BLACK, 0),
			   UserMsgHandler handler = nullptr);
	~OverlapWnd() override;
};



class GridWnd :public Wnd {

/////////////// For subwindows ///////////////
private:
	Pen gridlineStyle;
	std::vector<uint> colunmWidths;
	struct GridRow {
		uint height;
		std::vector<Wnd*> columns;
	};
	std::list<GridRow> rows;
private:
	// Get the row and column (packed in Size structure) of pos, 
	//   returns -1 if not on any grid.
	Size GetPosGrid(Pos pos);
public:
	bool InsertRow(uint row, const GridRow& items);
	bool DeleteRow(uint row);
	bool SetSubWnd(uint row, uint column, Wnd* wnd);
	
/////////////// For sub board managing ///////////////
private:
	bool AllowToPaint(const Board& board, Rect region = {}) const override;
	void RefreshBoard(const Board& board, Rect region = {}) const override;

/////////////// For painting and resizing ///////////////
private:
	Size size;
	PositionType posType;
	void WndSetBoard(const Board& board) override;
	void PaintOn(Board& board) const override {
		// First paint all the vertical lines.
	}


/////////////// Constructor and Destructor ///////////////
public:
	GridWnd() {}
};


class TabWnd :public Wnd {

};

} // namespace WndDesign