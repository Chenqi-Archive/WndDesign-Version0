#pragma once

#include "types.h"
#include "locaTable.h"

struct SimpleGlyph {
	USHORT* endPtsOfContours;	// Array of last points of each contour; n is the number of contours. 
	USHORT instructionLength;	// Total number of bytes for instructions. 
	UBYTE* instructions;		// Array of instructions for each glyph

	UBYTE* flags;				// Unpacked flags
	SHORT* xCoordinates;		// Unpacked x coordinates
	SHORT* yCoordinates;		// Unpacked y coordinates

	USHORT numPoints;

	enum FlagBitMask {
		ON_CURVE_POINT							= 0x01,
		X_SHORT_VECTOR							= 0x02,
		Y_SHORT_VECTOR							= 0x04,
		REPEAT_FLAG								= 0x08,
		X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR	= 0x10,
		Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR	= 0x20,
		OVERLAP_SIMPLE							= 0x40,
		Reserved								= 0x80,  // zero
	};

	void read(const void* const data, uint offset, SHORT numberOfContours) {
		ReadVarArrayNew(USHORT, endPtsOfContours, (uint)numberOfContours);
		ReadVar(instructionLength);
		ReadVarArrayNew(UBYTE, instructions, instructionLength);

		numPoints = endPtsOfContours[numberOfContours - 1] + 1;
		flags = new UBYTE[numPoints];

		UBYTE repeat = 0;
		UBYTE flagVal;
		for (USHORT i = 0; i < numPoints; ++i) {
			if (repeat > 0) { 
				repeat--;
			} else {
				ReadVar(flagVal);
				if (flagVal & 0x8) { ReadVar(repeat); }
			}
			flags[i] = flagVal;
		}
		xCoordinates = new SHORT[numPoints];
		SHORT last = 0;
		for (USHORT i = 0; i < numPoints; ++i) {
			if (flags[i] & X_SHORT_VECTOR) {
				UBYTE x; ReadVar(x);
				if (flags[i] & X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) {
					xCoordinates[i] = x;
				} else {
					xCoordinates[i] = -(SHORT)x;
				}
				xCoordinates[i] += last;
			} else {
				if (flags[i] & X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) {
					xCoordinates[i] = last; // Not delta value.
				} else {
					ReadVar(xCoordinates[i]);
					xCoordinates[i] += last;
				}
			}
			last = xCoordinates[i];
		}
		yCoordinates = new SHORT[numPoints];
		last = 0;
		for (USHORT i = 0; i < numPoints; ++i) {
			if (flags[i] & Y_SHORT_VECTOR) {
				UBYTE y; ReadVar(y);
				if (flags[i] & Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR) {
					yCoordinates[i] = y;
				} else {
					yCoordinates[i] = -(SHORT)y;
				}
				yCoordinates[i] += last;
			} else {
				if (flags[i] & Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR) {
					yCoordinates[i] = last;
				} else {
					ReadVar(yCoordinates[i]);
					yCoordinates[i] += last;
				}
			}
			last = yCoordinates[i];
		}
	}

	~SimpleGlyph() {
		delete[] endPtsOfContours;
		delete[] instructions;
		delete[] flags;
		delete[] xCoordinates;
		delete[] yCoordinates;
	}
};

struct CompositeGlyph {

};

struct Glyph {
	SHORT numberOfContours;
	SHORT xMin;	// Minimum x for coordinate data. 
	SHORT yMin;	// Minimum y for coordinate data. 
	SHORT xMax;	// Maximum x for coordinate data. 
	SHORT yMax;	// Maximum y for coordinate data. 

	union {
		SimpleGlyph* simpleGlyph;
		CompositeGlyph* compositeGlyph;
	};

	void read(const void* const data, uint offset) {
		ReadVar(numberOfContours);
		ReadVar(xMin);
		ReadVar(yMin);
		ReadVar(xMax);
		ReadVar(yMax);

		if (numberOfContours >= 0) {
			simpleGlyph = new SimpleGlyph;
			simpleGlyph->read(data, offset, numberOfContours);
		} else {
			//compositeGlyph = new CompositeGlyph;
		}
	}

	~Glyph() {
		if (numberOfContours >= 0) {
			delete simpleGlyph;
		} else {
			//
		}
	}
};

struct glyfTable {
	Glyph* glyphs;

	USHORT numGlyphs;

	void read(const void* const data, uint offset, const locaTable* locaTb) {
		this->numGlyphs = locaTb->numGlyphs;
		glyphs = new Glyph[this->numGlyphs];
		if (locaTb->indexToLocFormat == 0) {
			for (uint i = 0; i < locaTb->numGlyphs; ++i) {
				if (locaTb->offsets16[i] == locaTb->offsets16[i + 1]) {
					// A character without an outline.
					glyphs[i] = {}; continue;
				}
				glyphs[i].read(data, offset + locaTb->offsets16[i] * 2);
			}
		}
		if (locaTb->indexToLocFormat == 1) {
			for (uint i = 0; i < locaTb->numGlyphs; ++i) {
				if (locaTb->offsets32[i] == locaTb->offsets32[i + 1]) {
					// A character without an outline.
					glyphs[i] = {}; continue;
				}
				glyphs[i].read(data, offset + locaTb->offsets32[i]);
			}
		}
	}

	const Glyph* GetGlyph(USHORT glyphID) const {
		if (glyphID >= numGlyphs) { return nullptr; }
		// A character without an outline. Or a composite glyph.
		if (glyphs[glyphID].numberOfContours <= 0 ) { return nullptr; }
		return &glyphs[glyphID];
	}

	~glyfTable() {
		delete[] glyphs;
	}
};