#pragma once

#include "types.h"

struct locaTable {
	union {
		USHORT* offsets16;
		ULONG*	offsets32;
	};

	USHORT numGlyphs;
	SHORT indexToLocFormat;

	void read(const void* const data, uint offset, USHORT numGlyphs, SHORT indexToLocFormat) {
		this->numGlyphs = numGlyphs;
		this->indexToLocFormat = indexToLocFormat;

		if (indexToLocFormat == 0) {
			ReadVarArrayNew(USHORT, offsets16, (uint)numGlyphs + 1);
		} else if (indexToLocFormat == 1) {
			ReadVarArrayNew(ULONG, offsets32, (uint)numGlyphs + 1);
		}
	}

	ULONG GetGlyphOffset(USHORT glyphID) const {
		if (offsets16 == nullptr || glyphID > numGlyphs) { return 0; }

		if (indexToLocFormat == 0) {
			return (ULONG)offsets16[glyphID] * 2;
		} else if (indexToLocFormat == 1) {
			return offsets32[glyphID];
		}
	}


	~locaTable() {
		if (indexToLocFormat == 0) { delete[] offsets16; }
		else if (indexToLocFormat == 1) { delete[] offsets32; }
	}
};