#pragma once

#include "types.h"

struct headTable {
	Fixed	version;			//Table version number 0x00010000 for version 1.0. 
	Fixed	fontRevision;		// Set by font manufacturer. 
	ULONG	checkSumAdjustment;	// To compute: set it to 0, sum the entire font as ULONG, then store 0xB1B0AFBA - sum. 
	ULONG	magicNumber;		// Set to 0x5F0F3CF5. 
	USHORT	flags;				// lots of flags...
	/*
	Bit 0: Baseline for font at y=0;
	Bit 1: Left sidebearing point at x=0;
	Bit 2: Instructions may depend on point size;
	Bit 3: Force ppem to integer values for all internal scaler math; may use fractional ppem sizes if this bit is clear;
	Bit 4: Instructions may alter advance width (the advance widths might not scale linearly);
	Bits 5-10: These should be set according to Apple's specification . However, they are not implemented in OpenType.
	Bit 11: Font data is 'lossless,' as a result of having been compressed and decompressed with the Agfa MicroType Express engine.
	Bit 12: Font converted (produce compatible metrics)
	Bit 13: Font optimised for ClearType
	Bit 14: Reserved, set to 0
	Bit 15: Reserved, set to 0
	 */
	USHORT unitsPerEm;		// Valid range is from 16 to 16384. This value should be a power of 2 for fonts that have TrueType outlines. 
	LONGDATETIME created;	// Number of seconds since 12:00 midnight, January 1, 1904. 64-bit integer 
	LONGDATETIME modified;	// Number of seconds since 12:00 midnight, January 1, 1904. 64-bit integer 
	SHORT xMin;				// For all glyph bounding boxes. 
	SHORT yMin;				// For all glyph bounding boxes. 
	SHORT xMax;				// For all glyph bounding boxes. 
	SHORT yMax;				// For all glyph bounding boxes. 
	USHORT macStyle;		//
	/*
	Bit 0: Bold (if set to 1);
	Bit 1: Italic (if set to 1)
	Bit 2: Underline (if set to 1)
	Bit 3: Outline (if set to 1)
	Bit 4: Shadow (if set to 1)
	Bit 5: Condensed (if set to 1)
	Bit 6: Extended (if set to 1)
	Bits 7-15: Reserved (set to 0).
	*/
	USHORT lowestRecPPEM;		//Smallest readable size in pixels. 
	SHORT fontDirectionHint;	//
	/*
	0: Fully mixed directional glyphs;
	1: Only strongly left to right;
	2: Like 1 but also contains neutrals;
	-1: Only strongly right to left;
	-2: Like -1 but also contains neutrals. 1
	*/
	SHORT indexToLocFormat;	// 0 for short offsets, 1 for long. 
	SHORT glyphDataFormat;	// 0 for current format. 

	void read(const void* const data, uint offset) {
		ReadVar(version);
		ReadVar(fontRevision);
		ReadVar(checkSumAdjustment);
		ReadVar(magicNumber);
		ReadVar(flags);
		ReadVar(unitsPerEm);
		ReadVar(created);
		ReadVar(modified);
		ReadVar(xMin);
		ReadVar(yMin);
		ReadVar(xMax);
		ReadVar(yMax);
		ReadVar(macStyle);
		ReadVar(lowestRecPPEM);
		ReadVar(fontDirectionHint);
		ReadVar(indexToLocFormat);
		ReadVar(glyphDataFormat);
	}
};