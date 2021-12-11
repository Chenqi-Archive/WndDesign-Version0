#pragma once

#include "Wnd.h"
#include "../shape/Paint.h"

#include <list>

namespace WndDesign {

// FinalWnd has no subwindow but only a background Figure.
class FinalWnd:public Wnd {
protected:
	Figure* background;
private:
	bool AllowToPaint(const Bitmap* bitmap, Rect region) const override { return true; }  // No use.
	void RefreshBoard(const Bitmap* bitmap, Rect region) const override {}  // No use.
public:
	FinalWnd(Figure* background = new Color(COLOR_WHITE)) {
		if (background == nullptr) { background = new Color(COLOR_WHITE); }
		this->background = background;
		this->background->SetBoard(board); 
		this->background->Paint();
	}
	~FinalWnd() { delete background; }
	void SetBackground(Figure* background) {
		if (background == nullptr) { return; }
		delete this->background;
		this->background = background;
		this->background->SetBoard(board);
		this->background->Paint();
	}
	void SetBoard(const Board& board) override {
		this->board = board;
		background->SetBoard(this->board);
		background->Paint();
	}
	bool MsgHandler(Msg msg, void* para) override{ 
		return handler(this, msg, para);
	}

	void AddTemporatryWnd(Wnd* wnd){}
};


enum class PosType { Absolute, Relative };

// The subwindows of TileWnd cannot overlap, and the background cannot repaint if there is a subwindow.
class TileWnd:public Wnd {
	Figure* background;

	struct SubWndInfo {
		Wnd* wnd;
		Rect region;
		WndState state;
	};
	std::list<SubWndInfo>subWnds;   // The front window at the backmost of the list.
	PosType postype;

private:
	inline bool IsOverlapping(Rect region) {
		for (const auto& it : subWnds) {
			if (it.region.Intersect(region).size) {return true; }
		}
		return false;
	}

private:
	bool AllowToPaint(const Bitmap* bitmap, Rect region) const override { 
		// Only receives background requests.
		if (!subWnds.empty()) { return false; }
		Board& board = const_cast<Board&>(this->board);
		return board.BeginPaint();
	}
	void RefreshBoard(const Bitmap* bitmap, Rect region) const override {
		// Only receives background requests.
		Board& board = const_cast<Board&>(this->board);
		board.EndPaint();
	}
public:
	TileWnd(Figure* background = new Color(COLOR_WHITE), PosType postype = PosType::Relative) :postype(postype){
		if (background == nullptr) { background = new Color(COLOR_WHITE); }
		this->background = background;
		this->background->SetBoard(Board(this, board));
	}

	bool AddSubWnd(Wnd* wnd, Rect region, bool hasBorder, bool ) {
		if (IsOverlapping(region)) { return false; }
		wnd->SetBoard(Board(nullptr, board, region));
		subWnds.push_back({ wnd, region , WndState::Show });
		return true;
	}

	void SetBoard(const Board& board) override {
		Size oldsize = this->board.Size(); Size newsize = oldsize;
		if (postype == PosType::Absolute) { Wnd::SetBoard(board);}  // Scroll
		else { newsize = board.Size(); } // Scale
		Wnd::SetBoard(board);
		this->board = board;
		bool isAllowed = this->board.BeginPaint();
		background->SetBoard(Board(this, this->board));
		if (isAllowed) { background->PaintOn(this->board); }
		for (auto& item : subWnds) {
			Board subBoard = Board(nullptr, board, item.region.Scale(oldsize, newsize));
			item.wnd->SetBoard(subBoard);
			if (isAllowed) { item.wnd->PaintOn(subBoard); }
		}
		this->board.EndPaint();
	}

};

// The subwindows of FlowWnd cannot overlap are one by one arranged.
class FlowWnd:public Wnd {
private:
enum class FlowDirection{TL2TR, TL2DL, TR2TL, };
	struct SubWndInfo {
		Wnd* wnd;
		Rect region;
		WndState state;
	};
	std::list<SubWndInfo>subWnds;   // The front window at the backmost of the list.

	uint horizontalSpace;  // The minimun space between subwnd neighbors.
	uint verticalSpace;

private:
	bool AllowToPaint(const Bitmap* bitmap, Rect region) const override { return true; }
	void RefreshBoard(const Bitmap* bitmap, Rect region) const override {}

public:
	FlowWnd() {

	}

	bool AddSubWnd(Wnd* wnd, Rect region) {

	}
	void SetBoard(const Board& board) override {
	}
};

// 
class GridWnd:public Wnd {
	

private:
	bool AllowToPaint(const Bitmap* bitmap, Rect region) const override { return true; }
	void RefreshBoard(const Bitmap* bitmap, Rect region) const override {}

public:
	bool SetWnd(Wnd* wnd, uint row, uint column) {

	}
	bool SetWnd(Wnd* wnd, Pos pos) {

	}
	bool AddRow(uint row, uint height) {

	}
};


enum class SplitDirection { None, Vertical, Horizontal };  // None means it is a Wnd.


class SplitWnd:public Wnd {
	struct Node {
		Node* first = nullptr, * second = nullptr;
		Node* parent = nullptr;
		SplitDirection direction = SplitDirection::None;  // None means it is a Wnd.
		PosType type = PosType::Relative;
		uint linePos = 50;  // center pos of the line.
		Pen pen = Pen();  // The split line style.
		Rect region;
		Wnd* wnd = nullptr;
	};
	Node* root;

private:
	Node* GetNode(Pos pos) {  // The real pos.
		Node* node = root;
		while (1) {
			const Rect& region = node->region;
			uint linePos = node->linePos;
			Pos curPos = pos - region.pos;
			uint queryPos;
			if (node->type == PosType::Relative) {
				if (node->direction == SplitDirection::None) { return node; }  // A wnd.
				else if (node->direction == SplitDirection::Vertical) { linePos = linePos * region.size.width / 100; queryPos = curPos.x; }
				else { linePos = linePos * region.size.height / 100; queryPos = curPos.y; }
			}
			uint posLower = linePos - node->pen.Thickness() / 2, posUpper = linePos + node->pen.Thickness() / 2;
			if (queryPos < posLower) {node = node->first;}
			else if (queryPos <= posUpper) { return node; }  // Hit on the line.
			else { node = node->second; }
		}
	}

public:
	SplitWnd() {
		root = new Node;
		root->wnd = new FinalWnd();
	}
	~SplitWnd() {

	}
	bool SetWnd(Wnd* wnd, Pos pos) {
		Node* node = GetNode(pos);
		if (node->direction != SplitDirection::None) { return false; }  // Hit on the line.
		if (wnd != nullptr) {
			delete node->wnd; node->wnd = wnd;
			node->wnd->SetBoard(Board(nullptr, board, node->region));
			return true;
		} else { return false; }
	}
	bool SplitNode(Wnd* wnd, Pos pos, SplitDirection direction, bool is_first = false) {
		Node* node = GetNode(pos);
		if (node->direction != SplitDirection::None) { return false; }  // Hit on the line.


	}

	void SetBoard(const Board& board) override {

	}
};


enum class WndState { Show, Hide };

// Each subwindow of OverlapWnd has a distinct bitmap for displaying.
class OverlapWnd:public Wnd {
private:
	Figure* background;

	struct SubWndInfo {
		Wnd* wnd;
		Rect region;
		Bitmap* bitmap;
		WndState state;
	};
	std::list<SubWndInfo>subWnds;   // The front window at the backmost of the list.

private:
	Wnd* GetSubWnd(Pos pos) {
		for (auto it = subWnds.rbegin(); it != subWnds.rend(); ++it) {
			if (PosInRect(pos, it->region)) { return it->wnd; }
		}
		return nullptr;
	}
private:
	bool AllowToPaint(const Bitmap* bitmap, Rect region) const override {
		auto it = subWnds.begin();
		for (; it != subWnds.end(); ++it) {
			if (it->bitmap == bitmap) {
				if (it->state == WndState::Hide) { return false; }
				region = region + it->region.pos;
				break;
			}
		}
		for (++it; it != subWnds.end(); ++it) {
			if (RectInRect(region, it->region)) { return false; }
		}
		return true;
	}
	void RefreshBoard(const Bitmap* bitmap, Rect region) const override {
		auto it = subWnds.begin();
		for (; it != subWnds.end(); ++it) {
			if (it->bitmap == bitmap) {
				region = region + it->region.pos;
				break;
			}
		}
		if (it == subWnds.end()) { return; }
		Board board(nullptr, this->board, region);
		if (!board.BeginPaint()) { return; }
		for (; it != subWnds.end(); ++it) {
			Rect invalidRgn = region.Intersect(it->region);
			if (invalidRgn.size) {
				Pos setOffset = invalidRgn.pos - region.pos;
				Pos getOffset = invalidRgn.pos - it->region.pos;
				for (uint j = 0; j < invalidRgn.size.height; ++j) {
					for (uint i = 0; i < invalidRgn.size.width; ++i) {
						board.Set(setOffset + Pos{i,j}, it->bitmap->Get(getOffset + Pos{i,j}));
					}
				}
			}
		}
		board.EndPaint();
	}
private:
	bool RemoveSubWnd(Wnd* wnd) {
		Rect region = {};
		auto it = subWnds.begin();
		for (; it != subWnds.end(); ++it) {
			if (it->wnd == wnd) {
				region = it->region;
				subWnds.erase(it);
				wnd->SetBoard(Board());
			}
		}
		if (it == subWnds.end()) { return; }
		Board board(nullptr, this->board, region);
		if (!board.BeginPaint()) { return; }
		for (it = subWnds.begin(); it != subWnds.end(); ++it) {
			Rect invalidRgn = region.Intersect(it->region);
			if (invalidRgn.size) {
				Pos setOffset = invalidRgn.pos - region.pos;
				Pos getOffset = invalidRgn.pos - it->region.pos;
				for (uint j = 0; j < invalidRgn.size.height; ++j) {
					for (uint i = 0; i < invalidRgn.size.width; ++i) {
						board.Set(setOffset + Pos{i,j}, it->bitmap->Get(getOffset + Pos{i,j}));
					}
				}
			}
		}
		board.EndPaint();
	}
public:
	OverlapWnd() {}
	//void SetBoard(const Board& board) override { this->board = board; } // No need.
	bool AddSubWnd(Wnd* wnd, Rect region, WndState state = WndState::Show) {
		SubWndInfo subwnd = {wnd, region, new Bitmap(region.size)};
		subwnd.wnd->SetBoard(Board(this, subwnd.bitmap, region));
		subWnds.push_back(subwnd);
	}
	bool DestroySubWnd(Wnd* wnd) {
		RemoveSubWnd(wnd);
		delete wnd;
	}
};



} // namespace WndDesign