#pragma once

#include "types.h"

struct maxpTable {
	Fixed  version;					//Table version number 0x00005000 for version 0.5 
	USHORT numGlyphs;				// The number of glyphs in the font. 
	USHORT maxPoints;				// Maximum points in a non-composite glyph. 
	USHORT maxContours;				// Maximum contours in a non-composite glyph. 
	USHORT maxCompositePoints;		// Maximum points in a composite glyph. 
	USHORT maxCompositeContours;	// Maximum contours in a composite glyph. 
	USHORT maxZones;				// 1 if instructions do not use the twilight zone (Z0), or 2 if instructions do use Z0; should be set to 2 in most cases. 
	USHORT maxTwilightPoints;		// Maximum points used in Z0. 
	USHORT maxStorage;				// Number of Storage Area locations.  
	USHORT maxFunctionDefs;			// Number of FDEFs. 
	USHORT maxInstructionDefs;		// Number of IDEFs. 
	USHORT maxStackElements;		// Maximum stack depth2. 
	USHORT maxSizeOfInstructions;	// Maximum byte count for glyph instructions. 
	USHORT maxComponentElements;	// Maximum number of components referenced at "top level" for any composite glyph. 
	USHORT maxComponentDepth;		// Maximum levels of recursion; 1 for simple components. 

	void read(const void* const data, uint offset) {
		ReadVar(version);
		ReadVar(numGlyphs);
		ReadVar(maxPoints);
		ReadVar(maxContours);
		ReadVar(maxCompositePoints);
		ReadVar(maxCompositeContours);
		ReadVar(maxZones);
		ReadVar(maxTwilightPoints);
		ReadVar(maxStorage);
		ReadVar(maxFunctionDefs);
		ReadVar(maxInstructionDefs);
		ReadVar(maxStackElements);
		ReadVar(maxSizeOfInstructions);
		ReadVar(maxComponentElements);
		ReadVar(maxComponentDepth);
	}
};