#pragma once

#include "Figure.h"
#include "Bitmap.h"
#include "Msg.h"
#include "wndtypes.h"
#include "display.h"
#include <list>

namespace FreeUI {

// Wnd: a window that display and receive messages.
class Wnd: public Figure {
public:
	// Clone an object itself.
	inline virtual Figure* Clone() const { return new Wnd(); }
	// All figures can be painted on a display area.
	virtual void PaintOn(DisplayArea& displayArea) const;
	// Destructor.
	virtual ~Wnd();

private:
	DisplayArea displayArea;

public:
	virtual void MsgHandler(Msg msg, void* para);
};

// A FinalWnd is a wnd that has no subwindow. Like buttons.
class FinalWnd:public Wnd {

};

// A SplitWnd is a wnd that can be split into halves, each can also 
//   be split into halves or just a subwindow, forming a split tree.
// One can drag the split line to resize.
class SplitWnd:public Wnd {
	union Node {
		struct {
			Node* parent;
			Node* first, * second;
			enum { Vertical, Horizontal, Final }splitStyle;  // Final means it is a Wnd.
			uint lineSize;  // The linesize is 2 by default.
			Line* line;  // Optional
		};
		Wnd* subWnd;
	};
private:
	// The node must be a wnd, after splitting, the wnd will be the first wnd, the second
	//   wnd will be a null wnd.
	// This operation sends a resize message to the window.
	void SplitNode(Node* node, uint lineSize = 2, Line* line = Line()) {

	}

	// When a node destroys, the other node becomes the parent node.
	void DestroyNode(Node* node) {

	}
};

// A GridWnd is the window whose subwindows are ordered as a grid.
class GridWnd:public Wnd {
	GridLine* gridLine;

};

// A FlowWnd's subwindows are placed one after another.
class FlowWnd:public Wnd {

};

// An OverlapWnd is a wnd that has subwindows in free position, 
//   which can move and overlap.
class OverlapWnd:public Wnd {
	struct SubWndInfo {
		Wnd* subWnd;
		Pos pos;
		Size size;
	};
private:
	// The subwindow list, the foremost window at the end of the list.
	std::list<SubWndInfo> subWnds;



};

}

