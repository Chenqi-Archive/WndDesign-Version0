#pragma once

#include "Wnd.h"
#include "../shape/Paint.h"
#include <list>

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
	FinalWnd(Figure* background = new Color(COLOR_WHITE));
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
	TileWnd(Size size, Figure* background = new Color(COLOR_WHITE), 
			PositionType posType = PositionType::Relative, UserMsgHandler handler = nullptr);
	~TileWnd() override;
};



class SplitWnd :public Wnd {

/////////////// For subwindows ///////////////
private:
	Wnd* first;
	Wnd* second;
	enum SplitDirection { VerticalSplit, HorizontalSplit } splitDirection;
	uint linePos;
	Pen lineStyle;
private:
	Wnd* GetSubWnd(Pos pos);

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
	SplitWnd(Size size, SplitDirection splitDirection, uint linePos = 50, 
			 Wnd* first = new FinalWnd, Wnd* second = new FinalWnd, 
			 PositionType posType = PositionType::Relative, UserMsgHandler handler = nullptr);
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
	OverlapWnd(Figure* background = new Color(COLOR_WHITE), UserMsgHandler handler = nullptr);
	~OverlapWnd() override;
};

} // namespace WndDesign