//====================================================================
// TTF File Parser & Renderer.
// 
// By hChenqi 
// 
// day 0: 4/20/2020.
// day 1: Did prepare work and parsed some simple tables.
// day 2: Parsed glyf table.
// day 3: Loaded and parsed the bitmap file.
// day 4: Located the unicode characters and tried to do painting.
// day 5: Successfully painted a contour on the bitmap.
// 
// References:
//	- ISO/IEC 14496-22: Open Font Format
//	- 010 Editor template: TTF.bt
//	- TrueType字体文件解析和字体光栅化 (https://blog.csdn.net/yjr3426619/article/details/88597269)
//  - A Rasterizing Algorithm for Drawing Curves. (http://members.chello.at/easyfilter/bresenham.pdf)
//  
//====================================================================

#pragma once

#include "../../sys/File.h"


DECLARE_STRUCT(OffsetTable)
DECLARE_STRUCT(EntryTable)
DECLARE_STRUCT(headTable)
DECLARE_STRUCT(hheaTable)
DECLARE_STRUCT(maxpTable)
DECLARE_STRUCT(cmapTable)
DECLARE_STRUCT(nameTable)
DECLARE_STRUCT(locaTable)
DECLARE_STRUCT(hmtxTable)
DECLARE_STRUCT(glyfTable)

namespace WndDesign {

DECLARE_CLASS(Bitmap)

class TTFFile :public File {
private:
	OffsetTable* offsetTb;
	EntryTable* entries;  // An array.

	headTable* headTb;
	hheaTable* hheaTb;
	maxpTable* maxpTb;
	cmapTable* cmapTb;
	nameTable* nameTb;
	locaTable* locaTb;
	hmtxTable* hmtxTb;
	glyfTable* glyfTb;

public:
	TTFFile();
	~TTFFile() { Close(); }

	bool Open(const wchar_t* fileName);
	void Close();

public:
	bool Rasterize(wchar_t ch, Bitmap& bitmap) const;
};

} // namespace WndDesign