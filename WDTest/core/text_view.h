#pragma once

#include "document.h"
#include <deque>

class TextView {
	// Document
private:
	TextDoc doc;
public:
	void NewFile();
	void OpenFile();
	void SaveFile();

	// Cursor
private:
	uint cursor_row;
	uint cursor_col;
public:
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	// Selection
private:
	uint begin_row;   // -1 means no selection
	uint begin_col;
	uint end_row;
	uint end_col;     // Including the end column.
public:
	void SelectText(uint startLineNo, uint startLinePos,
					uint endLineNo, uint endLinePos);
	void UnselectText();
	void Delete();

	// Clipboard
public:
	// Copy the selected region.
	void Copy();
	// Cut the selected region.
	void Cut();
	// Paste at the cursor pos, or replace the selected region.
	void Paste();

	// Undo and redo
private:
	deque<int> operations;
	uint nextOp;
public:
	void Undo();
	void Redo();


public:
	TextView();
	~TextView();
};