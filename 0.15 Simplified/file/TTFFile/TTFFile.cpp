#include "TTFFile.h"

#include "OffsetTable.h"
#include "headTable.h"
#include "hheaTable.h"
#include "maxpTable.h"
#include "cmapTable.h"
#include "nameTable.h"
#include "locaTable.h"
#include "hmtxTable.h"
#include "glyfTable.h"

#include "../../shape/Bitmap.h"

#include <map>
#include <cmath>

#ifdef _DEBUG
#include <cstdio>
#endif // _DEBUG


WndDesign::TTFFile::TTFFile() {
	offsetTb = nullptr;
	entries = nullptr;
	headTb = nullptr;
	hheaTb = nullptr;
	maxpTb = nullptr;
	cmapTb = nullptr;
	nameTb = nullptr;
	locaTb = nullptr;
	hmtxTb = nullptr;
	glyfTb = nullptr;
}


bool WndDesign::TTFFile::Open(const wchar_t* fileName) {
	if (!File::Open(fileName)) { return false; }

	// Save the data and close the file.
	offsetTb = new OffsetTable();
	headTb = new headTable();
	hheaTb = new hheaTable();
	maxpTb = new maxpTable();
	cmapTb = new cmapTable();
	nameTb = new nameTable();
	locaTb = new locaTable();
	hmtxTb = new hmtxTable();
	glyfTb = new glyfTable();

	//if (Size().HighPart != 0) { break; }
	//uint size = Size().LowPart;  No size checking!

	uint offset = 0;
	const char* const data = (char*)PFile();

	offsetTb->read(data, offset);
	offset += sizeof(OffsetTable);

	std::map<ULONG, uint> map;

	entries = new EntryTable[offsetTb->numTables]();
	for (uint i = 0; i < offsetTb->numTables; ++i) {
		entries[i].read(data, offset);
		map[entries[i].Tag.asLong] = entries[i].offset;
		offset += sizeof(EntryTable);
	}

	headTb->read(data, map.find(*(ULONG*)"head")->second);
	hheaTb->read(data, map.find(*(ULONG*)"hhea")->second);
	maxpTb->read(data, map.find(*(ULONG*)"maxp")->second);
	cmapTb->read(data, map.find(*(ULONG*)"cmap")->second);
	nameTb->read(data, map.find(*(ULONG*)"name")->second);
	locaTb->read(data, map.find(*(ULONG*)"loca")->second, maxpTb->numGlyphs, headTb->indexToLocFormat);
	hmtxTb->read(data, map.find(*(ULONG*)"hmtx")->second, hheaTb->numberOfHMetrics, maxpTb->numGlyphs);
	glyfTb->read(data, map.find(*(ULONG*)"glyf")->second, locaTb);


	File::Close();

	return true;
}

void WndDesign::TTFFile::Close() {
	delete offsetTb; offsetTb = nullptr;
	delete[] entries; entries = nullptr;
	delete headTb; headTb = nullptr;
	delete hheaTb; hheaTb = nullptr;
	delete maxpTb; maxpTb = nullptr;
	delete cmapTb; cmapTb = nullptr;
	delete nameTb; nameTb = nullptr;
	delete locaTb; locaTb = nullptr;
	delete hmtxTb; hmtxTb = nullptr;
	delete glyfTb; glyfTb = nullptr;
	File::Close();
}


namespace WndDesign {
static inline void DrawBezier2(Bitmap& bitmap, Pos pos1, Pos pos2) {  // Line
	// Using Bresenham algorithm.
	int x0 = pos1.x, x1 = pos2.x, y0 = pos1.y, y1 = pos2.y;
	int dx = abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;  /* error value e_xy */
	while (true) { /* loop */
		bitmap.Set({ x0, y0 }, COLOR_BLACK);
		if (x0 == x1 && y0 == y1) break;
		int e2 = 2 * err;
		if (e2 >= dy) {
			err += dy; /* e_xy+e_x > 0 */
			x0 += sx;
		}
		if (e2 <= dx) { /* e_xy+e_y < 0 */
			err += dx;
			y0 += sy;
		}
	}
}
static inline void plotQuadBezierSeg(Bitmap& bitmap, int x0, int y0, int x1, int y1, int x2, int y2) {
	/* plot a limited quadratic Bezier segment */
	int sx = x2 - x1, sy = y2 - y1;
	long xx = x0 - x1, yy = y0 - y1, xy; /* relative values for checks */
	double dx, dy, err, cur = xx * sy - yy * sx; /* curvature */
	if (sx * (long)sx + sy * (long)sy > xx * xx + yy * yy) { /* begin with longer part */
		x2 = x0; x0 = sx + x1; y2 = y0; y0 = sy + y1; cur = -cur; /* swap P0 P2 */
	}
	if (cur != 0) { /* no straight line */
		xx += sx; xx *= sx = x0 < x2 ? 1 : -1; /* x step direction */
		yy += sy; yy *= sy = y0 < y2 ? 1 : -1; /* y step direction */
		xy = 2 * xx * yy; xx *= xx; yy *= yy; /* differences 2nd degree */
		if (cur * sx * sy < 0) { /* negated curvature? */
			xx = -xx; yy = -yy; xy = -xy; cur = -cur;
		}
		dx = 4.0 * sy * cur * (x1 - x0) + xx - xy; /* differences 1st degree */
		dy = 4.0 * sx * cur * (y0 - y1) + yy - xy;
		xx += xx; yy += yy; err = dx + dy + xy; /* error 1st step */
		do {
			bitmap.Set({ x0, y0 }, COLOR_BLACK); /* plot curve */
			if (x0 == x2 && y0 == y2) return; /* last pixel -> curve hasStartLineDrawn */
			y1 = 2 * err < dx; /* save value for test of y step */
			if (2 * err > dy) { x0 += sx; dx -= xy; err += dy += yy; } /* x step */
			if (y1) { y0 += sy; dy -= xy; err += dx += xx; } /* y step */
		} while (dy < 0 && dx > 0); /* gradient negates -> algorithm fails */
	}
	DrawBezier2(bitmap, { x0, y0 }, { x2, y2 }); /* plot remaining part to point */
}
static inline void DrawBezier3(Bitmap& bitmap, Pos pos1, Pos pos2, Pos pos3) {
	int x0 = pos1.x, x1 = pos2.x, x2 = pos3.x,
		y0 = pos1.y, y1 = pos2.y, y2 = pos3.y;
	int x = x0 - x1, y = y0 - y1;
	double t = x0 - 2 * x1 + x2, r;
	if ((long)x * (x2 - x1) > 0) { /* horizontal cut at P4? */
		if ((long)y * (y2 - y1) > 0) /* vertical cut at P6 too? */
			if (fabs((y0 - 2 * y1 + y2) / t * x) > abs(y)) { /* which first? */
				x0 = x2; x2 = x + x1; y0 = y2; y2 = y + y1; /* swap points */
			} /* now horizontal cut at P4 comes first */
		t = (x0 - x1) / t;
		r = (1 - t) * ((1 - t) * y0 + 2.0 * t * y1) + t * t * y2; /* By(t=P4) */
		t = (x0 * x2 - x1 * x1) * t / (x0 - x1); /* gradient dP4/dx=0 */
		x = floor(t + 0.5); y = floor(r + 0.5);
		r = (y1 - y0) * (t - x0) / (x1 - x0) + y0; /* intersect P3 | P0 P1 */
		plotQuadBezierSeg(bitmap, x0, y0, x, floor(r + 0.5), x, y);
		r = (y1 - y2) * (t - x2) / (x1 - x2) + y2; /* intersect P4 | P1 P2 */
		x0 = x1 = x; y0 = y; y1 = floor(r + 0.5); /* P0 = P4, P1 = P8 */
	}
	if ((long)(y0 - y1) * (y2 - y1) > 0) { /* vertical cut at P6? */
		t = y0 - 2 * y1 + y2; t = (y0 - y1) / t;
		r = (1 - t) * ((1 - t) * x0 + 2.0 * t * x1) + t * t * x2; /* Bx(t=P6) */
		t = (y0 * y2 - y1 * y1) * t / (y0 - y1); /* gradient dP6/dy=0 */
		x = floor(r + 0.5); y = floor(t + 0.5);
		r = (x1 - x0) * (t - y0) / (y1 - y0) + x0; /* intersect P6 | P0 P1 */
		plotQuadBezierSeg(bitmap, x0, y0, floor(r + 0.5), y, x, y);
		r = (x1 - x2) * (t - y2) / (y1 - y2) + x2; /* intersect P7 | P1 P2 */
		x0 = x; x1 = floor(r + 0.5); y0 = y1 = y; /* P0 = P6, P1 = P7 */
	}
	plotQuadBezierSeg(bitmap, x0, y0, x1, y1, x2, y2); /* remaining part */
}
}

bool WndDesign::TTFFile::Rasterize(wchar_t ch, Bitmap& bitmap) const {
	// Find the glyphID of character ch from campTable.
	// Returns 0 if not found.
	USHORT glyphID = cmapTb->find(ch);

	// Get the horizontal metric of the glyph from hmtxTable.
	longHorMetric hMetric = hmtxTb->GetGlyphMetric(glyphID);

	// Get the glyph from glyfTable.
	const Glyph* glyph = glyfTb->GetGlyph(glyphID);

	if (glyph == nullptr) { // A character without an outline. Or a composite glyph.
		bitmap.Resize({ 16, 16 });	
		bitmap.Clear(COLOR_WHITE);
		return false;
	}


	// Now begin painting with the glyph.

	WndDesign::Size size = { glyph->xMax - glyph->xMin + 1, glyph->yMax - glyph->yMin + 1 };
	bitmap.Resize(size);
	bitmap.Clear(COLOR_WHITE);

	// For simple glyphs only.
	const SimpleGlyph* simpleGlyph = glyph->simpleGlyph;
	USHORT p = 0;

//#ifdef _DEBUG
//	for (uint i = 0; i < glyph->numberOfContours; ++i) {
//		printf("\n Contour %d:\n", i);
//		for (; p <= simpleGlyph->endPtsOfContours[i]; ++p) {
//			UBYTE flag = simpleGlyph->flags[p];
//			if (flag & SimpleGlyph::ON_CURVE_POINT) {
//				printf("  On    ");
//			} else {
//				printf("  Off   ");
//			}
//			Pos point = { simpleGlyph->xCoordinates[p], simpleGlyph->yCoordinates[p] };
//			printf("%4d, %4d\n", point.x, point.y);
//		}
//	}
//	__asm int 3
//#endif // _DEBUG

	p = 0;
	for (uint i = 0; i < glyph->numberOfContours; ++i) {
		USHORT startPoint = p;
		bool hasStartLineDrawn = false;

		// Use an automaton to read and draw points.
		enum class State {
			Init, One, Two
		}state = State::Init;
		Pos points[2];

	DrawContour:
		USHORT currentPoint = startPoint;
		for (; currentPoint <= simpleGlyph->endPtsOfContours[i]; ++currentPoint) {
			UBYTE flag = simpleGlyph->flags[currentPoint];
			Pos point = { simpleGlyph->xCoordinates[currentPoint] - glyph->xMin,
						  simpleGlyph->yCoordinates[currentPoint] - glyph->yMin };
			point.y = size.height - point.y - 1; // Convert the y coordinate for bitmap.
			if (flag & SimpleGlyph::ON_CURVE_POINT) {
				switch (state) {
					case State::One: DrawBezier2(bitmap, points[0], point); break;
					case State::Two: DrawBezier3(bitmap, points[0], points[1], point); break;
				}
				points[0] = point;
				state = State::One;
				if (hasStartLineDrawn == true) { break; }
			} else {
				if (state == State::Init) { continue; }
				if (state == State::Two) {
					Pos mid = points[1] + point; mid.x /= 2; mid.y /= 2;
					DrawBezier3(bitmap, points[0], points[1], mid);
					points[0] = mid;
				}
				points[1] = point;
				state = State::Two;
			}
		}
		if (!hasStartLineDrawn) {  // Draw the start point.
			p = currentPoint;
			hasStartLineDrawn = true;
			goto DrawContour;
		}
	}

	return true;
}