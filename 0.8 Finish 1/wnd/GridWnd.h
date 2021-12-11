#pragma once

#include "../shape/BasicShapes.h"
#include "../lib/list.h"
#include "../Wnd.h"

class GridWnd :public Shape{
private:  // Wnd info
	//Bitmap display;

private:
	// Columns:
	const uint colCnt;
	uint* colSize;
	uint colSizeSum;

	// Rows:
	struct Row{
		uint colCnt;
		Wnd** wnd;
		uint height;  // Absolute height in pixel.
		Row(uint height, uint colCnt) :height(height), colCnt(colCnt){
			wnd = new Wnd * [colCnt]();
		}
		Row(const Row& row) :height(row.height), colCnt(row.colCnt) {
			wnd = new Wnd * [colCnt]();
			for (int i = 0; i < colCnt; ++i) {
				wnd[i] = row.wnd[i];
			}
		}
		~Row() { delete[]wnd; }
	};

	list<Row> rows;

	// Gridline Style: (display or not, size, color, etc.)

	// Gridlines are 1 pixel thick by default.  
	//   Draw left and top lines for each grid, right and bottom lines for right and bottom girds.


	// Subwindows are independent of gridlines.

	// Or gridlines could be different for each grid. (Styles are stored together somewhere different)

public:
	GridWnd() :colCnt(4) {
		colSize = new uint[colCnt]{50, 20, 30, 100};
		colSizeSum = 200;

		rows.push_back({ 20, colCnt });
		rows.push_back({ 8, colCnt });
		rows.push_back({ 14, colCnt });
	}

	void Display(Bitmap& display, Pos offset) const{
		// Draw grid lines
		// switch style

		uint rowNum = 0, colNum;
		Pos gridPos = { 0,0 }; // Current gird pos. Start from {0, 0}.
		for (auto it = rows.begin(); it; ++it, ++rowNum) {
			const Row& row = it.Item();
			gridPos.x = 0;
			for (colNum = 0; colNum < colCnt; ++colNum) {
				// Draw girdline
				class Rectangle rect;
				rect.PaintOn(display, { gridPos + offset, {colSize[colNum], row.height} });

				// Draw subWnd.
				Wnd* wnd = row.wnd[colNum];
				if (wnd != nullptr) {
					wnd->GetDisplay().PaintOn(display, { gridPos + Pos{1,1}, {colSize[colNum], row.height - 2} });
				}

				gridPos.x += colSize[colNum];
			}

			gridPos.y += row.height;
		}
	}

	Wnd* GetSubWnd(Pos pos) {
		uint rowNum = 0, colNum;
		Size size = {}; // The grid size.
		// Find row.
		auto it = rows.begin();
		for (; it; ++it, ++rowNum) {
			const Row& row = it.Item();
			if (pos.y >= row.height) { pos.y -= row.height; }
			else { size.height = row.height; break; }
		}
		if (!it) { return nullptr; }

		// Find column.
		for (colNum = 0; colNum < colCnt; ++colNum) {
			if (pos.x >= colSize[colNum]) { pos.x -= colSize[colNum]; }
			else { size.width = colSize[colNum]; break; }
		}
		if (colNum >= colCnt) { return nullptr; }

		// Check if on girdline
		if (pos.x == 0 || pos.x == size.width || pos.y == 0 || pos.y == size.height) {
			return nullptr;
		}

		const Row& row = it.Item();
		return row.wnd[colNum];
	}

	// For cloning a derived object.
	virtual Shape* Clone(void* para) const { return new GridWnd(); }


	// Ignore the pen and size. Only draw on
	virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) const {
		// For Wnd class, display to parent wnd.
		Display(bitmap, {100, 100});
	}


	// Deconstructor is needed!
	virtual ~GridWnd() { delete[]colSize; }
};